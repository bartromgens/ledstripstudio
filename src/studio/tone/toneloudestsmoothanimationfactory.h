#ifndef TONELOUDESTSMOOTHANIMATIONFACTORY_H
#define TONELOUDESTSMOOTHANIMATIONFACTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneLoudestSmoothAnimationFactory : public ToneAnimationFactory
{

public:

  ToneLoudestSmoothAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONELOUDESTSMOOTHANIMATIONFACTORY_H
