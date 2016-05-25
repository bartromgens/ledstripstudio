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

protected:

  const std::deque<std::pair<Tone, double>>& getToneHistory() const;

  static Color getToneColor(Tone tone, double colorWheelOffset = 2.8);
  static double getNormalisedBrightness(double toneAmplitude, const ToneData& toneData);

private:

  virtual Animation doCreateToneAnimation(unsigned int nLEDs, const ToneData& toneData) = 0;

private:

  std::deque<std::pair<Tone, double>> m_toneHistory;
  std::size_t m_sizeToneHistory;

};

#endif // TONEANIMATIONFACTORYHISTORY_H
