#ifndef AUDIOINPUTOBSERVER_H
#define AUDIOINPUTOBSERVER_H

#include <deque>

class AudioInputObserver
{
public:
  AudioInputObserver();
  ~AudioInputObserver();

  virtual void notifyAudioData(const std::deque<float>& audioData, int sampleRate) = 0;
};

#endif // AUDIOINPUTOBSERVER_H
