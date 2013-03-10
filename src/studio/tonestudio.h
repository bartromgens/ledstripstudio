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

  void calcMaxTone(const std::map<std::string, double>& tones);
  void calcToneMaxAverage();
private:
  void createRandomToneColorMap(const std::map<std::string, double>& tones);
  void createToneColorMap(const std::map<std::string, double> &tones);

  Animation createToneAnimationLoudest(unsigned int nLEDs);
  Animation createToneAnimationSmoothSum(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationHistory(unsigned int nLEDs, unsigned int speed);
  Animation createToneAnimationIndividual(unsigned int nLEDs, std::map<std::string, double> tones);
  Animation createToneAnimationCorners(unsigned int nLEDs, std::map<std::string, double> tones);

private:
  ToneAnimationType m_animationType;

  Frame m_toneHistoryFrame;
  std::map<std::string, Color> m_toneColorMap;

  std::deque<double> m_maxToneHistory;
  double m_toneMaxAverage;

  std::string m_maxTone;
  double m_maxToneAmplitude;
};

#endif // TONESTUDIO_H
