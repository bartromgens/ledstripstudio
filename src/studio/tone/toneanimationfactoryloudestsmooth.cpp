#include "toneanimationfactoryloudestsmooth.h"

#include "studio/studio.h"

#include <QDebug>

#include <cassert>


ToneAnimationFactoryLoudestSmooth::ToneAnimationFactoryLoudestSmooth()
: ToneAnimationFactory()
{
}


Animation
ToneAnimationFactoryLoudestSmooth::createToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  Animation animation;
  Frame frame(nLEDs);

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    for (const auto& toneAmplitude : toneData.currentTones)
    {
      if (toneAmplitude.first == Tone::C)
      {
        r += toneAmplitude.second;
      }
      else if (toneAmplitude.first == Tone::D)
      {
        r += toneAmplitude.second/2.0;
      }
      else if (toneAmplitude.first == Tone::E)
      {
        g += toneAmplitude.second;
      }
      else if (toneAmplitude.first == Tone::F)
      {
        g += toneAmplitude.second/2.0;
      }
      else if (toneAmplitude.first == Tone::G)
      {
        b += toneAmplitude.second;
      }
      else if (toneAmplitude.first == Tone::A)
      {
        b += toneAmplitude.second/2.0;
      }
      else if (toneAmplitude.first == Tone::B)
      {
        r += toneAmplitude.second;
      }
    }

    double norm = std::sqrt(r*r+g*g+b*b);
    int rNorm = static_cast<int>(r/norm*255);
    int gNorm = static_cast<int>(g/norm*255);
    int bNorm = static_cast<int>(b/norm*255);

    Color color(rNorm, gNorm, bNorm);

    LED led(i, color);
    frame.addLED(led);
  }

  animation.addFrame(frame);

  return animation;
}
