#ifndef SPECTRUMANIMATIONFACTORYRGBBANDS_H
#define SPECTRUMANIMATIONFACTORYRGBBANDS_H

#include "spectrumanimationfactory.h"

class ControlSettings;


class SpectrumAnimationFactoryRGBBands : public SpectrumAnimationFactory
{
public:
  SpectrumAnimationFactoryRGBBands();
  virtual ~SpectrumAnimationFactoryRGBBands();

  virtual Animation createSpectrumAnimation(unsigned int nLEDs,
                                            const std::vector<std::pair<double, double>>& spectrum,
                                            const ControlSettings& settings) const;

};

#endif // SPECTRUMANIMATIONFACTORYRGBBANDS_H
