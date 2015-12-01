#ifndef TONEANIMATIONFactoryHISTORYDYNAMIC_H
#define TONEANIMATIONFactoryHISTORYDYNAMIC_H

#include "toneanimationfactoryhistory.h"


class ToneAnimationFactoryHistoryDynamic : public ToneAnimationFactoryHistory
{
public:

  ToneAnimationFactoryHistoryDynamic();

private:

  virtual Animation doCreateToneAnimation(unsigned int nLEDs, const ToneData& toneData);

};

#endif // TONEANIMATIONFactoryHISTORYDYNAMIC_H
