#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include "powerspectrum.h"
#include "audioinput/audioinputobserver.h"

#include "fftw++.h"

#include <QTime>

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <set>
#include <vector>


class SpectrumObserver;

class SpectrumAnalyser : public AudioInputObserver
{
public:

  enum WindowingType
  {
    none,
    linear,
    hann
  };

public:

  SpectrumAnalyser(int nSamples);
  virtual ~SpectrumAnalyser();

  virtual void notifyAudioData(std::deque<float> audioData, int sampleRate);

  void registerObserver(SpectrumObserver* observer);
  void unregisterObserver(SpectrumObserver* observer);
  void notifyObservers(const std::vector<std::pair<double, double>>& spectrum);

  PowerSpectrum computeSpectrum(const std::deque<float>& realIn, int sampleRate, SpectrumAnalyser::WindowingType windowType) ;

  void setSampleSize(unsigned int nSamples);
  unsigned int getNSamples() const;

  void setWindowingType(WindowingType type);

  void initialiseHannWindowFactors(std::size_t size);

private:

  std::deque<float> hannWindowFunction(const std::deque<float>& in) const;
  std::deque<float> linearWindowFunction(const std::deque<float>& in) const;

private:

  double* m_f; // FFT input
  Complex* m_g; // FFT output
  unsigned int m_nSamples; // number of samples
  unsigned int m_np; // number of output samples, the first (m_nSamples/2)+1 Complex Fourier values
  //Note that for a real input signal (imaginary parts all zero) the second half of the FFT (bins from N / 2 + 1 to N - 1)
  //contain no useful additional information (they have complex conjugate symmetry with the first N / 2 - 1 bins).
  //The last useful bin (for practical aplications) is at N / 2 - 1,

  fftwpp::rcfft1d* m_forward;

  std::set< SpectrumObserver* > m_observers;

  mutable std::mutex m_mutex;

  WindowingType m_windowingType;

  std::vector<double> m_hannWindowFactors;
};

#endif // SPECTRUMANALYSER_H
