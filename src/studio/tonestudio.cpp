#include "tonestudio.h"

#include <cmath>
#include <string>

ToneStudio::ToneStudio()
  : m_animationType(Loudest),
    m_toneHistoryFrame(0)
{
}


ToneStudio::~ToneStudio()
{
}


void
ToneStudio::setAnimationType(ToneStudio::ToneAnimationType type)
{
  m_animationType = type;
}


void
ToneStudio::writeToneToConsole(const std::map<std::string, double>& tones)
{
  for (auto iter = tones.begin(); iter != tones.end(); ++iter)
  {
    std::cout << "Tone: " << iter->first << " : " ;
    for (std::size_t i = 0; i < iter->second/15; ++i)
    {
      std::cout << "-";
    }
    std::cout << std::endl;
  }
}


Animation
ToneStudio::createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones)
{
  unsigned int speed = 1;
  switch (m_animationType)
  {
    case Loudest:
    {
      return createToneAnimationLoudest(nLEDs, tones);
    }
    case SmoothSum:
    {
      return createToneAnimationSmoothSum(nLEDs, tones);
    }
    case History:
    {
      return createToneAnimationHistory(nLEDs, tones, speed);
    }
    case None:
    {
      return Animation();
    }
  }

  return Animation();
}


Animation
ToneStudio::createToneAnimationLoudest(unsigned int nLEDs, std::map<std::string, double> tones)
{
  Animation animation;

  Frame frame(nLEDs);

  double currentMax = 0.0;
  std::string maxTone;

  for(auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > currentMax)
    {
      maxTone = it->first;
      currentMax = it->second;
    }
  }

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
    Color color;

    if (maxTone == "C")
    {
      color = Color(127, 0, 0);
    }
    else if (maxTone == "D")
    {
      color = Color(127, 127, 0);
    }
    else if (maxTone == "E")
    {
      color = Color(0, 127, 0);
    }
    else if (maxTone == "F")
    {
      color = Color(0, 127, 127);
    }
    else if (maxTone == "G")
    {
      color = Color(0, 0, 127);
    }
    else if (maxTone == "A")
    {
      color = Color(127, 0, 127);
    }
    else if (maxTone == "B")
    {
      color = Color(127, 50, 50);
    }

    LED led(i, color);
    frame.addLED(led);
  }

  animation.addFrame(frame);

  return animation;
}


Animation
ToneStudio::createToneAnimationSmoothSum(unsigned int nLEDs, std::map<std::string, double> tones)
{
  Animation animation;

  Frame frame(nLEDs);

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    for (std::map<std::string, double>::const_iterator iter = tones.begin(); iter != tones.end(); ++iter)
    {
      if (iter->first == "C")
      {
        r += iter->second;
      }
      else if (iter->first == "D")
      {
        r += iter->second/2.0;
//        g += iter->second/2.0;
      }
      else if (iter->first == "E")
      {
        g += iter->second;
      }
      else if (iter->first == "F")
      {
        g += iter->second/2.0;
//        b += iter->second/2.0;
      }
      else if (iter->first == "G")
      {
        b += iter->second;
      }
      else if (iter->first == "A")
      {
        b += iter->second/2.0;
//        r += iter->second/2.0;
      }
      else if (iter->first == "B")
      {
        r += iter->second;
      }
    }

    double norm = std::sqrt(r*r+g*g+b*b);
    int rNorm = static_cast<int>(r/norm*127);
    int gNorm = static_cast<int>(g/norm*127);
    int bNorm = static_cast<int>(b/norm*127);

    Color color(rNorm, gNorm, bNorm);

    LED led(i, color);
    frame.addLED(led);
  }

  animation.addFrame(frame);

  return animation;
}


Animation
ToneStudio::createToneAnimationHistory(unsigned int nLEDs, std::map<std::string, double> tones, unsigned int speed)
{
  Animation animation;
  Frame frame(nLEDs);
  Color color;

  double currentMax = 0.0;
  std::string maxTone;

  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > currentMax)
    {
      maxTone = it->first;
      currentMax = it->second;
    }
  }

  int brightness = std::min(static_cast<int>(pow(currentMax/25.0, 4)), 127);

  if (maxTone == "C")
  {
    color = Color( brightness, brightness/2, 0);
  }
  else if (maxTone == "D")
  {
    color = Color(brightness, 0, 0);
  }
  else if (maxTone == "E")
  {
    color = Color(0, brightness, 0);
  }
  else if (maxTone == "F")
  {
    color = Color(0, brightness, brightness);
  }
  else if (maxTone == "G")
  {
    color = Color(0, 0, brightness);
  }
  else if (maxTone == "A")
  {
    color = Color(brightness, 0, brightness);
  }
  else if (maxTone == "B")
  {
    color = Color(127, 50, 50);
  }

  std::vector<LED> leds = m_toneHistoryFrame.getLEDs();
  for (std::size_t i = 0 ; i < leds.size()/2; ++i)
  {
    if (i < leds.size()/2-1)
    {
      leds[i].setLEDnr(leds[i].getLEDnr() + speed);
      leds[leds.size()-i].setLEDnr(leds[leds.size()-i].getLEDnr() - speed);
      frame.addLED(leds[i]);
      frame.addLED(leds[leds.size()-i]);
    }
  }

  for (unsigned int i = 1; i <= speed; ++i)
  {
    LED led(i, color);
    frame.addLED(led);
    LED led2(leds.size()-i, color);
    frame.addLED(led2);
  }

  animation.addFrame(frame);
  m_toneHistoryFrame = frame;

  return animation;
}
