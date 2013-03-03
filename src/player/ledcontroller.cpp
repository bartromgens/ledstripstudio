#include "ledcontroller.h"
#include "basic/universalsleep.h"

LEDController::LEDController()
  : m_serialPortName(),
    m_timer(),
    m_timer2(),
    m_fpsHistory(),
    m_io_service(new boost::asio::io_service()),
    m_serialPort()
{
}


LEDController::~LEDController()
{
  std::cout << "LEDController::~LEDController()" << std::endl;
  disconnect();
}


void
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
    return;
  }

  m_timer.start();
  m_timer2.start();

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
}

void
LEDController::disconnect()
{

}


void
LEDController::send(const Frame &frame)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  QByteArray bytes;

  const std::vector<LED>& leds = frame.getLEDs();

  // make sure n ms has elapsed since the last send, for the Arduino to process the data.
  int minSleep = 21;
  int elapsed_ms = m_timer.nsecsElapsed()/1000000;
  int toSleep = minSleep - elapsed_ms;

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

  m_timer2.restart();

  // send the frame
  for (std::size_t i = 0; i < leds.size(); ++i)
  {
    int red = leds.at(i).getColor().r;
    int green = leds.at(i).getColor().g;
    int blue = leds.at(i).getColor().b;

    bytes.append( leds.at(i).getLEDnr() );
    bytes.append( std::max(red, 0) );
    bytes.append( std::max(green, 0) );
    bytes.append( std::max(blue, 0) );

    boost::system::error_code error;
    boost::asio::write(*m_serialPort, boost::asio::buffer(bytes.constData(), bytes.size()), boost::asio::transfer_all(), error);
    if (error)
    {
      std::cout << "LEDController::send() - error: " << error.message() << std::endl;
    }

    bytes.clear();
  }

  m_fpsHistory.push_back( 1000.0 / (m_timer.nsecsElapsed()/1000000) );
  if (m_fpsHistory.size() > 10)
  {
    m_fpsHistory.pop_front();
  }
//  std::cout << "LEDController::send() - time: " << (m_timer.nsecsElapsed()/1000000) << std::endl;
  m_timer.restart();

  clearAll();
}


void
LEDController::read()
{

}


void
LEDController::clearAll()
{
  m_ledNr.clear();
  m_color.clear();
}


void
LEDController::setColor(const Color& color)
{
  m_color.push_back(color);
}


void
LEDController::setLEDnr(int ledNr)
{
  m_ledNr.push_back(ledNr);
}


int
LEDController::getFPS()
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


void
LEDController::setSerialPortName(QString serialPortName)
{
  m_serialPortName = serialPortName;
}
