#include "studio.h"

#include <algorithm>

Studio::Studio(int m_nLEDs)
  : m_nLEDs(m_nLEDs)
{
}


Studio::~Studio()
{
}


Animation
Studio::createRandomMovingDots(int nDots, int nFrames)
{
  Animation animation;

  std::vector<double> speeds;
  std::vector<Color> colors;
  std::vector<double> positions;
  std::vector<int> positions_int;

  for (int i = 0; i < nDots; ++i)
  {
    double speed = (rand() % 1000) / 2000.0;
    speeds.push_back(speed);
    int startPos = i*30;
    positions.push_back(startPos);
    positions_int.push_back(startPos);

    int r = (rand() % 128);
    int g = (rand() % 128);
    int b = (rand() % 128);

    r = 0;
    g = 0;
    b = 0;

    switch (i % 3)
    {
    case 0:
    {
      r = 127;
      break;
    }
    case 1:
    {
      g = 127;
      break;
    }
    case 2:
    {
      b = 127;
      break;
    }
    }

    colors.push_back(Color(r, g, b));
  }


  for (int j = 0; j < nFrames; ++j)
  {
    Frame frame(m_nLEDs);
    std::vector<int> usedPositions;

    for (int i = 0; i < nDots; ++i)
    {
      positions[i] += speeds[i];

      int pos = int(positions[i]) % m_nLEDs;
      LED led(pos, colors[i]);
      frame.addLED(led);
      usedPositions.push_back(pos);
    }

    for (int i = 0; i < nDots; ++i)
    {
      if (std::count(usedPositions.begin(), usedPositions.end(), int(usedPositions[i])) == 2)
      {
        speeds[i] *= -1.0;
        positions[i] += speeds[i];
      }
    }
    usedPositions.clear();
    animation.addFrame(frame);
  }
  return animation;
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
    int posInt = int(pos);
    if (posInt< (m_nLEDs-2))
    {
      LED led( posInt, Color(int(r-50*dx), int(g-50*dx), int(b-50*dx)) );
      frame.addLED(led);
      led = LED( posInt+1, Color(int(r-40*dx1), int(g-50*dx1), int(b-50*dx1)) );
      frame.addLED(led);
      led = LED( posInt + 2, Color(int(r-40*dx3), int(g-50*dx3), int(b-50*dx3)) );
      frame.addLED(led);
      if (pos > 3.0)
      {
        led = LED( posInt-1, Color(int(r-40*dx2), int(g-40*dx2), int(b-40*dx2)) );
        frame.addLED(led);
        led = LED( posInt-2, Color(int(r-40*dx4), int(g-40*dx4), int(b-40*dx4)) );
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
  int r = 0;
  int g = 0;
  int b = 0;

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
