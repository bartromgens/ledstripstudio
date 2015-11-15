#ifndef SPECTRUMOBSERVER_H
#define SPECTRUMOBSERVER_H

#include <iostream>
#include <vector>

class SpectrumObserver
{
public:
  SpectrumObserver();
  virtual ~SpectrumObserver();

  virtual void notifySpectrum(const std::vector<std::pair<double, double>>& spectrum) = 0;
};

#endif // SPECTRUMOBSERVER_H
