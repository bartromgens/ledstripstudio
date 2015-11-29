#ifndef TONEANIMATIONFACTORYLOUDESTSMOOTH_H
#define TONEANIMATIONFACTORYLOUDESTSMOOTH_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneAnimationFactoryLoudestSmooth : public ToneAnimationFactory
{

public:

  ToneAnimationFactoryLoudestSmooth();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONEANIMATIONFACTORYLOUDESTSMOOTH_H
