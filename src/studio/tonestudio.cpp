#include "tonestudio.h"

#include "spectrum/toneanalyser.h"
#include "studio.h"
#include "studio/tone/tonehistoryanimationfactory.h"

#include <QDebug>

#include <cassert>
#include <cmath>
#include <string>


unsigned int ToneStudio::m_historySize = 30;


ToneStudio::ToneStudio()
: m_toneAnimationFactory(new ToneHistoryAnimationFactory()),
  m_toneHistoryFrame(0),
  m_toneColorMap(),
  m_toneData()
{
}


ToneStudio::~ToneStudio()
{
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


void
ToneStudio::calcMaxAndMinTone(const std::map<Tone, double>& tones)
{
  m_toneData.maxToneAmplitude = std::numeric_limits<double>::min();
  m_toneData.minToneAmplitude = std::numeric_limits<double>::max();

  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > m_toneData.maxToneAmplitude)
    {
      m_toneData.maxTone = it->first;
      m_toneData.maxToneAmplitude = it->second;
    }

    if (it->second < m_toneData.minToneAmplitude)
    {
      m_toneData.minToneAmplitude = it->second;
    }
  }
}


void
ToneStudio::calcToneMaxAverage()
{
  m_toneData.maxToneHistory.push_back(m_toneData.maxToneAmplitude);
  m_toneData.toneMaxAverage += m_toneData.maxToneHistory.back()/m_historySize;

  if (m_toneData.maxToneHistory.size() > m_historySize)
  {
    m_toneData.toneMaxAverage -= m_toneData.maxToneHistory.front()/m_historySize;
    m_toneData.maxToneHistory.pop_front();
  }
}


void
ToneStudio::calcToneMinAverage()
{
  m_toneData.minToneHistory.push_back(m_toneData.minToneAmplitude);
  m_toneData.toneMinAverage += m_toneData.minToneHistory.back()/m_historySize;

  if (m_toneData.minToneHistory.size() > m_historySize)
  {
    m_toneData.toneMinAverage -= m_toneData.minToneHistory.front()/m_historySize;
    m_toneData.minToneHistory.pop_front();
  }
}


Animation
ToneStudio::createToneAnimation(unsigned int nLEDs,
                                const std::map<Tone, double>& tones,
                                ToneStudio::AnimationType animationType)
{
  m_toneData.maxToneAmplitude = 0.0;

  calcMaxAndMinTone(tones);

  calcToneMaxAverage();
  calcToneMinAverage();

  switch (animationType)
  {
    case ToneStudio::AnimationType::Loudest:
    {
      return createToneAnimationLoudest(nLEDs);
    }
    case ToneStudio::AnimationType::SmoothSum:
    {
      return createToneAnimationSmoothSum(nLEDs, tones);
    }
    case ToneStudio::AnimationType::History:
    {
      return createToneAnimationHistory(nLEDs);
    }
    case ToneStudio::AnimationType::Individual:
    {
      return createToneAnimationIndividual(nLEDs, tones);
    }
    case ToneStudio::AnimationType::Corner:
    {
      return createToneAnimationCorners(nLEDs, tones);
    }
    case ToneStudio::AnimationType::None:
    {
      return Animation();
    }
  }

  return Animation();
}


Animation
ToneStudio::createToneAnimationLoudest(unsigned int nLEDs)
{
  Animation animation;

  Frame frame(nLEDs);

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
    Color color;

    if (m_toneData.maxTone == Tone::C)
    {
      color = Color(127, 0, 0);
    }
    else if (m_toneData.maxTone == Tone::D)
    {
      color = Color(127, 127, 0);
    }
    else if (m_toneData.maxTone == Tone::E)
    {
      color = Color(0, 127, 0);
    }
    else if (m_toneData.maxTone == Tone::F)
    {
      color = Color(0, 127, 127);
    }
    else if (m_toneData.maxTone == Tone::G)
    {
      color = Color(0, 0, 127);
    }
    else if (m_toneData.maxTone == Tone::A)
    {
      color = Color(127, 0, 127);
    }
    else if (m_toneData.maxTone == Tone::B)
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
ToneStudio::createToneAnimationSmoothSum(unsigned int nLEDs, const std::map<Tone, double>& tones)
{
  Animation animation;

  Frame frame(nLEDs);

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    for (std::map<Tone, double>::const_iterator iter = tones.begin(); iter != tones.end(); ++iter)
    {
      if (iter->first == Tone::C)
      {
        r += iter->second;
      }
      else if (iter->first == Tone::D)
      {
        r += iter->second/2.0;
//        g += iter->second/2.0;
      }
      else if (iter->first == Tone::E)
      {
        g += iter->second;
      }
      else if (iter->first == Tone::F)
      {
        g += iter->second/2.0;
//        b += iter->second/2.0;
      }
      else if (iter->first == Tone::G)
      {
        b += iter->second;
      }
      else if (iter->first == Tone::A)
      {
        b += iter->second/2.0;
//        r += iter->second/2.0;
      }
      else if (iter->first == Tone::B)
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
ToneStudio::createToneAnimationHistory(unsigned int nLEDs)
{
  return m_toneAnimationFactory->createToneAnimation(nLEDs, m_toneData);
}


Animation
ToneStudio::createToneAnimationIndividual(unsigned int nLEDs, const std::map<Tone, double>& tones)
{
  const double minThreshold = 30;
  const double amplificationExponent = 2.0;

  if (m_toneColorMap.empty())
  {
    createToneColorMap(tones);
  }

  Animation animation;
  Frame frame(nLEDs);

  // if low amplitude, make strip black, prevent division by zero and flicker
  if (m_toneData.maxToneAmplitude < 20.0)
  {
    Studio studio(nLEDs);
    return studio.createSingleColorSingleFrameAnimation(Color());
  }

  // calculate tone amplifications, based on max amplitude and tone amplitude
  double totalAmplification = 0.0;
  std::map<Tone, double> toneAmplification;
  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    double amplification = it->second/m_toneData.maxToneAmplitude;
    toneAmplification[it->first] = std::pow(amplification, amplificationExponent);

    if (127 * toneAmplification[it->first] < minThreshold)
    {
      continue;
    }

    totalAmplification += toneAmplification[it->first];
  }

  // calculate number of leds per tone
  std::map<Tone, unsigned int> nLEDsPerToneMap;
  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    Tone tone = it->first;
    double amplification = toneAmplification[tone];

    int nLEDsPerTone = std::ceil(nLEDs / totalAmplification * amplification);

    if (127 * amplification < minThreshold)
    {
      nLEDsPerTone = 0;
    }

    nLEDsPerToneMap[it->first] = nLEDsPerTone/2;
  }

  double brightnessRelative = 1.0;
  if (m_toneData.toneMaxAverage > 20.00)
  {
    brightnessRelative = std::pow(m_toneData.maxToneAmplitude - m_toneData.toneMinAverage, 3) / std::pow(m_toneData.toneMaxAverage - m_toneData.toneMinAverage, 3) / 2.0 ;
//    std::cout << brightnessRelative << std::endl;
  }

  if (brightnessRelative > 1.0)
  {
    brightnessRelative = 1.0;
  }

  // create the animation
  Color color;
  int ledCounter = 0;
  for (auto it = tones.cbegin(); it != tones.cend(); ++it)
  {
    Tone tone = it->first;
    double amplification = toneAmplification[tone];

    Color color2;
    if (127 * amplification < minThreshold)
    {
      continue;
    }
    else
    {
      color = m_toneColorMap[tone];
      color2 = Color(color.r * brightnessRelative, color.g * brightnessRelative, color.b * brightnessRelative);
    }

    int startLed = ledCounter;
    for (unsigned int i = startLed; i < startLed + nLEDsPerToneMap[tone]; ++i)
    {
      if (i < nLEDs/2)
      {
        LED led(i, color2);
        LED led2(nLEDs-i, color2);
        frame.addLED(led);
        frame.addLED(led2);
        ledCounter++;
      }
    }
  }

  animation.addFrame(frame);

  return animation;
}


Animation
ToneStudio::createToneAnimationCorners(unsigned int nLEDs, const std::map<Tone, double>& tones)
{
  if (m_toneColorMap.empty())
  {
    createToneColorMap(tones);
  }

  Animation animation;
  Frame frame(nLEDs);

  int toneCounter = 0;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    Tone tone = iter->first;
    double amplitude = iter->second;

    double ampRatio = amplitude/m_toneData.maxToneAmplitude;
    double amplification = ampRatio*ampRatio;
    Color color2;
    if (127 * amplification < 40)
    {
      color2 = Color();
    }
    else
    {
      Color color = m_toneColorMap[tone];
      color2 = Color(color.r * amplification, color.g * amplification, color.b * amplification);
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
ToneStudio::createRandomToneColorMap(const std::map<Tone, double>& tones)
{
  std::map<Tone, Color> toneColorMap;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    Tone tone = iter->first;
    toneColorMap[tone] = Color::randomColor();
  }

  m_toneColorMap = toneColorMap;
}


void
ToneStudio::createToneColorMap(const std::map<Tone, double>& tones)
{
  std::map<Tone, Color> toneColorMap;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    Color color;
    int brightness = 127;
    Tone tone = iter->first;

    if (tone == Tone::C)
    {
      color = Color(0, brightness, brightness);
    }
    else if (tone == Tone::D)
    {
      color = Color(brightness, 0, 0);
    }
    else if (tone == Tone::E)
    {
      color = Color(0, brightness, 0);
    }
    else if (tone == Tone::F)
    {
      color = Color(0, 0, brightness);
    }
    else if (tone == Tone::G)
    {
      color = Color(brightness, 34, 4);
    }
    else if (tone == Tone::A)
    {
      color = Color(50, brightness, 0);
    }
    else if (tone == Tone::B)
    {
      color = Color(brightness, 75, 0);
    }
    toneColorMap[tone] = color;
  }

  m_toneColorMap = toneColorMap;
}


Color
ToneStudio::getToneColor(Tone tone, double colorWheelOffset)
{
  unsigned int i = 0;
  switch (tone)
  {
    case Tone::C :
      i = 0;
      break;
    case Tone::D :
      i = 1;
      break;
    case Tone::E :
      i = 2;
      break;
    case Tone::F :
      i = 3;
      break;
    case Tone::G :
      i = 4;
      break;
    case Tone::A :
      i = 5;
      break;
    case Tone::B :
      i = 6;
      break;
  }

  unsigned int colorInt = static_cast<int>( 3.0*127.0/7.0* (i + colorWheelOffset) ) % (3*128);
  return Studio::wheel(colorInt);
}
