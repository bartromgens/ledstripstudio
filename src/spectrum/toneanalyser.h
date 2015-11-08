#ifndef TONEANALYSER_H
#define TONEANALYSER_H

#include "spectrumobserver.h"
#include "toneobserver.h"
#include "basic/tone.h"

#include <iostream>
#include <string>
#include <vector>
#include <set>

class ToneAnalyser : public SpectrumObserver
{
public:

  ToneAnalyser();
  virtual ~ToneAnalyser();

  std::map<Tone, double> computeToneAmplitude(const std::map<double, double>& spectrum);

  void notifySpectrum(const std::map<double, double>& spectrum);

  void registerObserver(ToneObserver* observer);
  void unregisterObserver(ToneObserver* observer);
  void notifyObservers(const std::map<Tone, double>& toneAmplitudes);

private:

  std::map<Tone, double> m_baseTones;
  std::set<ToneObserver*> m_observers;

};

#endif // TONEANALYSER_H
