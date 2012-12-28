#ifndef AUDIOINPUTOBSERVER_H
#define AUDIOINPUTOBSERVER_H

#include <deque>

class AudioInputObserver
{
public:
  AudioInputObserver();
  ~AudioInputObserver();

  virtual void notifyAudioData(std::deque<float> audioData) = 0;
};

#endif // AUDIOINPUTOBSERVER_H
