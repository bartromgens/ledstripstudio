#ifndef SPECTRUMSTUDIO_H
#define SPECTRUMSTUDIO_H

#include <map>

class Animation;
class ControlSettings;

class SpectrumStudio
{
public:
  SpectrumStudio();

  Animation createWaveformAnimationCentral(int nLEDs, const std::map<double, double>& spectrum, ControlSettings* settings);

  void drawSpectrumInConsole(const std::map<double, double>& spectrum, int minFreq, int maxFreq) const;
};

#endif // SPECTRUMSTUDIO_H
