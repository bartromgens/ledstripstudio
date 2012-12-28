#include "spectrumanalyser.h"
#include "spectrumobserver.h"

#include <QTime>

#include <boost/thread.hpp>

using namespace fftwpp;

#define _USE_MATH_DEFINES
#include <math.h>

SpectrumAnalyser::SpectrumAnalyser(int nSamples)
  : AudioInputObserver(),
    m_nSamples(nSamples),
    m_np(nSamples/2+1),
    m_observers(),
    m_mutex()
{
  fftw::maxthreads = 1; // single thread is faster for the size the input data

  std::cout << "SpectrumAnalyser::SpectrumAnalyser() - m_n: " << m_nSamples << std::endl;

  m_f = FFTWdouble(m_nSamples);
  m_g = FFTWComplex(m_np);

  m_forward = new fftwpp::rcfft1d(m_nSamples, m_f, m_g);
}


SpectrumAnalyser::~SpectrumAnalyser()
{
  FFTWdelete(m_g);
  FFTWdelete(m_f);

  delete m_forward;
}


void
SpectrumAnalyser::notifyAudioData(std::deque<float> audioData, int sampleRate)
{
//  std::cout << "SpectrumAnalyser::notifyAudioData() - audio data size: " << audioData.size() << std::endl;

  std::map<double, double> spectrum = computeSpectrum(audioData, 4000, sampleRate, SpectrumAnalyser::linear);
  notifyObservers(spectrum);
}


void
SpectrumAnalyser::registerObserver(SpectrumObserver* observer)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  assert(observer);
  m_observers.push_back(observer);
}


void
SpectrumAnalyser::unregisterObserver(SpectrumObserver* observer)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  m_observers.erase( std::find(m_observers.begin(), m_observers.end(), observer) );
}


void
SpectrumAnalyser::notifyObservers(const std::map<double, double>& spectrum)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  for (std::size_t i = 0; i < m_observers.size(); ++i)
  {
    m_observers[i]->notifySpectrum(spectrum);
  }
}


unsigned int
SpectrumAnalyser::getNSamples() const
{
  return m_nSamples;
}


std::map<double, double>
SpectrumAnalyser::binSpectrum(const std::vector<double>& data, int nBins, int sampleRate) const
{
  std::map<double, double> hist;
  double binWidth = sampleRate/nBins;
//  std::cout << "buildHist() - min, max: " << min << ", " << max << std::endl;

  int max = data.size();
  int min = 0;

  for (std::size_t i = 0; i < data.size(); ++i)
  {
    int bin = static_cast<int>( (i-min) / ((max-min)/(nBins)) );

    if ( (bin >= 0) && ( bin < nBins) && (bin*binWidth < sampleRate/2))
    {
      hist[bin*binWidth] += data[i];
    }
  }

  return hist;
}

std::map<double, double>
SpectrumAnalyser::computeSpectrum(const std::deque<float>& realIn, int nBins, int sampleRate, SpectrumAnalyser::windowingType windowType)
{
  QTime timer;
  timer.start();

  std::deque<float> realInWindowed;

  if (windowType == SpectrumAnalyser::hann)
  {
    realInWindowed = hannWindowFunction(realIn);
  }
  else if (windowType == SpectrumAnalyser::linear)
  {
    realInWindowed = linearWindowFunction(realIn);
  }
  else if (windowType == SpectrumAnalyser::none)
  {
    realInWindowed = realIn;
  }

//  std::cout << "SpectrumAnalyser::computeSpectrum() - hannWindowFunction time: " << timer.elapsed() << std::endl;

  timer.restart();

  for(unsigned int i = 0; (i < m_nSamples) && (i < realIn.size()); i++)
  {
//    m_f[i] = realIn[i];
    m_f[i] = realInWindowed[i];
  }

  timer.restart();
  // forward FFT real to complex
  m_forward->fft(m_f, m_g);

  std::vector<double> magnitude(m_np);
  for(unsigned int i=0; i < m_np; i++)
  {
    magnitude[i] = sqrt( m_g[i].real()*m_g[i].real() + m_g[i].imag()*m_g[i].imag() );
  }

  // put into bins/buckets (histogram)
  std::map<double, double> bins = binSpectrum(magnitude, nBins, sampleRate);

//  std::cout << "SpectrumAnalyser::computeSpectrum() - end. timer: " << timer.elapsed() << std::endl;
  return bins;
}


std::deque<float>
SpectrumAnalyser::hannWindowFunction(const std::deque<float>& in) const
{
  std::size_t sizeIn = in.size();

  std::deque<float> out(sizeIn, 0.0);

//  if (in.size() < 65537)
//  {
//    std::cout << in.size() << std::endl;
//  }

  QTime timer;
  timer.start();
  for (std::size_t i = 0; i < sizeIn; i++)
  {
    out[i] = 0.5 * ( 1.0 - cos((2.0*M_PI*i)/sizeIn) ) * in[i];
  }

  return out;
}

std::deque<float>
SpectrumAnalyser::linearWindowFunction(const std::deque<float>& in) const
{
  std::size_t sizeIn = in.size();

  std::deque<float> out(sizeIn, 0.0);

//  if (in.size() < 65537)
//  {
//    std::cout << in.size() << std::endl;
//  }

  QTime timer;
  timer.start();
  for (std::size_t i = 0; i < sizeIn; i++)
  {
    out[i] = (1.0*i)/sizeIn * in[i];
  }

  return out;
}


