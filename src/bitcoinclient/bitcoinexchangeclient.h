#ifndef BITCOINEXCHANGECLIENT_H
#define BITCOINEXCHANGECLIENT_H

#include <iostream>
#include <string>

class BitcoinExchangeClient
{
public:
  BitcoinExchangeClient();
  ~BitcoinExchangeClient();

  void getMtGoxData();
  void getLatestBlock();
  void getUnconfirmedTransactions();

private:
  std::string getJsonUrl(const std::string& host, const std::string& path);

public:
  double m_high;
  double m_low;
  double m_last;
  double m_lastOld;
};

#endif // BITCOINEXCHANGECLIENT_H
