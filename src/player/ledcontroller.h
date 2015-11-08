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

  explicit LEDController(const QString& serialPortName);
  ~LEDController();

  bool connect();
  void send(const Frame& frame);

  int getFPS() const;

private:

  void disconnect();

  void addLedByte(QByteArray &bytes, const std::vector<LED>& leds, int pos, int offset, int brightness);
  void writeBytes(const QByteArray& bytes);
  void clearAll();

private:

  std::vector<int> m_ledNr;
  std::vector<Color> m_color;
  std::vector<std::vector<Color> > m_colorOld;

  QByteArray m_byteMessage;
  QString m_serialPortName;
  QElapsedTimer m_timer;
  std::deque<int> m_fpsHistory;
  mutable std::mutex m_mutex;

  std::unique_ptr<boost::asio::io_service> m_io_service;
  std::unique_ptr<boost::asio::serial_port> m_serialPort; // the serial port this instance is connected to

};

#endif // LEDCONTROLLER_H
