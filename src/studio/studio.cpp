#include "studio.h"

#include <algorithm>
#include <random>

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
    int startPos = (rand() % m_nLEDs);
    positions.push_back(startPos);
    positions_int.push_back(startPos);

    int r = (rand() % 256);
    int g = (rand() % 256);
    int b = (rand() % 256);

    r = 0;
    g = 0;
    b = 0;

    switch (i % 3)
    {
    case 0:
    {
      r = 255;
      break;
    }
    case 1:
    {
      g = 255;
      break;
    }
    case 2:
    {
      b = 255;
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
      if (pos < 0)
      {
        pos = m_nLEDs + pos;
      }
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
Studio::createMovingLine(int startPos, int nFrames, const Color& color, double speed)
{
  Animation animation;

  int r = color.r;
  int g = color.g;
  int b = color.b;

  for (int i = 0; i < nFrames; ++i)
  {
    double pos = i*speed/2.0+startPos;
    int posInt = std::floor(pos + 0.5);

    double maxDistance = 3.0 + 0.5;

    double dx = 1.0 - std::fabs(posInt-pos) / maxDistance;
    double dx1 = 1.0 - std::fabs(posInt-pos+1) / maxDistance;
    double dx2 = 1.0 - std::fabs(posInt-pos-1) / maxDistance;
    double dx3 = 1.0 - std::fabs(posInt-pos+2) / maxDistance;
    double dx4 = 1.0 - std::fabs(posInt-pos-2) / maxDistance;
    double dx5 = 1.0 - std::fabs(posInt-pos+3) / maxDistance;
    double dx6 = 1.0 - std::fabs(posInt-pos-3) / maxDistance;

    int exponent = 3;

    double dx_3 = std::pow(dx, exponent);
    double dx1_3 = std::pow(dx1, exponent);
    double dx2_3 = std::pow(dx2, exponent);
    double dx3_3 = std::pow(dx3, exponent);
    double dx4_3 = std::pow(dx4, exponent);
    double dx5_3 = std::pow(dx5, exponent);
    double dx6_3 = std::pow(dx6, exponent);

    posInt = int(posInt) % m_nLEDs;
    Frame frame(m_nLEDs);
    if (posInt < 0)
    {
      posInt = m_nLEDs + posInt;
    }

    if (posInt < (m_nLEDs-2))
    {
      LED led( posInt, Color(int(r*dx_3), int(g*dx_3), int(b*dx_3)) );
      frame.addLED(led);
      led = LED( posInt+1, Color(int(r*dx1_3), int(g*dx1_3), int(b*dx1_3)) );
      frame.addLED(led);
      led = LED( posInt+2, Color(int(r*dx3_3), int(g*dx3_3), int(b*dx3_3)) );
      frame.addLED(led);
      led = LED( posInt+3, Color(int(r*dx5_3), int(g*dx5_3), int(b*dx5_3)) );
      frame.addLED(led);
      led = LED( posInt-1, Color(int(r*dx2_3), int(g*dx2_3), int(b*dx2_3)) );
      frame.addLED(led);
      led = LED( posInt-2, Color(int(r*dx4_3), int(g*dx4_3), int(b*dx4_3)) );
      frame.addLED(led);
      led = LED( posInt-3, Color(int(r*dx6_3), int(g*dx6_3), int(b*dx6_3)) );
      frame.addLED(led);
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
    if (pos < 0)
    {
      pos = m_nLEDs + pos;
    }

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
Studio::createMovingRainbow(double speed)
{
  Animation animation;
  // taken from https://github.com/adafruit/LPD8806/blob/master/examples/strandtest/strandtest.ino
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
      Color color = wheel( ((i * 384 / m_nLEDs) + static_cast<int>(j*speed)) % 384*2 );
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

  switch (wheelPos / 256)
  {
    case 0:
      r = 255 - wheelPos % 256;   //Red down
      g = wheelPos % 256;      // Green up
      b = 0;                  //blue off
      break;
    case 1:
      g = 255 - wheelPos % 256;  //green down
      b = wheelPos % 256;      //blue up
      r = 0;                  //red off
      break;
    case 2:
      b = 255 - wheelPos % 256;  //blue down
      r = wheelPos % 256;      //red up
      g = 0;                  //green off
      break;
  }
  return Color(r, g, b);
}


Animation
Studio::createCellularAutomata()
{
  std::cout << "Studio::createCellularAutomata()" << std::endl;
  Animation animation;
  std::size_t nSteps = 1000;
  std::vector<int> state(m_nLEDs, 0);
  state[m_nLEDs-1] = 1;


  for (std::size_t i = 0; i < nSteps; ++i)
  {
    state = rule(state);
    Frame frame(m_nLEDs);

    for (int i = 0; i < m_nLEDs; i++)
    {
      Color color;
      if (state[i] == 1)
      {
        color = Color(255, 0, 0);
      }

      LED led(i, color);
      frame.addLED(led);
    }

    animation.addFrame(frame);
    animation.addFrame(frame);
    animation.addFrame(frame);
    animation.addFrame(frame);
    animation.addFrame(frame);
  }

  return animation;
}


std::vector<int>
Studio::rule(const std::vector<int>& state)
{
  std::vector<int> stateNew(state.size(), 0);

  for (std::size_t i = 1; i < state.size()-1; ++i)
  {
    if(state[i-1] == 0 && state[i] == 0 && state[i+1] == 0)
    {
      stateNew[i] = 0;
    }
    else if(state[i-1] == 0 && state[i] == 0 && state[i+1] == 1)
    {
      stateNew[i] = 1;
    }
    else if(state[i-1] == 0 && state[i] == 1 && state[i+1] == 0)
    {
      stateNew[i] = 1;
    }
    else if(state[i-1] == 0 && state[i] == 1 && state[i+1] == 1)
    {
      stateNew[i] = 1;
    }
    else if(state[i-1] == 1 && state[i] == 0 && state[i+1] == 0)
    {
      stateNew[i] = 0;
    }
    else if(state[i-1] == 1 && state[i] == 0 && state[i+1] == 1)
    {
      stateNew[i] = 1;
    }
    else if(state[i-1] == 1 && state[i] == 1 && state[i+1] == 0)
    {
      stateNew[i] = 1;
    }
    else if(state[i-1] == 1 && state[i] == 1 && state[i+1] == 1)
    {
      stateNew[i] = 0;
    }
  }

  return stateNew;
}
