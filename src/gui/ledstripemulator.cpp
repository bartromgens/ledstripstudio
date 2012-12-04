#include "ledstripemulator.h"

#include <QPainter>

#include <iostream>

LedStripEmulator::LedStripEmulator(QWidget* parent)
  : QWidget(parent),
    m_frame(0)
{
}


LedStripEmulator::~LedStripEmulator()
{
}


void
LedStripEmulator::setFrame(const Frame& frame)
{
  m_frame = frame;
}


void
LedStripEmulator::paintEvent(QPaintEvent* e)
{
  Q_UNUSED(e);
  QPainter qp(this);
  drawStrip(&qp);
}


void
LedStripEmulator::drawStrip(QPainter* qp)
{
  QPen pen(Qt::black, 2, Qt::SolidLine);
  qp->setPen(pen);

  const std::map<int, LED>& leds = m_frame.getLEDs();

  for (std::size_t i = 0; i < leds.size(); ++i)
  {
    LED led = leds.at(i);
    int width = 10;
    int x = (i*width)+width;
    int y = width;
    QRect rect(x, y, width, width*8);
    qp->drawRect(rect);
    QColor color(led.getColor().r*255/127.0, led.getColor().g*255/127.0, led.getColor().b*255/127.0);
    qp->fillRect(rect, color);
  }
}
