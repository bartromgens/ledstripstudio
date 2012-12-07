#include "studio.h"
#include "src/basic/frame.h"

#include <cmath>

Studio::Studio(int m_nLEDs)
  : m_nLEDs(m_nLEDs)
{
}


Studio::~Studio()
{
}


Animation
Studio::createSingleColorSingleFrameAnimation(const Color& color) const
{
  Frame frame(m_nLEDs);

  for (int i = 0; i < m_nLEDs; ++i)
  {
    LED led;
    led.setColor(color);
    led.setLEDnr(i);
    frame.addLED(led);
  }
  Animation animation;
  animation.addFrame(frame);
  return animation;
}


Animation
Studio::createMovingLine(const Color& color, double speed)
{
  Animation animation;
  int nFrames = m_nLEDs/speed;

  int r = color.r;
  int g = color.g;
  int b = color.b;

  for (int i = 1; i < nFrames; ++i)
  {
    double pos = i*speed+1;
    double dx = std::fabs(int(pos)-pos);
    double dx1 = std::fabs(int(pos+1)-pos);
    double dx2 = std::fabs(int(pos-1)-pos);
    double dx3 = std::fabs(int(pos+2)-pos);
    double dx4 = std::fabs(int(pos-2)-pos);

//    std::cout << dx1 << ", " << dx2 << ", " << dx3 << ", " << dx4 << std::endl;

    if ((int)pos < (m_nLEDs-2))
    {
      Frame frame(m_nLEDs);
      LED led;
      led.setLEDnr(int(pos));
      led.setColor(Color(r-50*dx, g-50*dx, b-50*dx));
      frame.addLED(led);
      led = LED();
      led.setLEDnr(int(pos+1));
      led.setColor(Color(r-40*dx1, g-50*dx, b-50*dx));
      frame.addLED(led);
      led = LED();
      led.setLEDnr(int(pos+2));
      led.setColor(Color(r-40*dx3, g-50*dx, b-50*dx));
      frame.addLED(led);
      if (pos > 3.0)
      {
        led = LED();
        led.setLEDnr(int(pos-1));
        led.setColor(Color(r-40*dx2, g-40*dx2, b-40*dx2));
        frame.addLED(led);
        led = LED();
        led.setLEDnr(int(pos-2));
        led.setColor(Color(r-40*dx4, g-40*dx4, b-40*dx4));
        frame.addLED(led);
      }
      animation.addFrame(frame);
    }
  }

  return animation;
}
