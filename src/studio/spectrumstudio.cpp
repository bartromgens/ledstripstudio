#include "spectrumstudio.h"

#include "basic/animation.h"

SpectrumStudio::SpectrumStudio()
{
}


Animation
SpectrumStudio::createWaveformAnimationCentral(int nLEDs, int brightnessRed, int brightnessGreen, int brightnessBlue)
{
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

    if (r > 127.0)
    {
      r = 127.0;
    }
    if (g > 127.0)
    {
      g = 127.0;
    }
    if (b > 127.0)
    {
      b = 127.0;
    }

    if (r < 0.0)
    {
      r = 0.0;
    }
    if (g < 0.0)
    {
      g = 0.0;
    }
    if (b < 0.0)
    {
      b = 0.0;
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
  for (std::map<double, double>::const_iterator iter = spectrum.begin();
       iter != spectrum.end(); ++iter)
  {
    int frequency = iter->first;
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
