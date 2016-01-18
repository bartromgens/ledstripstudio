#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "userdata.h"
#include "audioinputobserver.h"

#include "basic/color.h"
#include "basic/animation.h"
#include "player/player.h"
#include "settings/controlsettings.h"

#include "portaudio/portaudio.h"

#include <QAudioFormat>
#include <QAudioInput>
#include <QIODevice>
#include <QTime>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>


class AudioInput;


class AudioInfo : public QIODevice
{
  Q_OBJECT

public:
  AudioInfo(const QAudioFormat &format, std::size_t nSamples, AudioInput* audioInput, QObject *parent);
  virtual ~AudioInfo();

  void start();
  void stop();

  qint64 readData(char *data, qint64 maxlen);
  qint64 writeData(const char *data, qint64 len);

private:
  const QAudioFormat m_format;
  std::deque<float> m_data;
  AudioInput* m_audioInput;
};


class AudioInput
{
public:
  struct Stream{
    paUserData* data;
    PaStream* stream;
  };

public:
  AudioInput(unsigned int nSamples, ControlSettings& settings);
  ~AudioInput();

  void openStream();
  void startStream();
  bool closeStream();

  void registerObserver(AudioInputObserver* observer);
  void unregisterObserver(AudioInputObserver* observer);
  void notifyObservers(const std::deque<float>& audioData);

  void setSampleSize(unsigned int nSamples);

  void startAudioMonitoring();

private:

  void initializeUserData();

  //This routine will be called by the PortAudio engine when audio is needed.
  //It may be called at interrupt level on some machines so don't do anything
  //that could mess up the system like calling malloc() or free().
  static int recordCallback( const void* inputBuffer,
                             void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData );

  void terminatePortAudio(PaError err);

private:

  double m_sampleRate;
  unsigned int m_nSamples;
  int m_nChannels;
  PaStream* m_stream;
  paUserData m_data;
  ControlSettings& m_controlSettings;

  std::set<AudioInputObserver*> m_audioObservers;

  mutable std::mutex m_observerMutex;

  QAudioInput* m_input;
  AudioInfo* m_audioInfo;
  QAudioFormat m_format;
  QAudioDeviceInfo m_deviceInfo;
};

#endif // AUDIOINPUT_H
