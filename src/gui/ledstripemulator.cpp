#include "ledstripemulator.h"

#include <QPainter>

#include <iostream>

LedStripEmulator::LedStripEmulator(QWidget* parent)
  : QWidget(parent),
    m_frame(0),
    m_height(10),
    m_width(10)
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
LedStripEmulator::setHeight(int height)
{
  m_height = height;
}

void
LedStripEmulator::setWidth(int width)
{
  m_width = width;
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

  const std::vector<LED>& leds = m_frame.getLEDs();

  for (std::size_t i = 0; i < leds.size(); ++i)
  {
    LED led = leds.at(i);
    int x = (i*m_width);
    int y = 2;
    QRect rect(x, y, m_width, m_height-2);
    qp->drawRect(rect);
    QColor color(std::max(int(led.getColor().r*254/127.0), 0), std::max(int(led.getColor().g*254/127.0), 0), std::max(int(led.getColor().b*254/127.0), 0));
    qp->fillRect(rect, color);
  }
}
