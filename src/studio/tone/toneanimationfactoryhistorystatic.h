#ifndef TONEANIMATIONFACTORYHISTORYSTATIC_H
#define TONEANIMATIONFACTORYHISTORYSTATIC_H

#include "toneanimationfactoryhistory.h"

class ToneAnimationFactoryHistoryStatic : public ToneAnimationFactoryHistory
{
public:

  ToneAnimationFactoryHistoryStatic();

private:

  virtual Animation doCreateToneAnimation(unsigned int nLEDs, const ToneData& toneData);

private:

  Frame m_toneHistoryFrame;

};

#endif // TONEANIMATIONFACTORYHISTORYSTATIC_H
