#include "powerspectrum.h"

#include <iostream>


PowerSpectrum::PowerSpectrum(int nBins, double minFrequency, double maxFrequency)
: m_nBins(nBins),
  m_minFrequency(minFrequency),
  m_maxFrequency(maxFrequency),
  m_spectrum()
{
  m_spectrum.reserve(m_nBins);
}


double
PowerSpectrum::getBinSize() const
{
  return (m_maxFrequency - m_minFrequency) / static_cast<double>(m_nBins);
}


void
PowerSpectrum::pushBack(double frequency, double value)
{
  m_spectrum.push_back(std::make_pair(frequency, value));
}


const std::vector<std::pair<double, double>>&
PowerSpectrum::getSpectrum() const
{
  return m_spectrum;
}


std::vector<std::pair<double, double>>
PowerSpectrum::getSpectrum(double maxFrequency) const
{
  std::size_t nBins = maxFrequency/getBinSize();
  std::vector<std::pair<double, double>> spectrum(m_spectrum.begin(), m_spectrum.begin()+nBins);
  return spectrum;
}


std::vector<std::pair<double, double>>
PowerSpectrum::resizeSpectrum(std::vector<std::pair<double, double>> spectrum,
                              std::size_t nBins,
                              double maxFrequency)
{
  if (spectrum.size() < 2)
  {
    return {};
  }

  double binWidthOld = spectrum[1].first - spectrum[0].first;
  double binWidth = maxFrequency/nBins;

  if (binWidth < binWidthOld)
  {
    std::size_t nBinsOld = maxFrequency/binWidthOld;
    std::vector<std::pair<double, double>> spectrumResized(spectrum.begin(), spectrum.begin()+nBinsOld);
    return spectrumResized;
  }

  std::vector<std::pair<double, double>> spectrumNew;
  spectrumNew.reserve(nBins);
  std::size_t currentBin = 0;

  for (std::size_t i = 0; i < nBins; ++i)
  {
    double binUpperFrequency = (i+1)*binWidth;
    double binValue = 0.0;
    while (currentBin < spectrum.size() && spectrum[currentBin].first < binUpperFrequency)
    {
      binValue += spectrum[currentBin].second;
      currentBin++;
    }
    spectrumNew.push_back(std::make_pair(i*binWidth, binValue));
  }

  return spectrumNew;
}
