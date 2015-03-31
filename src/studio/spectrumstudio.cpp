#include "spectrumstudio.h"

#include "basic/animation.h"
#include "settings/controlsettings.h"


SpectrumStudio::SpectrumStudio()
{
}


Animation
SpectrumStudio::createWaveformAnimationCentral(int nLEDs, const std::map<double, double>& spectrum, ControlSettings* settings)
{
  double brightnessRed = 0.0;
  double brightnessGreen = 0.0;
  double brightnessBlue = 0.0;

  double amplifyFactor = settings->volumeTotal/1000.0;
  double amplifyFactorRed = settings->volumeRed/25.0;
  double amplifyFactorGreen = settings->volumeGreen/50.0;
  double amplifyFactorBlue = settings->volumeBlue/100.0;

  for (std::map<double, double>::const_iterator iter = spectrum.begin();
       iter != spectrum.end(); ++iter)
  {
    double frequency = iter->first;
    double amplitude = iter->second;

    if (frequency > settings->freqRedMin && frequency < settings->freqRedMax)
    {
      brightnessRed += amplitude*amplifyFactor*amplifyFactorRed;
    }

    if (frequency > settings->freqGreenMin && frequency < settings->freqGreenMax)
    {
      brightnessGreen += amplitude*amplifyFactor*amplifyFactorGreen;
    }

    if (frequency > settings->freqBlueMin && frequency < settings->freqBlueMax)
    {
      brightnessBlue += amplitude*amplifyFactor*amplifyFactorBlue;
    }
  }

  Animation animation;
  Frame frame(nLEDs);

  int centreLedNr = nLEDs/2;

//  int maxRGB = std::max(brightnessBlue, std::max(brightnessRed, brightnessGreen));
//  brightnessRed *= 127.0/maxRGB;
//  brightnessGreen *= 127.0/maxRGB;
//  brightnessBlue *= 127.0/maxRGB;

  for (int i = 0; i < centreLedNr; ++i)
  {
    int scaleFact = (i * 127) / centreLedNr;
//    int scaleFact = 0.0;

    int r = brightnessRed - scaleFact;
    int g = brightnessGreen - scaleFact;
    int b = brightnessBlue - scaleFact;

    if (r > 127)
    {
      r = 127;
    }
    if (g > 127)
    {
      g = 127;
    }
    if (b > 127)
    {
      b = 127;
    }

    if (r < 0)
    {
      r = 0;
    }
    if (g < 0)
    {
      g = 0;
    }
    if (b < 0)
    {
      b = 0;
    }

    int offSet = 0;
    int ledNr1 = (centreLedNr + i + 1 + offSet) % nLEDs;
    int ledNr2 = (centreLedNr - i + offSet) % nLEDs;

    LED led(ledNr1, Color(r, g, b));
    frame.addLED(led);

    LED led2(ledNr2, Color(r, g, b));
    frame.addLED(led2);
  }

  animation.addFrame(frame);
  return animation;
}


void
SpectrumStudio::drawSpectrumInConsole(const std::map<double, double>& spectrum, int minFreq, int maxFreq) const
{
  for (auto iter = spectrum.begin(); iter != spectrum.end(); ++iter)
  {
    int frequency = static_cast<int>(iter->first);
    if (frequency > minFreq && frequency < maxFreq)
    {
      std::cout << frequency << " :";
      for (std::size_t j = 0; j < iter->second/4; ++j)
      {
        std::cout << ".";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}
