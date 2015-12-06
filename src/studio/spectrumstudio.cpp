#include "spectrumstudio.h"

#include "basic/animation.h"
#include "settings/controlsettings.h"
#include "spectrum/spectrumanimationfactory.h"
#include "spectrum/spectrumanimationfactoryrgbbands.h"


SpectrumStudio::SpectrumStudio()
: m_spectrumAnimationFactory(new SpectrumAnimationFactoryRGBBands())
{
}


SpectrumStudio::~SpectrumStudio()
{
}


Animation
SpectrumStudio::createAnimation(unsigned int nLEDs,
                                const std::vector<std::pair<double, double>>& spectrum,
                                const ControlSettings& settings)
{
  return m_spectrumAnimationFactory->createSpectrumAnimation(nLEDs, spectrum, settings);
}


void
SpectrumStudio::drawSpectrumInConsole(const std::vector<std::pair<double, double>>& spectrum,
                                      int minFreq,
                                      int maxFreq) const
{
  for (const auto& bin : spectrum)
  {
    int frequency = static_cast<int>(bin.first);
    if (frequency > minFreq && frequency < maxFreq)
    {
      std::cout << frequency << " :";
      for (std::size_t j = 0; j < bin.second/4; ++j)
      {
        std::cout << ".";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}
