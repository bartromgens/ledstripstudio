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

