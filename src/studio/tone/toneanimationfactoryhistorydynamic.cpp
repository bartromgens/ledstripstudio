#include "toneanimationfactoryhistorydynamic.h"


ToneAnimationFactoryHistoryDynamic::ToneAnimationFactoryHistoryDynamic()
: ToneAnimationFactoryHistory()
{  
}


Animation
ToneAnimationFactoryHistoryDynamic::doCreateToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  Animation animation;
  Frame frame(nLEDs);

  unsigned int ledNr = 0;
  for (const auto& toneAmplitude : getToneHistory())
  {
    Tone tone = toneAmplitude.first;

    Color color = ToneAnimationFactoryHistory::getToneColor(tone);
    double brightness = ToneAnimationFactoryHistory::getNormalisedBrightness(toneData.currentTones.at(tone), toneData);
    brightness *= (nLEDs/2.0-ledNr)/static_cast<double>(nLEDs/2.0);  // division by 2 because of frame.mirror(). TODO BR: encapsulate this in the mirror functionality
    color.setBrightness(brightness);

    LED led(ledNr, color);
    frame.addLED(led);
    ledNr++;
  }

  frame.mirror();
  animation.addFrame(frame);
  return animation;
}

