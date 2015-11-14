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

