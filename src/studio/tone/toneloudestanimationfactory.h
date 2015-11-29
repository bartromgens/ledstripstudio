#ifndef TONELOUDESTANIMATIONFACTORY_H
#define TONELOUDESTANIMATIONFACTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneLoudestAnimationFactory : public ToneAnimationFactory
{

public:

  ToneLoudestAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONELOUDESTANIMATIONFACTORY_H
