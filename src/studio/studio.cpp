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
    LED led(i, color);
    frame.addLED(led);
  }
  Animation animation;
  animation.addFrame(frame);
  return animation;
}


Animation
Studio::createMovingLine(int nFrames, const Color& color, double speed)
{
  Animation animation;

  int r = color.r;
  int g = color.g;
  int b = color.b;

  for (int i = 0; i < nFrames; ++i)
  {
    double pos = int(i*speed+1) % m_nLEDs;
    double dx = std::fabs(int(pos)-pos);
    double dx1 = std::fabs(int(pos+1)-pos);
    double dx2 = std::fabs(int(pos-1)-pos);
    double dx3 = std::fabs(int(pos+2)-pos);
    double dx4 = std::fabs(int(pos-2)-pos);

//    std::cout << dx1 << ", " << dx2 << ", " << dx3 << ", " << dx4 << std::endl;

    Frame frame(m_nLEDs);
    if ((int)pos < (m_nLEDs-2))
    {
      LED led(int(pos), Color(r-50*dx, g-50*dx, b-50*dx));
      frame.addLED(led);
      led = LED(int(pos+1), Color(r-40*dx1, g-50*dx1, b-50*dx1));
      frame.addLED(led);
      led = LED(int(pos+2), Color(r-40*dx3, g-50*dx3, b-50*dx3));
      frame.addLED(led);
      if (pos > 3.0)
      {
        led = LED(int(pos-1), Color(r-40*dx2, g-40*dx2, b-40*dx2));
        frame.addLED(led);
        led = LED(int(pos-2), Color(r-40*dx4, g-40*dx4, b-40*dx4));
        frame.addLED(led);
      }
    }
    animation.addFrame(frame);
  }

  return animation;
}

Animation
Studio::createMovingDot(int startPos, int nFrames, const Color &color, double speed)
{
  Animation animation;

  for (int i = 1; i < nFrames; ++i)
  {
    double pos = int(i*speed+startPos) % m_nLEDs;

//    std::cout << dx1 << ", " << dx2 << ", " << dx3 << ", " << dx4 << std::endl;

    if ((int)pos < (m_nLEDs-2))
    {
      Frame frame(m_nLEDs);
      LED led(int(pos), color);
      frame.addLED(led);
      animation.addFrame(frame);
    }
  }

  return animation;
}


Animation
Studio::createMovingRainbow()
{
  Animation animation;
  for (int j = 0; j < 384 * 5; j++)
  {
    // 5 cycles of all 384 colors in the wheel
    Frame frame(m_nLEDs);
    for (int i = 0; i < m_nLEDs; i++)
    {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      Color color = wheel( ((i * 384 / m_nLEDs) + j) % 384 );
      LED led(i, color);
//      led.print();
      frame.addLED(led);
    }
    animation.addFrame(frame);
  }
  return animation;
}

Color
Studio::wheel(int wheelPos)
{
  int r;
  int g;
  int b;

  switch(wheelPos / 128)
  {
    case 0:
      r = 127 - wheelPos % 128;   //Red down
      g = wheelPos % 128;      // Green up
      b = 0;                  //blue off
      break;
    case 1:
      g = 127 - wheelPos % 128;  //green down
      b = wheelPos % 128;      //blue up
      r = 0;                  //red off
      break;
    case 2:
      b = 127 - wheelPos % 128;  //blue down
      r = wheelPos % 128;      //red up
      g = 0;                  //green off
      break;
  }
  return Color(r, g, b);
}
