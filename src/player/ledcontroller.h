#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "basic/color.h"
#include "basic/frame.h"

#include <QString>
#include <QElapsedTimer>

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/thread.hpp>

#include <deque>
//#include <mutex>
#include <memory>
#include <vector>

class QextSerialPort;

class LEDController
{
public:
  LEDController();
  ~LEDController();

  void send(const Frame& frame);

  void clearAll();

  void connect();

  void setSerialPortName(QString serialPortName);

  void setBrightness(int brightness);

  void setColor(const Color& color);
  void setLEDnr(int letdNr);

  int getFPS();

private:
  void disconnect();

  void writeBytes(const QByteArray& bytes);
  void addLedByte(QByteArray &bytes, const std::vector<LED> &leds, int pos, int offset, int brightness);

  void read();

private:
  std::vector<int> m_ledNr;
  std::vector<Color> m_color;
  std::vector<std::vector<Color> > m_colorOld;

  QByteArray m_byteMessage;
  QString m_serialPortName;
  QElapsedTimer m_timer;
  QElapsedTimer m_timer2;
  std::deque<int> m_fpsHistory;
  mutable boost::mutex m_mutex;

  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::serial_port> m_serialPort; // the serial port this instance is connected to
};

#endif // LEDCONTROLLER_H
