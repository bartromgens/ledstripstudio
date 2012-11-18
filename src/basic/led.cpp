#include "led.h"

LED::LED()
{
}

const Color &LED::getColor() const
{
  return m_color;
}

int LED::getLEDnr() const
{
  return m_ledNr;
}

void
LED::setColor(const Color& color)
{
  m_color = color;
}

void LED::setLEDnr(int ledNr)
{
  m_ledNr = ledNr;
}

