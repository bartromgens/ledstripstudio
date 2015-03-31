#ifndef AUDIOINPUTOBSERVER_H
#define AUDIOINPUTOBSERVER_H

#include <deque>

class AudioInputObserver
{
public:

  AudioInputObserver();
  virtual ~AudioInputObserver();

  virtual void notifyAudioData(std::deque<float> audioData, int sampleRate) = 0;

};

#endif // AUDIOINPUTOBSERVER_H
