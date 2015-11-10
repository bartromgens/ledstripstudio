#ifndef BEATANALYSER_H
#define BEATANALYSER_H

#include "audioinput/audioinputobserver.h"

#include <QTime>

class BeatAnalyser : public AudioInputObserver
{
public:

  BeatAnalyser();
  virtual ~BeatAnalyser();

  virtual void notifyAudioData(std::deque<float> audioData, int sampleRate);

private:
  std::deque<double> m_energySamples;
  QTime m_timer;
};

#endif // BEATANALYSER_H
