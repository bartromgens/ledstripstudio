#ifndef TONEANIMATIONFACTORYRANGE_H
#define TONEANIMATIONFACTORYRANGE_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneAnimationFactoryRange : public ToneAnimationFactory
{

public:

  ToneAnimationFactoryRange();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONEANIMATIONFACTORYRANGE_H
