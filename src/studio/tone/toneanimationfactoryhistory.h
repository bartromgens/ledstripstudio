#ifndef TONEANIMATIONFACTORYHISTORY_H
#define TONEANIMATIONFACTORYHISTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"

#include <deque>


class ToneAnimationFactoryHistory : public ToneAnimationFactory
{

public:

  ToneAnimationFactoryHistory();
  virtual ~ToneAnimationFactoryHistory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

  Animation createToneAnimationStaticHistory(unsigned int nLEDs, const ToneData& toneData);

private:

  static Color getToneColor(Tone tone, double colorWheelOffset = 2.8);

  Animation createToneAnimationDynamicHistory(unsigned int nLEDs, const ToneData& toneData);

  static double getNormalisedBrightness(double toneAmplitude, const ToneData& toneData);

private:

  Frame m_toneHistoryFrame;
  std::deque<std::pair<Tone, double>> m_toneHistory;

};

#endif // TONEANIMATIONFACTORYHISTORY_H
