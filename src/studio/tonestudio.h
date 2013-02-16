#ifndef TONESTUDIO_H
#define TONESTUDIO_H

#include "basic/animation.h"

class ToneStudio
{
public:
  ToneStudio();
  ~ToneStudio();

  Animation createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones);
};

#endif // TONESTUDIO_H
