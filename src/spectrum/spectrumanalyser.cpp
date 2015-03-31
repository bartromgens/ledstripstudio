#include "spectrumanalyser.h"
#include "spectrumobserver.h"

#include <QDebug>
#include <QTime>

#include <cassert>

using namespace fftwpp;

#define _USE_MATH_DEFINES
#include <math.h>

SpectrumAnalyser::SpectrumAnalyser(int nSamples)
  : AudioInputObserver(),
    m_nSamples(nSamples),
    m_np(nSamples/2+1),
    m_observers(),
    m_mutex(),
    m_windowingType(linear)
{
  fftw::maxthreads = 1; // single thread is faster for the size of most input data

  m_f = FFTWdouble(m_nSamples);
  m_g = FFTWComplex(m_np);

  m_forward = new fftwpp::rcfft1d(m_nSamples, m_f, m_g);

  m_hannWindowFactors.reserve(m_nSamples);

  double factor = (2.0*M_PI)/m_nSamples;
  for (std::size_t i = 0; i < m_nSamples; i++)
  {
    m_hannWindowFactors[i] = 0.5 * ( 1.0 - cos(factor*i) );
  }
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
//  computeSpectrumThread(audioData, 4000, sampleRate, SpectrumAnalyser::linear);
//  computeSpectrum(audioData, 4000, sampleRate, SpectrumAnalyser::hann);
//  m_time.restart();

  WindowingType windowType;
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    windowType = m_windowingType;
  }

  computeSpectrum(audioData, 4000, sampleRate, windowType);
//  std::cout << "SpectrumAnalyser::notifyAudioData() - computeSpectrum time: " << m_time.elapsed() << std::endl;
}


void
SpectrumAnalyser::registerObserver(SpectrumObserver* observer)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  assert(observer);
  m_observers.insert(observer);
}


void
SpectrumAnalyser::unregisterObserver(SpectrumObserver* observer)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (std::find(m_observers.begin(), m_observers.end(), observer) != m_observers.end())
  {
    m_observers.erase( std::find(m_observers.begin(), m_observers.end(), observer) );
  }
}


void
SpectrumAnalyser::notifyObservers(const std::map<double, double>& spectrum)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  for (auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
  {
    (*iter)->notifySpectrum(spectrum);
  }
}


std::map<double, double>
SpectrumAnalyser::computeSpectrum(const std::deque<float>& realIn, int nBins, int sampleRate, SpectrumAnalyser::WindowingType windowType)
{
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

  std::map<double, double> bins;
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    for(unsigned int i = 0; (i < m_nSamples) && (i < realIn.size()); i++)
    {
      m_f[i] = realInWindowed[i];
    }

    // forward FFT real to complex
    m_forward->fft(m_f, m_g);

    std::vector<double> magnitude(m_np);
    for (unsigned int i = 0; i < m_np; i++)
    {
      magnitude[i] = sqrt( m_g[i].real()*m_g[i].real() + m_g[i].imag()*m_g[i].imag() );
    }

    double upperFrequency = 2000.0;
    // put into bins/buckets (histogram)
    bins = binSpectrum(magnitude, nBins, sampleRate, upperFrequency);
  }

  notifyObservers(bins);
  return bins;
}


std::deque<float>
SpectrumAnalyser::hannWindowFunction(const std::deque<float>& in) const
{
  assert(in.size() >= m_nSamples);

  std::deque<float> out(m_nSamples);
  for (std::size_t i = 0; i < m_nSamples; i++)
  {
    out[i] = m_hannWindowFactors[i] * in[i];
  }

  return out;
}


std::deque<float>
SpectrumAnalyser::linearWindowFunction(const std::deque<float>& in) const
{
  std::size_t sizeIn = in.size();

  std::deque<float> out(sizeIn, 0.0);

  QTime timer;
  timer.start();
  for (std::size_t i = 0; i < sizeIn; i++)
  {
    out[i] = (1.0*i)/sizeIn * in[i];
  }

  return out;
}


std::map<double, double>
SpectrumAnalyser::binSpectrum(const std::vector<double>& data, int nBins, int sampleRate, double upperFrequency) const
{
  assert(data.size() == m_np);
  std::map<double, double> hist;
  double binWidth = sampleRate/nBins;
//  std::cout << "buildHist() - min, max: " << min << ", " << max << std::endl;

  int max = m_np;
  int min = 0;
  int difference = max-min;

  double frequency = 0.0;

  for (std::size_t i = 0; i < m_np && frequency < upperFrequency; ++i)
  {
    int bin = static_cast<int>( (i-min) / (difference/nBins) );

    double frequency = bin*binWidth;
    if ( (bin >= 0) && ( bin < nBins) && (frequency < sampleRate/2))
    {
      hist[frequency] += data[i];
    }
  }

  return hist;
}


void
SpectrumAnalyser::setNSamples(unsigned int nSamples)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_nSamples = nSamples;
  m_np = nSamples/2 + 1;

  m_f = FFTWdouble(nSamples);

  m_g = FFTWComplex(m_np);

  m_forward = new fftwpp::rcfft1d(nSamples, m_f, m_g);
}


unsigned int
SpectrumAnalyser::getNSamples() const
{
  std::lock_guard<std::mutex> lock(m_mutex);

  return m_nSamples;
}


void
SpectrumAnalyser::setWindowingType(WindowingType type)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_windowingType = type;
}
