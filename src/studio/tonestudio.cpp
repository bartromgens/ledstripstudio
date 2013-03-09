#include "tonestudio.h"

#include "studio.h"

#include <cmath>
#include <string>

ToneStudio::ToneStudio()
  : m_animationType(Loudest),
    m_toneHistoryFrame(0),
    m_toneColorMap()
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
    case Individual:
    {
      return createToneAnimationIndividual(nLEDs, tones);
    }
    case Corner:
    {
      return createToneAnimationCorners(nLEDs, tones);
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

  double maxAmplitude = 0.0;
  std::string maxTone;

  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > maxAmplitude)
    {
      maxTone = it->first;
      maxAmplitude = it->second;
    }
  }

  int brightness = std::min(static_cast<int>(pow(maxAmplitude/25.0, 4)), 127);

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


Animation
ToneStudio::createToneAnimationIndividual(unsigned int nLEDs, std::map<std::string, double> tones)
{
  if (m_toneColorMap.empty())
  {
    createToneColorMap(tones);
  }

  Animation animation;
  Frame frame(nLEDs);

  double maxAmplitude = 0.0;

  // calculate max amplitude
  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > maxAmplitude)
    {
      maxAmplitude = it->second;
    }
  }

  // if low amplitude, make strip black, prevent division by zero and flicker
  if (maxAmplitude < 1.0)
  {
    Studio studio(nLEDs);
    return studio.createSingleColorSingleFrameAnimation(Color());
  }

  // calculate tone amplifications, based on max amplitude and tone amplitude
  double totalAmpSquare = 0.0;
  std::map<std::string, double> toneAmpSquare;
  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    double amplification = it->second/maxAmplitude;
    double ampSquare = amplification*amplification;
    toneAmpSquare[it->first] = ampSquare;

    if (127 * ampSquare < 40)
    {
      continue;
    }

    totalAmpSquare += ampSquare;
  }

  // calculate number of leds per tone
  std::map<std::string, unsigned int> nLEDsPerToneMap;
  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    std::string tone = it->first;
    double ampSquare = toneAmpSquare[tone];

    int nLEDsPerTone = std::ceil(nLEDs / totalAmpSquare * ampSquare);

    if (127 * ampSquare < 40)
    {
      nLEDsPerTone = 0;
    }

    nLEDsPerToneMap[it->first] = nLEDsPerTone;
  }

  // create the animation
  int ledCounter = 0;
  for (auto it = tones.cbegin(); it != tones.cend(); ++it)
  {
    std::string tone = it->first;
    double ampSquare = toneAmpSquare[tone];

    Color color2;
    if (127 * ampSquare < 40)
    {
      continue;
    }
    else
    {
      Color color = m_toneColorMap[tone];
      color2 = Color(color.r * ampSquare, color.g * ampSquare, color.b * ampSquare);
    }

    int startLed = ledCounter;
    for (unsigned int i = startLed; i < startLed + nLEDsPerToneMap[tone]; ++i)
    {
      LED led(i, color2);
      frame.addLED(led);
      ledCounter++;
    }
  }

  animation.addFrame(frame);

  return animation;
}


Animation
ToneStudio::createToneAnimationCorners(unsigned int nLEDs, std::map<std::string, double> tones)
{
  if (m_toneColorMap.empty())
  {
    createToneColorMap(tones);
  }

  Animation animation;
  Frame frame(nLEDs);

  double maxAmplitude = 0.0;
  std::string maxTone;

  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > maxAmplitude)
    {
      maxTone = it->first;
      maxAmplitude = it->second;
    }
  }

  int toneCounter = 0;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    std::string tone = iter->first;
    double amplitude = iter->second;

    double amplification = amplitude/maxAmplitude;
    double ampSquare = amplification*amplification;
    Color color2;
    if (127 * ampSquare < 40)
    {
      color2 = Color();
    }
    else
    {
      Color color = m_toneColorMap[tone];
      color2 = Color(color.r * ampSquare, color.g * ampSquare, color.b * ampSquare);
    }

    for (unsigned int i = nLEDs/tones.size() * (toneCounter); i < nLEDs/tones.size() * (toneCounter+1); ++i)
    {
      LED led(i, color2);
      frame.addLED(led);
    }
    toneCounter++;
  }

  animation.addFrame(frame);

  return animation;
}


void
ToneStudio::createRandomToneColorMap(const std::map<std::string, double>& tones)
{
  std::map<std::string, Color> toneColorMap;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    std::string tone = iter->first;
    toneColorMap[tone] = Color::randomColor();
  }

  m_toneColorMap = toneColorMap;
}


void
ToneStudio::createToneColorMap(const std::map<std::string, double>& tones)
{
  std::map<std::string, Color> toneColorMap;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    Color color;
    int brightness = 127;
    std::string tone = iter->first;

    if (tone == "C")
    {
      color = Color(0, brightness, brightness);
    }
    else if (tone == "D")
    {
      color = Color(brightness, 0, 0);
    }
    else if (tone == "E")
    {
      color = Color(0, brightness, 0);
    }
    else if (tone == "F")
    {
      color = Color(0, 0, brightness);
    }
    else if (tone == "G")
    {
      color = Color(brightness, 34, 4);
    }
    else if (tone == "A")
    {
      color = Color(50, brightness, 0);
    }
    else if (tone == "B")
    {
      color = Color(brightness, 75, 0);
    }
    toneColorMap[tone] = color;
  }

  m_toneColorMap = toneColorMap;
}
