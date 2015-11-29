#ifndef TONESTUDIO_H
#define TONESTUDIO_H

#include "basic/animation.h"
#include "basic/tone.h"
#include "studio/tone/tonedata.h"

#include <memory>

class ToneAnimationFactory;


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
    Range
  };

public:

  ToneStudio();
  ~ToneStudio();

  Animation createToneAnimation(unsigned int nLEDs, const std::map<Tone, double>& tones);
  void setToneAnimationType(ToneStudio::AnimationType animationType);

  static void writeToneToConsole(const std::map<std::string, double>& tones);

private:

  void calcToneMaxAverage();
  void calcToneMinAverage();
  void calcMaxAndMinTone(const std::map<Tone, double>& tones);
  static Color getToneColor(Tone tone, double colorWheelOffset);

private:

  std::unique_ptr<ToneAnimationFactory> m_toneAnimationFactory;
  Frame m_toneHistoryFrame;
  ToneData m_toneData;

  static unsigned int m_historySize;
};

#endif // TONESTUDIO_H
