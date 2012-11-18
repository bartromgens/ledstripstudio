#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "src/basic/color.h"
#include "src/basic/frame.h"

#include <QString>
#include <QTime>
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

private:
  void disconnect();
  QextSerialPort* createPort();

private:
  std::vector<int> m_ledNr;
  std::vector<Color> m_color;
  std::vector<std::vector<Color> > m_colorOld;

  QextSerialPort* m_port ;
  QByteArray m_byteMessage;
  QString m_serialPortName;
  QTime m_timer;
};

#endif // LEDCONTROLLER_H
