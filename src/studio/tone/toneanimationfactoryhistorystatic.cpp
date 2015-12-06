#include "toneanimationfactoryhistorystatic.h"


ToneAnimationFactoryHistoryStatic::ToneAnimationFactoryHistoryStatic()
: ToneAnimationFactoryHistory(),
  m_toneHistoryFrame(0)
{
}


Animation
ToneAnimationFactoryHistoryStatic::doCreateToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  Animation animation;
  Color color = ToneAnimationFactoryHistory::getToneColor(toneData.maxTone);
  double brightness = ToneAnimationFactoryHistory::getNormalisedBrightness(toneData.maxToneAmplitude, toneData);
  color.setBrightness(brightness);

  Frame frame(nLEDs);

  LED led(0, color);
  frame.addLED(led);

  std::vector<LED> leds = m_toneHistoryFrame.getLEDs();
  for (auto& led : leds)
  {
    led.setLEDnr(led.getLEDnr() + 1);
    frame.addLED(led);
  }

  frame.mirror();
  animation.addFrame(frame);
  m_toneHistoryFrame = frame;
  return animation;
}
