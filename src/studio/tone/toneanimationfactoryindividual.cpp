#include "toneanimationfactoryindividual.h"

#include "studio/studio.h"

#include <QDebug>

#include <cassert>


ToneAnimationFactoryIndividual::ToneAnimationFactoryIndividual()
: ToneAnimationFactory()
{
}


Animation
ToneAnimationFactoryIndividual::createToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  Animation animation;
  Frame frame(nLEDs);

  int toneCounter = 0;
  for (const auto& toneAmplitude : toneData.currentTones)
  {
    Tone tone = toneAmplitude.first;
    double amplitude = toneAmplitude.second;

    double ampRatio = amplitude/toneData.maxToneAmplitude;
    double amplification = ampRatio*ampRatio;
    Color color2;
    if (127 * amplification < 40)
    {
      color2 = Color();
    }
    else
    {
      Color color = getToneColorMap(toneData.currentTones).at(tone);
      color2 = Color(color.r * amplification, color.g * amplification, color.b * amplification);
    }

    for (unsigned int i = nLEDs/toneData.currentTones.size() * (toneCounter); i < nLEDs/toneData.currentTones.size() * (toneCounter+1); ++i)
    {
      LED led(i, color2);
      frame.addLED(led);
    }
    toneCounter++;
  }

  animation.addFrame(frame);

  return animation;
}
