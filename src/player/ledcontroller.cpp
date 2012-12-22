#include "ledcontroller.h"

//#include "qextserialport.h"
#include "/home/bart/prog/arduinoControl/qextserialport-1.2beta2/src/qextserialport.h"

#include <unistd.h>  // for (u)sleep on Linux

LEDController::LEDController()
  : m_port(),
    m_serialPortName("/dev/ttyACM0"),
    m_timer(),
    m_timer2(),
    m_fpsHistory(),
    m_io_service(new boost::asio::io_service()),
    m_serialPort(new boost::asio::serial_port(*m_io_service, m_serialPortName.toStdString()))
{
  m_timer.start();
  m_timer2.start();
  usleep(10*1000);

  unsigned int baud = 2000000;
//  unsigned int baud = 9600;
  boost::asio::serial_port_base::baud_rate baud_option(baud);
  m_serialPort->set_option(baud_option); // set the baud rate after the port has been opened
}


LEDController::~LEDController()
{
  std::cout << "LEDController::~LEDController()" << std::endl;
  disconnect();
  delete m_serialPort;
  delete m_io_service;
}


std::unique_ptr<QextSerialPort>
LEDController::createPort()
{
  std::cout << "LEDController::createPort() - port name: " << m_serialPortName.toStdString() << std::endl;
  std::unique_ptr<QextSerialPort> port(new QextSerialPort(m_serialPortName, QextSerialPort::EventDriven));
  port->setBaudRate(BAUD2000000);
  port->setFlowControl(FLOW_OFF);
  port->setParity(PAR_NONE);
  port->setDataBits(DATA_8);
  port->setStopBits(STOP_2);

  return port;
}


void
LEDController::connect()
{
//  m_port = createPort();
//  m_port->open(QIODevice::ReadWrite);
}

void
LEDController::disconnect()
{
 m_port->close();
}


void
LEDController::send(const Frame &frame)
{
  QByteArray bytes;
  std::string message = "";

  const std::vector<LED>& leds = frame.getLEDs();

  // make sure n ms has elapsed since the last send, for the Arduino to process the data.
  int minSleep = 20;
  int elapsed = m_timer.elapsed();
  int toSleep = minSleep - elapsed;

  if (toSleep > 0)
  {
    usleep(toSleep*1000);
  }

//  std::cout << "send() time to sleep: " << toSleep << std::endl;
//  std::cout << "LEDController::send() - elapsed since last send: " <<  m_timer2.elapsed() << "[ms]" << std::endl;
  m_timer2.restart();

  // send the frame
  for (std::size_t i = 0; i < leds.size(); ++i)
  {
    int red = leds.at(i).getColor().r;
    int green = leds.at(i).getColor().g;
    int blue = leds.at(i).getColor().b;

    message += leds.at(i).getLEDnr();
    message += std::max(red, 0);
    message += std::max(green, 0);
    message += std::max(blue, 0);
//    message.append(std::max(red, 0));
//    message.append(std::max(green, 0));
//    message.append(std::max(blue, 0));

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
//    std::cout << "LEDController::send() - write success!" << std::endl;
//    std::cout << "LEDController::send() - message: " << message << std::endl;

//    m_port->write(bytes);
    bytes.clear();
  }

  m_fpsHistory.push_back(1000.0/m_timer.elapsed());
  if (m_fpsHistory.size() > 10)
  {
    m_fpsHistory.pop_front();
  }
  m_timer.restart();

//  std::cout << 1000/timerSend.elapsed() << " fps" << std::endl;

  clearAll();
}


void
LEDController::read()
{
  QByteArray result;
  int bytesAvailable = m_port->bytesAvailable();
  result.resize(bytesAvailable);
  m_port->read(result.data(), result.size());
  for (int i = 0; i < result.size(); ++i)
  {
    std::cout << int(result[i]) << std::endl;
  }
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
