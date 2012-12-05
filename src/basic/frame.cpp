#include "frame.h"

Frame::Frame(int nLEDs)
  : m_nLEDs(nLEDs),
    m_leds()
{
  for (int i = 0; i < nLEDs; ++i)
  {
    LED led;
    led.setColor(Color());
    led.setLEDnr(i);
    m_leds[i] = led;
  }
}

const std::map<int, LED>&
Frame::getLEDs() const
{
  return m_leds;
}

void
Frame::addLED(LED led)
{
  m_leds[led.getLEDnr()] = led;
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
    LED led;
    led.setColor(Color());
    led.setLEDnr(i);
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
