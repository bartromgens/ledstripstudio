#ifndef TONEHISTORYANIMATIONFACTORY_H
#define TONEHISTORYANIMATIONFACTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"
#include "studio/tonestudio.h"


class ToneHistoryAnimationFactory : public ToneAnimationFactory
{

public:

  ToneHistoryAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

  static Color getToneColor(Tone tone, double colorWheelOffset);

private:

  Frame m_toneHistoryFrame;

};

#endif // TONEHISTORYANIMATIONFACTORY_H
