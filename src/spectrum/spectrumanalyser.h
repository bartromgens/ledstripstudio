#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include "./fftwpp/fftw++.h"

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>


class SpectrumAnalyser
{
public:
  enum windowingType
  {
    none,
    linear,
    hann
  };

public:
  SpectrumAnalyser(int nSamples);
  ~SpectrumAnalyser();

  std::map<double, double> computeSpectrum(const std::deque<float>& realIn, int nBins, int sampleRate, SpectrumAnalyser::windowingType windowType) ;

  unsigned int getNSamples() const;

private:
  std::map<double, double> binSpectrum(std::vector<double> data, int nBins, int sampleRate) const;
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
};

#endif // SPECTRUMANALYSER_H
