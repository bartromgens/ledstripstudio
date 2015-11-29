#ifndef TONEANIMATIONFACTORYHISTORY_H
#define TONEANIMATIONFACTORYHISTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneAnimationFactoryHistory : public ToneAnimationFactory
{

public:

  ToneAnimationFactoryHistory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

  static Color getToneColor(Tone tone, double colorWheelOffset);

private:

  Frame m_toneHistoryFrame;

};

#endif // TONEANIMATIONFACTORYHISTORY_H
