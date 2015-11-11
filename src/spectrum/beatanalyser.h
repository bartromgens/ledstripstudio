#ifndef BEATANALYSER_H
#define BEATANALYSER_H

#include "spectrumobserver.h"

#include <QTime>

#include <deque>


class BeatAnalyser : public SpectrumObserver
{
public:

  BeatAnalyser();
  virtual ~BeatAnalyser();

  virtual void notifySpectrum(const std::map<double, double>& spectrum);

private:
  std::vector<std::deque<double>> m_energyHistoryBands;
  QTime m_timer;
  std::vector<int> m_timeLastBeat;
  std::vector<int> m_timeSinceLastBeat;
};

#endif // BEATANALYSER_H
