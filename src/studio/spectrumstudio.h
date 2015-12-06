#ifndef SPECTRUMSTUDIO_H
#define SPECTRUMSTUDIO_H

#include <memory>
#include <vector>

class Animation;
class ControlSettings;
class SpectrumAnimationFactory;


class SpectrumStudio
{
public:

  SpectrumStudio();
  ~SpectrumStudio();

  Animation createWaveformAnimationCentral(unsigned int nLEDs,
                                           const std::vector<std::pair<double, double>>& spectrum,
                                           const ControlSettings& settings);

  void drawSpectrumInConsole(const std::vector<std::pair<double, double>>& spectrum,
                             int minFreq,
                             int maxFreq) const;

private:

  std::unique_ptr<SpectrumAnimationFactory> m_spectrumAnimationFactory;

};

#endif // SPECTRUMSTUDIO_H
