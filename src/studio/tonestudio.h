#ifndef TONESTUDIO_H
#define TONESTUDIO_H

#include "basic/animation.h"

class ToneStudio
{
public:
  ToneStudio();
  ~ToneStudio();

  Animation createToneAnimationLoudest(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationSmoothSum(unsigned int nLEDs, std::map<std::string, double> tones);

  void writeToneToConsole(const std::map<std::string, double>& tones);
};

#endif // TONESTUDIO_H
