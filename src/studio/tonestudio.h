#ifndef TONESTUDIO_H
#define TONESTUDIO_H

#include "basic/animation.h"

class ToneStudio
{
public:
  enum ToneAnimationType
  {
    None,
    Loudest,
    SmoothSum
  };
public:
  ToneStudio();
  ~ToneStudio();

  void setAnimationType(ToneAnimationType type);

  Animation createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones);

  void writeToneToConsole(const std::map<std::string, double>& tones);

private:
  Animation createToneAnimationLoudest(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationSmoothSum(unsigned int nLEDs, std::map<std::string, double> tones);

private:
  ToneAnimationType m_animationType;
};

#endif // TONESTUDIO_H
