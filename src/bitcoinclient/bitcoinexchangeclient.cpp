#include "bitcoinexchangeclient.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <set>

#include <boost/lexical_cast.hpp>

#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/assign/list_inserter.hpp>
//#include <boost/date_time/gregorian/gregorian.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::ip::tcp;

typedef boost::gregorian::date Calendar;

BitcoinExchangeClient::BitcoinExchangeClient()
{
}


BitcoinExchangeClient::~BitcoinExchangeClient()
{
}


std::string
BitcoinExchangeClient::getJsonUrl(const std::string& host, const std::string& path)
{
  using boost::asio::ip::tcp;

  std::string reply = "";

  try
  {
    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while(error && endpoint_iterator != end){
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }

    if(error)
    {
      throw boost::system::system_error(error);
    }

    // Form the request. We specify the "Connection: close" header so that the server will close the socket
    // after transmitting the response. This will allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, request);

    // Read the response status line.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);

    std::string http_version;
    response_stream >> http_version;

    unsigned int status_code;
    response_stream >> status_code;

    std::string status_message;
    std::getline(response_stream, status_message);

    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
    }
    if (status_code != 200)
    {
      std::cout << "Response returned with status code " << status_code << "\n";
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
    {
      std::ostringstream oss;
      oss << &response;
      reply += oss.str();
    }

    if(error != boost::asio::error::eof)
    {
      throw boost::system::system_error(error);
    }
  }
  catch(std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  int start = reply.find_first_of("{");

  std::string jsonString = "";

  for (std::size_t i = start; i < reply.size(); ++i)
  {
    jsonString += reply[i];
  }

  return jsonString;
}


void
BitcoinExchangeClient::getMtGoxData()
{
  std::string host = "data.mtgox.com";
  std::string path = "/api/1/BTCUSD/ticker";

  std::string reply = getJsonUrl(host, path);

  std::cout << "BitcoinExchangeClient::getMtGoxData() : " << reply << std::endl;

  // Create an empty property tree object
  using boost::property_tree::ptree;
  boost::property_tree::ptree pt;

  std::stringstream testinput;
  testinput << reply;

  try
  {
    boost::property_tree::read_json(testinput, pt);
  }
  catch( std::exception& error )
  {
    std::cout << "BitcoinExchangeClient::getMtGoxData() - ERROR: " << error.what() << std::endl;
    return;
  }

  std::string high_str = pt.get<std::string>("return.high.value");
  std::string low_str = pt.get<std::string>("return.low.value");
  std::string last_str = pt.get<std::string>("return.last_local.value");

  m_lastOld = m_last;
  m_high = boost::lexical_cast<double>( high_str );
  m_low = boost::lexical_cast<double>( low_str );
  m_last = boost::lexical_cast<double>( last_str );
}


void
BitcoinExchangeClient::getUnconfirmedTransactions()
{
  std::string host = "blockchain.info";
  std::string path = "/unconfirmed-transactions?format=json";

  std::string reply = getJsonUrl(host, path);

  std::cout << "BitcoinExchangeClient::getUnconfirmedTransactions() : " << reply << std::endl;

  // Create an empty property tree object
  using boost::property_tree::ptree;
  boost::property_tree::ptree pt;

  std::stringstream testinput;
  testinput << reply;

  try
  {
    boost::property_tree::read_json(testinput, pt);
  }
  catch( std::exception& error )
  {
    std::cout << "BitcoinExchangeClient::getUnconfirmedTransactions() - ERROR: " << error.what() << std::endl;
    return;
  }
}


void
BitcoinExchangeClient::getLatestBlock()
{
  std::string host = "blockchain.info";
  std::string path = "/latestblock?format=json";

  std::string reply = getJsonUrl(host, path);

  std::cout << "BitcoinExchangeClient::getLatestBlock() : " << reply << std::endl;

  // Create an empty property tree object
  using boost::property_tree::ptree;
  boost::property_tree::ptree pt;

  std::stringstream testinput;
  testinput << reply;

  try
  {
    boost::property_tree::read_json(testinput, pt);
  }
  catch( std::exception& error )
  {
    std::cout << "BitcoinExchangeClient::getLatestBlock() - ERROR: " << error.what() << std::endl;
    return;
  }

  std::list<int> txIndexes;
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("txIndexes"))
  {
    assert(v.first.empty()); // array elements have no names
    std::cout << v.second.data() << std::endl;
    std::string txIndexes_str = v.second.data();
    try
    {
      txIndexes.push_back(boost::lexical_cast<int>(txIndexes_str));
    }
    catch ( std::exception& error )
    {
      std::cout << "BitcoinExchangeClient::getLatestBlock() - ERROR: " << error.what() << std::endl;
    }
  }

//  std::cout << txIndexes_str << std::endl;
}
