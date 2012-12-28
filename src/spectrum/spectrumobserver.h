#ifndef SPECTRUMOBSERVER_H
#define SPECTRUMOBSERVER_H

#include <map>

class SpectrumObserver
{
public:
  SpectrumObserver();

  virtual void notifySpectrum(std::map<double, double> spectrum) = 0;
};

#endif // SPECTRUMOBSERVER_H
