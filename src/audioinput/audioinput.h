#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include "portaudio/portaudio.h"
#include "userdata.h"
//#include "recordCallback.h"

#include "basic/color.h"
#include "basic/animation.h"
#include "player/player.h"
#include "settings/controlsettings.h"

#include <QTime>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <mutex>

class AudioInput
{
public:
  struct Stream{
    paUserData* data;
    PaStream* stream;
  };

public:
  AudioInput();
  ~AudioInput();

  void openStream();
  void startStream();
  void closeStream();

  void setLedPlayer(std::shared_ptr<Player> ledPlayer);
  void setControlSettings(std::shared_ptr<ControlSettings> settings);

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
  void drawSpectrumInConsole(const std::map<double, double>& spectrum, int minFreq, int maxFreq) const;
  void updateLEDs(const std::map<double, double>& spectrum);
  Animation createWaveformAnimationCentral(int nLEDs, int brightnessRed, int brightnessGreen, int brightnessBlue);

private:

  double m_sampleRate;
  int m_nSamples;
  unsigned long m_framesPerBuffer;
  int m_nChannels;
//  int m_nSamples;
  PaStream* m_stream;
  paUserData m_data;
  std::shared_ptr<Player> m_ledPlayer;
  std::shared_ptr<ControlSettings> m_controlSettings;
  int m_nUpdates;

  int m_offSet;
};

#endif // AUDIOINPUT_H
