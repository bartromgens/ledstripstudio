#ifndef TONEANALYSER_H
#define TONEANALYSER_H

#include "spectrumobserver.h"
#include "toneobserver.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>

class ToneAnalyser : public SpectrumObserver
{
public:
  ToneAnalyser();
  ~ToneAnalyser();

  std::map<std::string, double> computeToneAmplitude(const std::map<double, double>& spectrum);

  void notifySpectrum(const std::map<double, double>& spectrum);

  void registerObserver(ToneObserver* observer);
  void unregisterObserver(ToneObserver* observer);
  void notifyObservers(const std::map<std::string, double>& toneAmplitudes);

private:
  std::map<std::string, double> m_baseTones;
  std::set< ToneObserver* > m_observers;
};

#endif // TONEANALYSER_H
