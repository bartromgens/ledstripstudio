#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "userdata.h"
#include "audioinputobserver.h"

#include "basic/color.h"
#include "basic/animation.h"
#include "player/player.h"
#include "settings/controlsettings.h"

#include "portaudio/portaudio.h"

#include <QTime>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>

class AudioInput
{
public:
  struct Stream{
    paUserData* data;
    PaStream* stream;
  };

public:
  AudioInput(unsigned int nSamples, unsigned int nLEDs);
  ~AudioInput();

  void openStream();
  void startStream();
  bool closeStream();

  void setControlSettings(std::shared_ptr<ControlSettings> settings);

  void registerObserver(std::shared_ptr<AudioInputObserver> observer);
  void unregisterObserver(std::shared_ptr<AudioInputObserver> observer);
  void notifyObservers(const std::deque<float>& audioData);

  void setNSamples(unsigned int nSamples);

private:

  void initializeUserData();

  //This routine will be called by the PortAudio engine when audio is needed.
  //It may be called at interrupt level on some machines so don't do anything
  //that could mess up the system like calling malloc() or free().
  static int recordCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData );

  void terminatePortAudio(PaError err);

private:

  double m_sampleRate;
  unsigned int m_nSamples;
  int m_nChannels;
  PaStream* m_stream;
  paUserData m_data;
  std::shared_ptr<ControlSettings> m_controlSettings;
  int m_nUpdates;

  int m_offSet;
  unsigned int m_nLEDs;

  std::set< std::shared_ptr<AudioInputObserver> > m_audioObservers;

  mutable std::mutex m_mutex;
};

#endif // AUDIOINPUT_H
