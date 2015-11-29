#include "toneanimationfactoryrange.h"

#include "studio/studio.h"

#include <QDebug>

#include <cassert>


ToneAnimationFactoryRange::ToneAnimationFactoryRange()
: ToneAnimationFactory()
{
}


Animation
ToneAnimationFactoryRange::createToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  const double minThreshold = 30;
  const double amplificationExponent = 2.0;

  Animation animation;
  Frame frame(nLEDs);

  // if low amplitude, make strip black, prevent division by zero and flicker
  if (toneData.maxToneAmplitude < 20.0)
  {
    Studio studio(nLEDs);
    return studio.createSingleColorSingleFrameAnimation(Color());
  }

  // calculate tone amplifications, based on max amplitude and tone amplitude
  double totalAmplification = 0.0;
  std::map<Tone, double> toneAmplification;
  for (const auto& toneAmplitude : toneData.currentTones)
  {
    double amplification = toneAmplitude.second/toneData.maxToneAmplitude;
    toneAmplification[toneAmplitude.first] = std::pow(amplification, amplificationExponent);

    if (127 * toneAmplification[toneAmplitude.first] < minThreshold)
    {
      continue;
    }

    totalAmplification += toneAmplification[toneAmplitude.first];
  }

  // calculate number of leds per tone
  std::map<Tone, unsigned int> nLEDsPerToneMap;
  for (const auto& toneAmplitude : toneData.currentTones)
  {
    Tone tone = toneAmplitude.first;
    double amplification = toneAmplification[tone];

    int nLEDsPerTone = std::ceil(nLEDs / totalAmplification * amplification);

    if (127 * amplification < minThreshold)
    {
      nLEDsPerTone = 0;
    }

    nLEDsPerToneMap[tone] = nLEDsPerTone/2;
  }

  double brightnessRelative = 1.0;
  if (toneData.toneMaxAverage > 20.00)
  {
    brightnessRelative = std::pow(toneData.maxToneAmplitude - toneData.toneMinAverage, 3) / std::pow(toneData.toneMaxAverage - toneData.toneMinAverage, 3) / 2.0 ;
//    std::cout << brightnessRelative << std::endl;
  }

  if (brightnessRelative > 1.0)
  {
    brightnessRelative = 1.0;
  }

  // create the animation
  Color color;
  int ledCounter = 0;
  for (const auto& toneAmplitude : toneData.currentTones)
  {
    Tone tone = toneAmplitude.first;
    double amplification = toneAmplification[tone];

    Color color2;
    if (127 * amplification < minThreshold)
    {
      continue;
    }
    else
    {
      color = getToneColorMap(toneData.currentTones).at(tone);
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
