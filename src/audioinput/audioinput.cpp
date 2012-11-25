#include "src/audioinput/audioinput.h"
#include "src/audioinput/userdata.h"
#include "src/spectrum/spectrumanalyser.h"

#include <fstream>
#include <cmath>
#include <unistd.h>  // for (u)sleep on Linux

std::mutex g_pages_mutex;

AudioInput::AudioInput()
  : m_sampleRate(44100),
    m_nSamples(pow(2, 16)),
    m_framesPerBuffer(512),
    m_nChannels(2),
    m_data(),
    m_ledPlayer(0),
    m_controlSettings(0)
{
  std::cout << "AudioInput::AudioInput()" << std::endl;
  initializeUserData(); // From now on, recordedSamples is initialised.

//  m_ledPlayer = new Player();

//  openStream();
}


AudioInput::~AudioInput()
{
  std::cout << "AudioInput::~AudioInput()" << std::endl;
  if (Pa_IsStreamActive( m_stream ))
  {
    closeStream();
    terminatePortAudio(paNoError);
  }
}


void
AudioInput::setLedPlayer(Player* ledPlayer)
{
  m_ledPlayer = ledPlayer;
}


void
AudioInput::setControlSettings(ControlSettings* settings)
{
  m_controlSettings = settings;
}


void
AudioInput::initializeUserData()
{

  m_data.nChannels = m_nChannels;
  m_data.sampleRate = m_sampleRate;
  m_data.nSamples = m_nSamples;
}


void
AudioInput::openStream()
{
  PaError err = paNoError;

  err = Pa_Initialize();
  if( err != paNoError )
  {
    terminatePortAudio(err);
  }

  PaStreamParameters inputParameters;

  inputParameters.device = Pa_GetDefaultInputDevice(); // default input device
  if (inputParameters.device == paNoDevice) {
    fprintf(stderr,"Error: No default input device.\n");
    terminatePortAudio(err);
  }

  inputParameters.channelCount = 2;                    // stereo input
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  std::cout << "AudioInput::openStream() - suggested latency: " << inputParameters.suggestedLatency << std::endl;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  // Record some audio. --------------------------------------------
  err = Pa_OpenStream( &m_stream,
                       &inputParameters,
                       NULL,                  // &outputParameters,
                       m_sampleRate,
                       m_framesPerBuffer,
                       paClipOff,      // we won't output out of range samples so don't bother clipping them
                       recordCallback,
                       &m_data );

  if( err != paNoError )
  {
    terminatePortAudio(err);
  }
}


void
AudioInput::updateLEDs(const std::map<double, double>& spectrum)
{
  double brightnessRed = 0.0;
  double brightnessGreen = 0.0;
  double brightnessBlue = 0.0;

  m_controlSettings->lock();
  double amplifyFactor = m_controlSettings->volumeTotal/1000.0;
  double amplifyFactorRed = m_controlSettings->volumeRed/25.0;
  double amplifyFactorGreen = m_controlSettings->volumeGreen/50.0;
  double amplifyFactorBlue = m_controlSettings->volumeBlue/100.0;

  int freqRmin = m_controlSettings->freqRedMin;
  int freqRmax = m_controlSettings->freqRedMax;
  int freqGmin = m_controlSettings->freqGreenMin;
  int freqGmax = m_controlSettings->freqGreenMax;
  int freqBmin = m_controlSettings->freqBlueMin;
  int freqBmax = m_controlSettings->freqBlueMax;

  m_controlSettings->unlock();

  m_controlSettings->setStatusFPS(m_ledPlayer->getFPS());


//      std::cout << brightnessRed << ", " << brightnessGreen << ", " << brightnessBlue << std::endl;
  for (std::map<double, double>::const_iterator iter = spectrum.begin();
       iter != spectrum.end(); ++iter)
  {
    double frequency = iter->first;
    double amplitude = iter->second;

    if (frequency > freqRmin && frequency < freqRmax)
    {
        brightnessRed += amplitude*amplifyFactor*amplifyFactorRed;
    }

    if (frequency > freqGmin && frequency < freqGmax)
    {
      brightnessGreen += amplitude*amplifyFactor*amplifyFactorGreen;
    }

    if (frequency > freqBmin && frequency < freqBmax)
    {
      brightnessBlue += amplitude*amplifyFactor*amplifyFactorBlue;
    }
  }

  if (brightnessBlue > 127.0)
  {
    brightnessBlue = 127.0;
  }
  if (brightnessGreen > 127.0)
  {
    brightnessGreen = 127.0;
  }
  if (brightnessRed > 127.0)
  {
    brightnessRed = 127.0;
  }

  int nLEDs = 160;

  Animation animation = createWaveformAnimationCentral(nLEDs, brightnessRed, brightnessGreen, brightnessBlue);
//  Animation animation = createWaveformAnimationUpwards(nLEDs, brightnessRed, brightnessGreen, brightnessBlue);

  m_ledPlayer->play(animation);
}


void
AudioInput::drawSpectrumInConsole(const std::map<double, double>& spectrum, int minFreq, int maxFreq) const
{
  for (std::map<double, double>::const_iterator iter = spectrum.begin();
       iter != spectrum.end(); ++iter)
  {
    int frequency = iter->first;
    if (frequency > minFreq && frequency < maxFreq)
    {
      std::cout << frequency << " :";
      for (std::size_t j = 0; j < iter->second; ++j)
      {
        std::cout << "=";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}


void
AudioInput::startStream()
{
//  int minFreq = 100;
//  int maxFreq = 600;

  PaError err = paNoError;

  err = Pa_StartStream( m_stream );
  if( err != paNoError )
  {
    terminatePortAudio(err);
  }

  SpectrumAnalyser spectrumAnalyser(m_nSamples);

  QTime timer;

  bool run = true;
  while( ( err = Pa_IsStreamActive( m_stream ) ) == 1
         && run)
  {
    timer.start();
    Pa_Sleep(15);

    m_controlSettings->lock();
    run = m_controlSettings->isActive;
    m_controlSettings->unlock();

    if (m_data.recordedSamplesVec.size() >= spectrumAnalyser.getNSamples())
    {
      if (m_data.data_mutex.try_lock())
      {
        timer.restart();
        std::map<double, double> spectrum = spectrumAnalyser.computeSpectrum(m_data.recordedSamplesVec, 4000, m_sampleRate);
        m_data.data_mutex.unlock();
        //        std::cout << "AudioInput::startStream() - computeSpectrum time: " << timer.elapsed() << std::endl;

        timer.restart();
        updateLEDs(spectrum);
//        drawSpectrumInConsole(spectrum, minFreq, maxFreq);
//        std::cout << "AudioInput::startStream() - updateLEDs time: " << timer.elapsed() << std::endl;
      }
      else
      {
        std::cout << "AudioInput::startStream() - m_data locked!" << std::endl;
      }

    }
//    std::cout << "AudioInput::startStream() - timer: " << timer.elapsed() << std::endl;
  }
  if( err < 0 )
  {
    terminatePortAudio(err);
  }
}


void
AudioInput::closeStream()
{
  std::cout << "AudioInput::closeStream()" << std::endl;
  PaError err = paNoError;

  err = Pa_CloseStream( m_stream );
  if( err != paNoError )
  {
    std::cout << "AudioInput::closeStream() - ERROR: terminatePortAudio" << std::endl;
    terminatePortAudio(err);
  }
}


int
AudioInput::recordCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{

  QTime timer;
  timer.start();

  paUserData *data = (paUserData*)userData;
  data->data_mutex.lock();

  const float *rptr = (const float*)inputBuffer;
//  float *wptr = &data->recordedSamples[data->frameIndex * data->nChannels];
  int finished(0);
//  unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

  (void) outputBuffer; // Prevent unused variable warnings.
  (void) timeInfo;
  (void) statusFlags;
  (void) userData;

  if( inputBuffer == NULL )
  {
    std::cout << "AudioInput::recordCallback() - inputBuffer NULL" << std::endl;
  }
  else
  {
    for (unsigned int i = 0; i < framesPerBuffer; i++)
    {
      if (data->recordedSamplesVec.size() > data->nSamples)
      {
        data->recordedSamplesVec.pop_front();
      }
      data->recordedSamplesVec.push_back((float)(*rptr++));

      if( data->nChannels == 2 )
      {
        if (data->recordedSamplesVec.size() > data->nSamples)
        {
          data->recordedSamplesVec.pop_front();
        }
        data->recordedSamplesVec.push_back((float)(*rptr++));
      }
    }
  }
//  data->frameIndex += framesToCalc;
//  std::cout << "time elapsed: " << timer.elapsed() << std::endl;
  data->data_mutex.unlock();
  return finished;
}


void
AudioInput::terminatePortAudio(PaError err)
{
  std::cout << "AudioInput::terminatePortAudio()" << std::endl;
  Pa_Terminate();

  if( err != paNoError )
  {
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    err = 1;          // Always return 0 or 1, but no other return codes.
  }
}


Animation
AudioInput::createWaveformAnimationCentral(int nLEDs, int brightnessRed, int brightnessGreen, int brightnessBlue)
{
  Animation animation;
  Frame frame(nLEDs);

  int centreLedNr = nLEDs/2;

  for (int i = 0; i < centreLedNr; ++i)
  {
    int scaleFact = (i * 127) / centreLedNr ;

    LED led;
    led.setLEDnr(centreLedNr+i);
    led.setColor( Color((int)(brightnessRed) - scaleFact, (int)(brightnessGreen) - scaleFact, (int)(brightnessBlue) - scaleFact ));
    frame.addLED(led);

    LED led2;
    led2.setLEDnr(centreLedNr-i+1);
    led2.setColor( Color((int)(brightnessRed) - scaleFact, (int)(brightnessGreen) - scaleFact, (int)(brightnessBlue) - scaleFact ));
    frame.addLED(led2);
  }

  animation.addFrame(frame);
  return animation;
}
