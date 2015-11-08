#include "toneanalyser.h"

#include <algorithm>
#include <cmath>
#include <cassert>

ToneAnalyser::ToneAnalyser()
  : SpectrumObserver(),
    m_baseTones()
{
  m_baseTones[Tone::C] = 16.35;
  m_baseTones[Tone::D] = 18.35;
  m_baseTones[Tone::E] = 20.6;
  m_baseTones[Tone::F] = 22.83; // 21.83 (original value, increased for better distinction)
  m_baseTones[Tone::G] = 24.50;
  m_baseTones[Tone::A] = 27.50;
  m_baseTones[Tone::B] = 31.87; // 30.87 (original value, increased for better distinction)
}


ToneAnalyser::~ToneAnalyser()
{
  std::cout << "ToneAnalyser::~ToneAnalyser()" << std::endl;
}


void
ToneAnalyser::notifySpectrum(const std::map<double, double>& spectrum)
{
  const std::map<Tone, double>& toneAmplitude = computeToneAmplitude(spectrum);
  notifyObservers(toneAmplitude);
}


std::map<Tone, double>
ToneAnalyser::computeToneAmplitude(const std::map<double, double>& spectrum)
{
  double range = 1.0;

  std::map<Tone, double> toneAmplitudes;

  for (auto iter = spectrum.begin(); iter != spectrum.end(); ++iter)
  {
    int frequency = iter->first;
    int amplitude = iter->second;

    // find octave above base tones;
    int nOctaves = 0;
    for (int j = 1; j < 10; ++j)
    {
      if ( frequency > m_baseTones[Tone::C] * std::pow(2.0, j) - range * std::pow(2.0, nOctaves)
           && frequency < m_baseTones[Tone::B] * std::pow(2.0, j) + range * std::pow(2.0, nOctaves) )
      {
//        std::cout << m_baseTones["C"] * std::pow(2, j) << std::endl;
        nOctaves = j;
        break;
      }
    }

    if (nOctaves > 2)
    {
      for (auto iterBaseTones = m_baseTones.begin(); iterBaseTones != m_baseTones.end(); ++iterBaseTones)
      {
        if ( frequency > iterBaseTones->second * std::pow(2.0, nOctaves) - range * std::pow(2.0, nOctaves)
             && frequency < iterBaseTones->second * std::pow(2.0, nOctaves) + range * std::pow(2.0, nOctaves))
        {
          toneAmplitudes[iterBaseTones->first] += amplitude;
        }
      }
    }
  }

  return toneAmplitudes;
}


void
ToneAnalyser::registerObserver(ToneObserver* observer)
{
  assert(observer);
  m_observers.insert(observer);
}


void
ToneAnalyser::unregisterObserver(ToneObserver* observer)
{
  m_observers.erase( std::find(m_observers.begin(), m_observers.end(), observer) );
}


void
ToneAnalyser::notifyObservers(const std::map<Tone, double>& toneAmplitudes)
{
  for (auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
  {
    (*iter)->notifyTone(toneAmplitudes);
  }
}
