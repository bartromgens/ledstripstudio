#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "basic/color.h"
#include "basic/frame.h"

#include <QString>
#include <QElapsedTimer>

#include <boost/asio/serial_port.hpp>

#include <deque>
#include <mutex>
#include <memory>
#include <string>
#include <vector>


namespace boost
{
  namespace asio
  {
    class io_service;
  }
}

class LEDController
{

public:

  explicit LEDController();
  ~LEDController();

  bool connect(const std::string& serialPortName);
  void send(const Frame& frame, double minSleep_ms);

  bool isConnected() const;

  int getFPS() const;

private:

  void disconnect();

  void addLedByte(QByteArray &bytes, const std::vector<LED>& leds, int pos, int offset, int brightness);
  void writeBytes(const QByteArray& bytes);

private:

  QElapsedTimer m_timer;
  std::deque<int> m_fpsHistory;
  mutable std::mutex m_mutex;

  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::serial_port> m_serialPort; // the serial port this instance is connected to

};

#endif // LEDCONTROLLER_H
