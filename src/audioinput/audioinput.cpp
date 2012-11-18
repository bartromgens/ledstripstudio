#include "src/audioinput/audioinput.h"
#include "src/audioinput/userdata.h"
#include "src/fft/spectrumanalyser.h"

#include <fstream>
#include <cmath>
#include <unistd.h>  // for (u)sleep on Linux

std::mutex g_pages_mutex;

AudioInput::AudioInput()
  : m_sampleRate(44100),
    m_nSamples(pow(2, 16)),
    m_framesPerBuffer(512),
    m_nChannels(2),
    m_data()
{
  initializeUserData(); // From now on, recordedSamples is initialised.

  m_ledPlayer = new Player();

  openStream();
  startStream();
  closeStream();

  terminatePortAudio(paNoError);
}


AudioInput::~AudioInput()
{
  delete m_ledPlayer;
}

void
AudioInput::initializeUserData()
{
//  int numBytes;
//  int totalFrames = m_nSeconds * m_sampleRate;
//  paUserData data;
//  m_data.maxFrameIndex = m_nSeconds * m_sampleRate; // Record for a few seconds.
//  m_data.frameIndex = 0;
  m_data.nChannels = m_nChannels;
  m_data.sampleRate = m_sampleRate;
  m_data.nSamples = m_nSamples;
//  m_nSamples = totalFrames * m_nChannels;
//  numBytes = m_nSamples * sizeof(float);
//  m_data.recordedSamples = (float*) malloc( numBytes );
//  m_data.recordedSamplesVec = std::vector<float>(m_nSamples);
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

  double amplifyFactor = 0.04;

//      std::cout << brightnessRed << ", " << brightnessGreen << ", " << brightnessBlue << std::endl;
  for (std::map<double, double>::const_iterator iter = spectrum.begin();
       iter != spectrum.end(); ++iter)
  {
    double frequency = iter->first;
    double amplitude = iter->second;

    if (frequency > 150)
    {
      if (frequency < 220.0)
      {
        brightnessRed += amplitude*amplifyFactor*2.0;
      }
      else if (frequency < 440.0)
      {
        brightnessGreen += amplitude*amplifyFactor;
      }
      else if (frequency < 2000.0)
      {
        brightnessBlue += amplitude*amplifyFactor/3.0;
      }
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
  printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

  SpectrumAnalyser spectrumAnalyser(m_nSamples);

  QTime timer;

  while( ( err = Pa_IsStreamActive( m_stream ) ) == 1 )
  {
    timer.start();
    Pa_Sleep(15);
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
