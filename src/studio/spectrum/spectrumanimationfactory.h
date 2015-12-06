#ifndef SPECTRUMANIMATIONFACTORY_H
#define SPECTRUMANIMATIONFACTORY_H

#include "basic/animation.h"
#include "studio/tone/tonedata.h"

class ControlSettings;


class SpectrumAnimationFactory
{
public:
  SpectrumAnimationFactory();
  virtual ~SpectrumAnimationFactory();

  virtual Animation createSpectrumAnimation(unsigned int nLEDs,
                                            const std::vector<std::pair<double, double>>& spectrum,
                                            const ControlSettings& settings) = 0;

private:


};

#endif // SPECTRUMANIMATIONFACTORY_H
