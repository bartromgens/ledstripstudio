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
    SmoothSum,
    History
  };
public:
  ToneStudio();
  ~ToneStudio();

  void setAnimationType(ToneAnimationType type);

  Animation createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones);

  static void writeToneToConsole(const std::map<std::string, double>& tones);

private:
  static Animation createToneAnimationLoudest(unsigned int nLEDs, std::map<std::string, double> tones);
  static Animation createToneAnimationSmoothSum(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationHistory(unsigned int nLEDs, std::map<std::string, double> tones, unsigned int speed);

private:
  ToneAnimationType m_animationType;

  Frame m_toneHistoryFrame;
};

#endif // TONESTUDIO_H
