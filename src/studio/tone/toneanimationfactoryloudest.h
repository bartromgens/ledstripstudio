#ifndef TONEANIMATIONFACTORYLOUDEST_H
#define TONEANIMATIONFACTORYLOUDEST_H

#include "toneanimationfactory.h"

#include "basic/animation.h"
#include "basic/tone.h"


class ToneAnimationFactoryLoudest : public ToneAnimationFactory
{

public:

  ToneAnimationFactoryLoudest();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

};

#endif // TONEANIMATIONFACTORYLOUDEST_H
