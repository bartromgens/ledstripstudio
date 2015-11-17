#ifndef TONESTUDIO_H
#define TONESTUDIO_H

#include "basic/animation.h"
#include "basic/tone.h"

class ToneStudio
{
public:
  enum class AnimationType
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

  Animation createToneAnimation(unsigned int nLEDs,
                                const std::map<Tone, double>& tones,
                                ToneStudio::AnimationType animationType,
                                double colorWheelOffset = 2.8);

  static void writeToneToConsole(const std::map<std::string, double>& tones);

  void calcMaxAndMinTone(const std::map<Tone, double>& tones);
  void calcToneMaxAverage();
  void calcToneMinAverage();

private:
  void createRandomToneColorMap(const std::map<Tone, double>& tones);
  void createToneColorMap(const std::map<Tone, double>& tones);

  Animation createToneAnimationLoudest(unsigned int nLEDs);
  Animation createToneAnimationSmoothSum(unsigned int nLEDs, const std::map<Tone, double>& tones);
  Animation createToneAnimationHistory(unsigned int nLEDs, unsigned int speed, double colorWheelOffset);
  Animation createToneAnimationIndividual(unsigned int nLEDs, const std::map<Tone, double>& tones);
  Animation createToneAnimationCorners(unsigned int nLEDs, const std::map<Tone, double>& tones);

private:

  Frame m_toneHistoryFrame;
  std::map<Tone, Color> m_toneColorMap;

  std::deque<double> m_maxToneHistory;
  std::deque<double> m_minToneHistory;
  double m_toneMaxAverage;
  double m_toneMinAverage;

  Tone m_maxTone;
  double m_maxToneAmplitude;
  double m_minToneAmplitude;

  static unsigned int m_historySize;
};

#endif // TONESTUDIO_H
