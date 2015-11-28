#ifndef TONEANIMATIONFACTORY_H
#define TONEANIMATIONFACTORY_H

#include "basic/animation.h"
#include "studio/tone/tonedata.h"


class ToneAnimationFactory
{
public:
  ToneAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData) = 0;
};

#endif // TONEANIMATIONFACTORY_H
