#ifndef TONEANIMATIONFACTORYINDIVIDUAL_H
#define TONEANIMATIONFACTORYINDIVIDUAL_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneAnimationFactoryIndividual : public ToneAnimationFactory
{

public:

  ToneAnimationFactoryIndividual();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONEANIMATIONFACTORYINDIVIDUAL_H
