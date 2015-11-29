#include "toneanimationfactoryhistory.h"

#include "studio/studio.h"

#include <QDebug>

#include <cassert>


ToneAnimationFactoryHistory::ToneAnimationFactoryHistory()
: ToneAnimationFactory(),
  m_toneHistoryFrame(0)
{
}


Animation
ToneAnimationFactoryHistory::createToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  const unsigned int speed = 1;
  const double colorWheelOffset = 2.8;

  Animation animation;

  double brightnessRelative = 0.0;
  if (toneData.toneMaxAverage > 0.01)
  {
    brightnessRelative = std::log(toneData.maxToneAmplitude/ ((toneData.toneMaxAverage + toneData.toneMinAverage)/1.5)) * 0.5;  // see also https://en.wikipedia.org/wiki/Weber-Fechner_law
    if (brightnessRelative < 0.0)
    {
      brightnessRelative = 0.0;
    }
    assert(brightnessRelative >= 0.0);
  }
  unsigned int brightness = std::min(static_cast<int>(127 * brightnessRelative), 127);

  Color color = ToneAnimationFactoryHistory::getToneColor(toneData.maxTone, colorWheelOffset);

  color.r = color.r * brightness/127.0;
  color.g = color.g * brightness/127.0;
  color.b = color.b * brightness/127.0;

  Frame frame(nLEDs);
  std::vector<LED> leds = m_toneHistoryFrame.getLEDs();

  for (std::size_t i = 0 ; i < leds.size()/2; ++i)
  {
    unsigned int mirrorI = leds.size()-i-1;
    if (mirrorI > i + speed)
    {
      leds[i].setLEDnr(leds[i].getLEDnr() + speed);
      leds[mirrorI].setLEDnr(leds[mirrorI].getLEDnr() - speed);

      frame.addLED(leds[i]);
      frame.addLED(leds[mirrorI]);
    }
  }

  for (unsigned int i = 0; i < speed; ++i)
  {
    int mirrorI = nLEDs-i-1;

    LED led(i, color);
    frame.addLED(led);
    LED led2(mirrorI, color);
    frame.addLED(led2);
  }

  animation.addFrame(frame);
  m_toneHistoryFrame = frame;

  return animation;
}


Color
ToneAnimationFactoryHistory::getToneColor(Tone tone, double colorWheelOffset)
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
