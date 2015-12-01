#include "frame.h"

#include <cassert>

Frame::Frame(int nLEDs)
  : m_nLEDs(nLEDs),
    m_leds(),
    m_offset(0),
    m_brightness(100)
{
  fill();
}

Frame::~Frame()
{
}

void
Frame::fill()
{
  m_leds.reserve(m_nLEDs);
  for (int i = 0; i < m_nLEDs; ++i)
  {
    LED emptyLed(i, Color(0, 0, 0));
    m_leds.push_back(emptyLed);
  }
}

const std::vector<LED>&
Frame::getLEDs() const
{
  return m_leds;
}

void
Frame::addLED(const LED& led)
{
  if (led.getLEDnr() >= 0 && led.getLEDnr() < m_nLEDs)
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
    m_leds[i].setColor( Color(int(colorOld.r * facR), int(colorOld.g * facG), int(colorOld.b * facB)) );
  }
}

int
Frame::getOffset() const
{
  return m_offset;
}

void
Frame::setOffset(int offset)
{
  m_offset = offset;
}

int
Frame::getBrightness() const
{
  return m_brightness;
}

void
Frame::setBrightness(int brightness)
{
  m_brightness = brightness;
}

void
Frame::mirror()
{
  const std::vector<LED>& leds = getLEDs();
  for (std::size_t i = 0 ; i < leds.size()/2; ++i)
  {
    unsigned int mirrorI = leds.size()-i-1;
    LED led(mirrorI, leds[i].getColor());
    addLED(led);
  }
}
