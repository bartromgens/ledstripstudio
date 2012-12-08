#include "frame.h"

Frame::Frame(int nLEDs)
  : m_nLEDs(nLEDs),
    m_leds()
{
  m_leds.reserve(m_nLEDs);
  for (int i = 0; i < m_nLEDs; ++i)
  {
    LED emptyLed(i, Color(0, 0, 0));
    m_leds.push_back(emptyLed);
  }
}

Frame::~Frame()
{
}

const std::vector<LED>&
Frame::getLEDs() const
{
  return m_leds;
}

void
Frame::addLED(const LED& led)
{
  if (led.getLEDnr() > 0 && led.getLEDnr() < m_nLEDs)
  {
    m_leds[led.getLEDnr()] = led;
  }
}

void
Frame::print()
{
  for (std::size_t i = 0; i < m_leds.size(); ++i)
  {
    m_leds[i].getColor().print();
  }
}

void
Frame::setAllLedsOff()
{
  for (int i = 1; i <= m_nLEDs; ++i)
  {
    LED led(i, Color());
    m_leds[i] = led;
  }
}

void
Frame::amplifyRGB(double facR, double facG, double facB)
{
  for (std::size_t i = 0; i < m_leds.size(); ++i)
  {
    const Color& colorOld = m_leds[i].getColor();
    m_leds[i].setColor(Color(colorOld.r * facR, colorOld.g * facG, colorOld.b * facB));
  }
}
