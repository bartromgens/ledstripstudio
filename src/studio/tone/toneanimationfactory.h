#ifndef TONEANIMATIONFACTORY_H
#define TONEANIMATIONFACTORY_H

#include "basic/animation.h"
#include "studio/tone/tonedata.h"


class ToneAnimationFactory
{
public:
  ToneAnimationFactory();

  virtual Animation createToneAnimation(unsigned int nLEDs, const ToneData& toneData) = 0;

protected:

  const std::map<Tone, Color>& getToneColorMap(const std::map<Tone, double>& tones);

private:

  void createRandomToneColorMap(const std::map<Tone, double>& tones);
  void createToneColorMap(const std::map<Tone, double>& tones);

private:

  std::map<Tone, Color> m_toneColorMap;

};

#endif // TONEANIMATIONFACTORY_H
