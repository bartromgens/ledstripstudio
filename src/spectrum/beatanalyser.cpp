#include "beatanalyser.h"

#include <QDebug>

const std::size_t nSubBands = 64;

BeatAnalyser::BeatAnalyser()
: SpectrumObserver(),
  m_energyHistoryBands(nSubBands),
  m_timer(),
  m_timeLastBeat(nSubBands, 0),
  m_timeSinceLastBeat(nSubBands, 0)
{
  m_timer.start();
}


BeatAnalyser::~BeatAnalyser()
{
}


void
BeatAnalyser::notifySpectrum(const std::vector<std::pair<double, double>>& spectrum)
{
  const std::size_t nFrequencies = spectrum.size();
  const std::size_t nSamples = 43;

  std::vector<double> amplitudes;
  for (const auto& band : spectrum)
  {
    amplitudes.push_back(band.second);
  }

  for (std::size_t j = 0; j < nSubBands; ++j)
  {
    double bandEnergy = 0.0;
    for (std::size_t i = 0; i < nFrequencies/nSubBands; ++i)
    {
      bandEnergy += amplitudes[j*nSubBands+i];
    }
    m_energyHistoryBands[j].push_front(bandEnergy);
  }

  for (std::size_t i = 0; i < m_energyHistoryBands.size(); ++i)
  {
    m_energyHistoryBands[i].resize(nSamples);
  }

  std::vector<double> averageEnergy(m_energyHistoryBands.size());
  for (std::size_t i = 0; i < m_energyHistoryBands.size(); ++i)
  {
    for (std::size_t j = 0; j < m_energyHistoryBands[i].size(); ++j)
    {
      averageEnergy[i] += m_energyHistoryBands[i][j];
    }
    averageEnergy[i] /= static_cast<double>(m_energyHistoryBands[i].size());
  }

  double factor = 2.7;
  int currentTime = m_timer.elapsed();
  for (std::size_t i = 0; i < m_energyHistoryBands.size(); ++i)
  {
    if (m_energyHistoryBands[i].front() > factor * averageEnergy[i])
    {
      double beatForce = m_energyHistoryBands[i].front() / averageEnergy[i];
      int beatBand = i;
      m_timeSinceLastBeat[i] = currentTime - m_timeLastBeat[i];
      m_timeLastBeat[i] = currentTime;
      if (m_timeSinceLastBeat[i] > 100 && m_timeSinceLastBeat[i] < 1000)
      {
        qDebug() << "BEAT!!! in band " << beatBand << " - force: " << beatForce << " - time since last beat: " << m_timeSinceLastBeat[beatBand];
      }
    }
  }
}
