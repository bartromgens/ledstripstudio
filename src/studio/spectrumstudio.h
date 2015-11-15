#ifndef SPECTRUMSTUDIO_H
#define SPECTRUMSTUDIO_H

#include <vector>

class Animation;
class ControlSettings;


class SpectrumStudio
{
public:
  SpectrumStudio();

  Animation createWaveformAnimationCentral(int nLEDs, const std::vector<std::pair<double, double>>& spectrum, ControlSettings& settings);

  void drawSpectrumInConsole(const std::vector<std::pair<double, double>>& spectrum, int minFreq, int maxFreq) const;
};

#endif // SPECTRUMSTUDIO_H
