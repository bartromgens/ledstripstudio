#ifndef SPECTRUMOBSERVER_H
#define SPECTRUMOBSERVER_H

#include <iostream>
#include <map>

class SpectrumObserver
{
public:
  SpectrumObserver();
  virtual ~SpectrumObserver();

  virtual void notifySpectrum(const std::map<double, double>& spectrum) = 0;
};

#endif // SPECTRUMOBSERVER_H
