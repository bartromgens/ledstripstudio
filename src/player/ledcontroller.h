#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "src/basic/color.h"
#include "src/basic/frame.h"

#include <QString>
#include <QTime>

#include <deque>
#include <mutex>
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
  std::unique_ptr<QextSerialPort> createPort();

private:
  std::vector<int> m_ledNr;
  std::vector<Color> m_color;
  std::vector<std::vector<Color> > m_colorOld;

  std::unique_ptr<QextSerialPort> m_port ;
  QByteArray m_byteMessage;
  QString m_serialPortName;
  QTime m_timer;
  QTime m_timer2;
  std::deque<int> m_fpsHistory;
  mutable std::mutex m_mutex;
};

#endif // LEDCONTROLLER_H
