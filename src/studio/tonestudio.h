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
    History,
    Individual,
    Corner
  };

public:
  ToneStudio();
  ~ToneStudio();

  void setAnimationType(ToneAnimationType type);
  ToneAnimationType getAnimationType() const;

  Animation createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones);

  static void writeToneToConsole(const std::map<std::string, double>& tones);

private:
  void createRandomToneColorMap(const std::map<std::string, double>& tones);
  void createToneColorMap(const std::map<std::string, double> &tones);

  static Animation createToneAnimationLoudest(unsigned int nLEDs, std::map<std::string, double> tones);
  static Animation createToneAnimationSmoothSum(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationHistory(unsigned int nLEDs, std::map<std::string, double> tones, unsigned int speed);
  Animation createToneAnimationIndividual(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationCorners(unsigned int nLEDs, std::map<std::string, double> tones);

private:
  ToneAnimationType m_animationType;

  Frame m_toneHistoryFrame;
  std::map<std::string, Color> m_toneColorMap;
};

#endif // TONESTUDIO_H
