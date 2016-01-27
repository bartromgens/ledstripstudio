#include "toneanimationfactoryloudest.h"

#include "studio/studio.h"

#include <QDebug>

#include <cassert>


ToneAnimationFactoryLoudest::ToneAnimationFactoryLoudest()
: ToneAnimationFactory()
{
}


Animation
ToneAnimationFactoryLoudest::createToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  Animation animation;

  Frame frame(nLEDs);

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
    Color color;

    switch (toneData.maxTone)
    {
      case Tone::C :
        color = Color(255, 0, 0);
        break;
      case Tone::D :
        color = Color(255, 255, 0);
        break;
      case Tone::E :
        color = Color(0, 255, 0);
        break;
      case Tone::F :
        color = Color(0, 255, 255);
        break;
      case Tone::G :
        color = Color(0, 0, 255);
        break;
      case Tone::A :
        color = Color(255, 0, 255);
        break;
      case Tone::B :
        color = Color(255, 50, 50);
        break;
    }

    LED led(i, color);
    frame.addLED(led);
  }

  animation.addFrame(frame);

  return animation;
}
