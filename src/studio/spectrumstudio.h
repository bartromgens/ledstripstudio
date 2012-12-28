#ifndef SPECTRUMSTUDIO_H
#define SPECTRUMSTUDIO_H

#include <map>

class Animation;

class SpectrumStudio
{
public:
  SpectrumStudio();

  Animation createWaveformAnimationCentral(int nLEDs, int brightnessRed, int brightnessGreen, int brightnessBlue);
};

#endif // SPECTRUMSTUDIO_H