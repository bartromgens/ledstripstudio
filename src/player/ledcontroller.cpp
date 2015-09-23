#include "ledcontroller.h"
#include "basic/universalsleep.h"

#include <boost/asio.hpp>


LEDController::LEDController(const QString& serialPortName)
: m_serialPortName(serialPortName),
  m_timer(),
  m_fpsHistory(),
  m_io_service(new boost::asio::io_service()),
  m_serialPort()
{
  m_timer.start();
}


LEDController::~LEDController()
{
  std::cout << "LEDController::~LEDController()" << std::endl;
  disconnect();
}


bool
LEDController::connect()
{
  try
  {
    m_serialPort.reset( new boost::asio::serial_port(*m_io_service, m_serialPortName.toStdString()) );
  }
  catch( std::exception& e )
  {
    std::cout << "LEDController::LEDController() - warning: " << e.what() << std::endl;
    std::cout << "LEDController::LEDController() - could not create a serial connection on " << m_serialPortName.toStdString() << std::endl;
    return false;
  }


//  unsigned int baud = 921600;
  unsigned int baud = 2000000;
  boost::asio::serial_port_base::baud_rate baud_option(baud);
  boost::asio::serial_port_base::flow_control flowControl(boost::asio::serial_port_base::flow_control::none);
  boost::asio::serial_port_base::parity parityType(boost::asio::serial_port_base::parity::none);
  boost::asio::serial_port_base::stop_bits stopBits(boost::asio::serial_port_base::stop_bits::two);

  m_serialPort->set_option(baud_option); // set the baud rate after the port has been opened
  m_serialPort->set_option(flowControl);
  m_serialPort->set_option(parityType);
  m_serialPort->set_option(boost::asio::serial_port_base::character_size(8));
  m_serialPort->set_option(stopBits);

  return true;
}


void
LEDController::disconnect()
{
  if (m_serialPort)
  {
    boost::system::error_code err;
    m_serialPort->close(err);
    if (err)
    {
      std::cerr << "LEDController::disconnect() - ERROR: " << err.message() << std::endl;
    }
  }
}


void
LEDController::send(const Frame& frame)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  QByteArray bytes;

  const std::vector<LED>& leds = frame.getLEDs();

  // make sure n ms has elapsed since the last send, for the Arduino to process the data.
//  int minSleep = 20; //160 leds
//  int minSleep = 21/160.0 * leds.size();
  int minSleep = 20/160.0 * leds.size();
  qint64 elapsed_ms = m_timer.nsecsElapsed()/1000000;
  qint64 toSleep = minSleep - elapsed_ms;

  if (toSleep > 0)
  {
    universalsleep::sleep_ms(toSleep);
//    std::cout << "send() elapsed: " << elapsed_ms << std::endl;
//    std::cout << "send() time to sleep: " << toSleep << std::endl;
  }

  if (!m_serialPort)
  {
    m_timer.restart();
    return;
  }

  // send the frame
//  std::cout << "=================" << std::endl;
  int nLedsPerWrite = 4; // TODO: needs to be global setting, needs to be aligned with arduino code.
  int offset = frame.getOffset();
  int brightness = frame.getBrightness();
  for (std::size_t i = 0; i < leds.size()/nLedsPerWrite; ++i)
  {
    for (int j = 0; j < nLedsPerWrite; ++j)
    {
      int pos = i*nLedsPerWrite + j ;
//      std::cout << pos << std::endl;
      addLedByte(bytes, leds, pos, offset, brightness);
    }

    writeBytes(bytes);
    bytes.clear();
  }

  m_fpsHistory.push_back( 1000.0 / (m_timer.nsecsElapsed()/1000000) );
  if (m_fpsHistory.size() > 10)
  {
    m_fpsHistory.pop_front();
  }

//  std::cout << "LEDController::send() - time: " << (m_timer2.nsecsElapsed()/1000000) << std::endl;
  m_timer.restart();
  clearAll();
}


void
LEDController::addLedByte(QByteArray& bytes, const std::vector<LED>& leds, int pos, int offset, int brightness)
{
  std::uint8_t red = leds.at(pos).getColor().r * brightness/100.0;
  std::uint8_t green = leds.at(pos).getColor().g * brightness/100.0;
  std::uint8_t blue = leds.at(pos).getColor().b * brightness/100.0;

  std::uint8_t ledPos = (leds.at(pos).getLEDnr() + offset) % leds.size();
  bytes.append( ledPos );
  bytes.append( std::max(red, static_cast<std::uint8_t>(0) ) );
  bytes.append( std::max(green, static_cast<std::uint8_t>(0) ) );
  bytes.append( std::max(blue, static_cast<std::uint8_t>(0) ) );
}


void
LEDController::writeBytes(const QByteArray& bytes)
{
  boost::system::error_code error;
  boost::asio::write(*m_serialPort, boost::asio::buffer(bytes.constData(), bytes.size()), boost::asio::transfer_all(), error);
  if (error)
  {
    std::cout << "LEDController::send() - error: " << error.message() << std::endl;
  }
}


void
LEDController::clearAll()
{
  m_ledNr.clear();
  m_color.clear();
}


int
LEDController::getFPS() const
{
  if (m_fpsHistory.empty())
  {
    return 0;
  }

  int sumFPS = 0;
  for (std::size_t i = 0; i < m_fpsHistory.size(); ++i)
  {
    sumFPS += m_fpsHistory[i];
  }

  int averageFPS = sumFPS/m_fpsHistory.size();

  return averageFPS;
}
