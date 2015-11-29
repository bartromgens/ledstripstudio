#ifndef TONEINDIVIDUALANIMATIONFACTORY_H
#define TONEINDIVIDUALANIMATIONFACTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneIndividualAnimationFactory : public ToneAnimationFactory
{

public:

  ToneIndividualAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONEINDIVIDUALANIMATIONFACTORY_H
