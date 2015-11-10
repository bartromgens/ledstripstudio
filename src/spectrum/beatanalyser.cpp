#include "beatanalyser.h"

#include <QDebug>


BeatAnalyser::BeatAnalyser()
: AudioInputObserver(),
  m_timer()
{
  m_timer.start();
}


BeatAnalyser::~BeatAnalyser()
{
}


void
BeatAnalyser::notifyAudioData(std::deque<float> audioData, int sampleRate)
{
  if (audioData.size() < 1024)
  {
    return;
  }

  std::size_t sampleSize = 1024;
  std::size_t nSamples = 43;

  for (std::size_t j = 0; j < audioData.size()/sampleSize; ++j)
  {
    double energySample = 0.0;
    for (std::size_t i = 0; i < sampleSize; ++i)
    {
      energySample += audioData[i+j*sampleSize]*audioData[i+j*sampleSize];
    }
    m_energySamples.push_front(energySample);
  }

  m_energySamples.resize(nSamples);

  double energyAverage = 0.0;
  for (std::size_t i = 0; i < m_energySamples.size(); ++i)
  {
    energyAverage += m_energySamples[i];
  }
  energyAverage /= static_cast<double>(m_energySamples.size());

  double energyVariance = 0.0;
  for (std::size_t i = 0; i < m_energySamples.size(); ++i)
  {
    double energyDifference = m_energySamples[i] - energyAverage;
    energyVariance += energyDifference * energyDifference;
  }
  energyVariance /= static_cast<double>(m_energySamples.size());

  double factor = -0.0025714 * energyVariance + 1.5142;
  if (m_energySamples.front() > factor * energyAverage)
  {
    qDebug() << "BEAT!!!";
//    qDebug() << "factor: " << factor;
//    qDebug() << "energyVariance: " << energyVariance;
  }
}

