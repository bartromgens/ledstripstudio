#include "led.h"

#include <iostream>

LED::LED(int ledNr, const Color& color)
  : m_ledNr(ledNr),
    m_color(color)
{
}

const Color&
LED::getColor() const
{
  return m_color;
}

QColor
LED::getQColor() const
{
  double factor = 255.0 / 127.0;
  QColor qcolor(m_color.r * factor, m_color.g * factor, m_color.b * factor);
  return qcolor;
}

int
LED::getLEDnr() const
{
  return m_ledNr;
}

void
LED::setColor(const Color& color)
{
  m_color = color;
}

void
LED::setLEDnr(int ledNr)
{
  m_ledNr = ledNr;
}

void
LED::print() const
{
  std::cout << "LED : nr: " << m_ledNr << ", color: " << m_color.r << ", " << m_color.g << ", " << m_color.b << std::endl;
}

