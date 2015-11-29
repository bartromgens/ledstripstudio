#ifndef TONERANGEANIMATIONFACTORY_H
#define TONERANGEANIMATIONFACTORY_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneRangeAnimationFactory : public ToneAnimationFactory
{

public:

  ToneRangeAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONERANGEANIMATIONFACTORY_H
