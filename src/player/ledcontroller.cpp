#include "ledcontroller.h"
#include "src/basic/frame.h"

//#include "qextserialport.h"
#include "/home/bart/prog/arduinoControl/qextserialport-1.2beta2/src/qextserialport.h"

#include <QDebug>
#include <QIODevice>
#include <QRegExp>
#include <QStringList>
#include <QTime>

#include <iostream>
#include <cmath>
#include <unistd.h>  // for (u)sleep on Linux

LEDController::LEDController()
  : m_port(0),
    m_serialPortName("none"),
    m_timer()
{
  m_timer.start();
}


LEDController::~LEDController()
{
  std::cout << "LEDController::~LEDController()" << std::endl;
  disconnect();
}


void
LEDController::send(const Frame &frame)
{
  QByteArray bytes;

  const std::map<int, LED>& leds = frame.getLEDs();

  // make sure 15 ms has elapsed since the last send, for the Arduino to process the data.
  int minSleep = 15;
  int toSleep = minSleep - m_timer.elapsed();

  if (toSleep > 0)
  {
    std::cout << "sleep: " << toSleep << std::endl;
    usleep(toSleep*1000);
  }

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

    m_port->write(bytes);
    bytes.clear();
//    QByteArray result;
//    int bytesAvailable = m_port->bytesAvailable();
//    result.resize(bytesAvailable);
//    m_port->read(result.data(), result.size());
//    for (int i = 0; i < result.size(); ++i)
//    {
//      std::cout << int(result[i]) << std::endl;
//    }
  }
  std::cout << "LEDController::send() - elapsed since last send: " <<  m_timer.elapsed() << "[ms]" << std::endl;
  m_timer.restart();

//  std::cout << 1000/timerSend.elapsed() << " fps" << std::endl;

  clearAll();
}

void
LEDController::clearAll()
{
  m_ledNr.clear();
  m_color.clear();
}


void
LEDController::connect()
{
  m_port = createPort();
  m_port->open(QIODevice::ReadWrite);
}

void
LEDController::disconnect()
{
 m_port->close();
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


void
LEDController::setSerialPortName(QString serialPortName)
{
  m_serialPortName = serialPortName;
}

QextSerialPort *
LEDController::createPort()
{
  std::cout << "LEDController::createPort() - port name: " << m_serialPortName.toStdString() << std::endl;
  QextSerialPort* port = new QextSerialPort(m_serialPortName, QextSerialPort::EventDriven);
  port->setBaudRate(BAUD2000000);
  port->setFlowControl(FLOW_OFF);
  port->setParity(PAR_NONE);
  port->setDataBits(DATA_8);
  port->setStopBits(STOP_2);

  return port;
}
