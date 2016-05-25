#include "toneanimationfactoryhistorydynamic.h"


ToneAnimationFactoryHistoryDynamic::ToneAnimationFactoryHistoryDynamic()
: ToneAnimationFactoryHistory(),
  m_speed(4)
{
}


Animation
ToneAnimationFactoryHistoryDynamic::doCreateToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  Animation animation;
  Frame frame(nLEDs);

  unsigned int ledNr = 0;
  const std::deque<std::pair<Tone, double> >& toneHistory = getToneHistory();
  for (std::size_t i = 0; i < toneHistory.size(); ++i)
  {
    int ledPos = ledNr/m_speed;
    if (ledPos >= nLEDs)  //TODO: remove break and just stop loop naturally
    {
      break;
    }

    Color color;
    for (std::size_t j = 0; j < m_speed; ++j)
    {
      if (i + j < toneHistory.size())
      {
        Color toneColor = ToneAnimationFactoryHistory::getToneColor(toneHistory[i+j].first);
        color.r += toneColor.r;
        color.g += toneColor.g;
        color.b += toneColor.b;
      }
    }
    color.r /= m_speed;
    color.g /= m_speed;
    color.b /= m_speed;
    double brightness = ToneAnimationFactoryHistory::getNormalisedBrightness(toneData.currentTones.at(toneHistory[i].first), toneData);
    brightness *= (nLEDs/1.2-ledPos) / static_cast<double>(nLEDs/2.0);  // division by 2 because of frame.mirror(). TODO BR: encapsulate this in the mirror functionality
    color.setBrightness(brightness);

    LED led(ledPos, color);
    frame.addLED(led);
    ledNr++;
  }

  frame.mirror();
  animation.addFrame(frame);
  return animation;
}

