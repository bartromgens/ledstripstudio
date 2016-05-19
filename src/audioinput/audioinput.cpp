#include "audioinput/audioinput.h"
#include "spectrum/spectrumanalyser.h"
#include "spectrum/toneanalyser.h"

#include <cassert>
#include <cmath>


AudioInput::AudioInput(unsigned int nSamples, ControlSettings& settings)
  : m_sampleRate(44100),
    m_nSamples(nSamples),
    m_nChannels(1),
    m_data(),
    m_controlSettings(settings),
    m_audioObservers(),
    m_observerMutex()
{
  std::cout << "AudioInput::AudioInput() - sample size: " << m_nSamples << std::endl;

  initializeUserData(); // From now on, recordedSamples is initialised.
}


AudioInput::~AudioInput()
{
  std::cout << "AudioInput::~AudioInput()" << std::endl;
}


void
AudioInput::initializeUserData()
{
  m_data.nChannels = m_nChannels;
  m_data.sampleRate = m_sampleRate;
  m_data.nSamples = m_nSamples;
}


void
AudioInput::setSampleSize(unsigned int nSamples)
{
  std::lock_guard<std::mutex> lock(m_data.data_mutex);
  m_data.nSamples = nSamples;
  m_nSamples = nSamples;
  m_data.recordedSamplesVec.resize(nSamples);
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
  if (inputParameters.device == paNoDevice)
  {
    fprintf(stderr,"Error: No default input device.\n");
    terminatePortAudio(err);
  }

  inputParameters.channelCount = m_nChannels;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  std::cout << "AudioInput::openStream() - suggested latency: " << inputParameters.suggestedLatency << std::endl;

  err = Pa_OpenStream( &m_stream,
                       &inputParameters,
                       NULL,                  // &outputParameters,
                       m_sampleRate,
                       paFramesPerBufferUnspecified, // accept buffers of any size
                       paClipOff,      // we won't output out of range samples so don't bother clipping them
                       recordCallback,
                       &m_data );

  if( err != paNoError )
  {
    terminatePortAudio(err);
  }
}


bool
AudioInput::closeStream()
{
  std::cout << "AudioInput::closeStream()" << std::endl;
  PaError err = paNoError;

  err = Pa_CloseStream( m_stream );
  terminatePortAudio(err);
  if( err != paNoError )
  {
    std::cout << "AudioInput::closeStream() - terminatePortAudio ERROR: " << static_cast<int>(err) << std::endl;
    return false;
  }

  return true;
}


void
AudioInput::startStream()
{
  PaError err = paNoError;

  err = Pa_StartStream( m_stream );
  if( err != paNoError )
  {
    terminatePortAudio(err);
  }

  bool run = true;
  while( ( err = Pa_IsStreamActive( m_stream ) ) == 1
         && run)
  {
    Pa_Sleep(10); // put the caller to sleep while the observers do their thing. related to FPS of ledstrip.

    run = m_controlSettings.isActive();

    if (m_data.recordedSamplesVec.size() >= m_nSamples)
    {
      if (m_data.data_mutex.try_lock())
      {
        std::deque<float> samples = m_data.recordedSamplesVec;
        m_data.data_mutex.unlock();

        notifyObservers(samples);
      }
    }
  }
  if( err < 0 )
  {
    terminatePortAudio(err);
  }
}


int
AudioInput::recordCallback( const void* inputBuffer,
                            void* /*outputBuffer*/,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* /*timeInfo*/,
                            PaStreamCallbackFlags /*statusFlags*/,
                            void* userData )
{
  paUserData* data = (paUserData*)userData;

  std::lock_guard<std::mutex> lock(data->data_mutex);

  const float* rptr = (const float*)inputBuffer;
//  float *wptr = &data->recordedSamples[data->frameIndex * data->nChannels];
//  unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

  if( inputBuffer == NULL )
  {
    std::cout << "AudioInput::recordCallback() - inputBuffer NULL" << std::endl;
    return 0;
  }

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

  return 0;
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


void
AudioInput::registerObserver(AudioInputObserver* observer)
{
  std::lock_guard<std::mutex> lock(m_observerMutex);

  assert(observer);
  m_audioObservers.insert(observer);
}


void
AudioInput::unregisterObserver(AudioInputObserver* observer)
{
  std::lock_guard<std::mutex> lock(m_observerMutex);

  if ( std::find(m_audioObservers.begin(), m_audioObservers.end(), observer) != m_audioObservers.end() )
  {
    m_audioObservers.erase( std::find(m_audioObservers.begin(), m_audioObservers.end(), observer) );
  }
}


void
AudioInput::notifyObservers(const std::deque<float>& audioData)
{
  std::lock_guard<std::mutex> lock(m_observerMutex);

  for (auto iter = m_audioObservers.begin(); iter != m_audioObservers.end(); ++iter)
  {
    (*iter)->notifyAudioData(audioData, m_sampleRate);
  }
}
