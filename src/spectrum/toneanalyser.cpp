#include "toneanalyser.h"

#include <cmath>

ToneAnalyser::ToneAnalyser()
  : m_baseTones()
{
  m_baseTones["C"] = 16.35;
  m_baseTones["D"] = 18.35;
  m_baseTones["E"] = 20.6;
  m_baseTones["F"] = 21.83;
  m_baseTones["G"] = 24.50;
  m_baseTones["A"] = 27.50;
  m_baseTones["B"] = 30.87;
}


std::map<std::string, double>
ToneAnalyser::computeToneAmplitude(std::map<double, double> spectrum)
{
  double range = 1.0;

  std::map<std::string, double> toneAmplitudes;

  for (auto iter = spectrum.begin(); iter != spectrum.end(); ++iter)
  {
    int frequency = iter->first;
    int amplitude = iter->second;

    // find octave above base tones;
    int nOctaves = 0;
    for (std::size_t j = 1; j < 10; ++j)
    {
      if ( frequency > m_baseTones["C"] * std::pow(2, j) - range * std::pow(2, nOctaves)
           && frequency < m_baseTones["B"] * std::pow(2, j) + range * std::pow(2, nOctaves) )
      {
//        std::cout << m_baseTones["C"] * std::pow(2, j) << std::endl;
        nOctaves = j;
        break;
      }
    }

//    std::cout << "ToneAnalyser::computeToneAmplitude() - frequency: " << frequency << ", nOctaves: " << nOctaves << std::endl;

    if (nOctaves > 2)
    {
      for (auto iterBaseTones = m_baseTones.begin(); iterBaseTones != m_baseTones.end(); ++iterBaseTones)
      {
        if ( frequency > iterBaseTones->second * std::pow(2, nOctaves) - range * std::pow(2, nOctaves)
             && frequency < iterBaseTones->second * std::pow(2, nOctaves) + range * std::pow(2, nOctaves))
        {
          toneAmplitudes[iterBaseTones->first] += amplitude;
        }
      }
    }
  }

  return toneAmplitudes;
}
