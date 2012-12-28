#include "audioinput/audioinput.h"
#include "spectrum/spectrumanalyser.h"
#include "spectrum/toneanalyser.h"

#include <cmath>

AudioInput::AudioInput()
  : m_sampleRate(44100),
    m_nSamples( static_cast<int>(std::pow(2.0, 16)) ),
    m_nChannels(2),
    m_data(),
    m_ledPlayer(0),
    m_controlSettings(0),
    m_nUpdates(0),
    m_nLEDs(160),
    m_audioObservers(),
    m_mutex()
{
  std::cout << "AudioInput::AudioInput() - sample size: " << m_nSamples << std::endl;

  initializeUserData(); // From now on, recordedSamples is initialised.
}


AudioInput::~AudioInput()
{
  std::cout << "AudioInput::~AudioInput()" << std::endl;
  if (Pa_IsStreamActive( m_stream ))
  {
    bool closeSuccess = closeStream();
    if (closeSuccess)
    {
      terminatePortAudio(paNoError);
    }
  }
}


void
AudioInput::setLedPlayer(std::shared_ptr<Player>  ledPlayer)
{
  m_ledPlayer = ledPlayer;
}


void
AudioInput::setControlSettings(std::shared_ptr<ControlSettings> settings)
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
  if (inputParameters.device == paNoDevice)
  {
    fprintf(stderr,"Error: No default input device.\n");
    terminatePortAudio(err);
  }

  inputParameters.channelCount = 2;                    // stereo input
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

  Animation animation = createWaveformAnimationCentral(m_nLEDs, brightnessRed, brightnessGreen, brightnessBlue);
  m_ledPlayer->addAnimation(animation);

  //  Studio studio(nLEDs);
//  if (m_nUpdates++ % (nLEDs/30) == 1)
//  {
//    double speed = 1.0;
//    m_ledPlayer->addAnimation( studio.createMovingLine(nLEDs/speed,
//                                                       Color(std::abs(cos(m_nUpdates/5.0+1)*127), std::abs(sin(m_nUpdates/8.0)*127), std::abs(sin(m_nUpdates/23.0+5)*127)),
//                                                       speed)
//                             );
//  }

  m_ledPlayer->playFrame();
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
  int minFreq = 400;
  int maxFreq = 800;

  PaError err = paNoError;

  err = Pa_StartStream( m_stream );
  if( err != paNoError )
  {
    terminatePortAudio(err);
  }

  SpectrumAnalyser spectrumAnalyser(m_nSamples);
  ToneAnalyser toneAnalyser;

  QTime timer;
  timer.start();

  bool run = true;
  while( ( err = Pa_IsStreamActive( m_stream ) ) == 1
         && run)
  {
    Pa_Sleep(5);

    run = m_controlSettings->isActive();

    if (m_data.recordedSamplesVec.size() >= spectrumAnalyser.getNSamples())
    {
      if (m_data.data_mutex.try_lock())
      {
//        timer.restart();
        std::map<double, double> spectrum = spectrumAnalyser.computeSpectrum(m_data.recordedSamplesVec, 4000, m_sampleRate, SpectrumAnalyser::linear);
//        std::map<double, double> spectrum = spectrumAnalyser.computeSpectrum(m_data.recordedSamplesVec, 4000, m_sampleRate, SpectrumAnalyser::hann);
        m_data.data_mutex.unlock();

//        std::map<std::string, double> tones = toneAnalyser.computeToneAmplitude(spectrum);

//        Animation animation = createToneAnimation(m_nLEDs, tones);
//        m_ledPlayer->addAnimation(animation);
//        m_ledPlayer->playFrame();

//        std::cout << "AudioInput::startStream() - computeSpectrum time: " << timer.elapsed() << std::endl;

        updateLEDs(spectrum);

//        drawSpectrumInConsole(spectrum, minFreq, maxFreq);
      }
    }
  }
  if( err < 0 )
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
  if( err != paNoError )
  {
    std::cout << "AudioInput::closeStream() - ERROR: terminatePortAudio" << std::endl;
    terminatePortAudio(err);
    return false;
  }

  return true;
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

  boost::lock_guard<boost::mutex> lock(data->data_mutex);

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

//  int maxRGB = std::max(brightnessBlue, std::max(brightnessRed, brightnessGreen));
//  brightnessRed *= 127.0/maxRGB;
//  brightnessGreen *= 127.0/maxRGB;
//  brightnessBlue *= 127.0/maxRGB;

  for (int i = 0; i < centreLedNr; ++i)
  {
    int scaleFact = (i * 127) / centreLedNr;
//    int scaleFact = 0.0;

    int r = brightnessRed - scaleFact;
    int g = brightnessGreen - scaleFact;
    int b = brightnessBlue - scaleFact;

    if (r > 127.0)
    {
      r = 127.0;
    }
    if (g > 127.0)
    {
      g = 127.0;
    }
    if (b > 127.0)
    {
      b = 127.0;
    }

    if (r < 0.0)
    {
      r = 0.0;
    }
    if (g < 0.0)
    {
      g = 0.0;
    }
    if (b < 0.0)
    {
      b = 0.0;
    }

    int offSet = 0;
    int ledNr1 = (centreLedNr + i + 1 + offSet) % nLEDs;
    int ledNr2 = (centreLedNr - i + offSet) % nLEDs;

    LED led(ledNr1, Color(r, g, b));
    frame.addLED(led);

    LED led2(ledNr2, Color(r, g, b));
    frame.addLED(led2);
  }

  animation.addFrame(frame);
  return animation;
}


Animation
AudioInput::createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones)
{
  Animation animation;

  Frame frame(nLEDs);

//  for (auto iter = tones.begin(); iter != tones.end(); ++iter)
//  {
//    std::cout << "Tone: " << iter->first << " : " ;
//    for (std::size_t i = 0; i < iter->second/15; ++i)
//    {
//      std::cout << "-";
//    }
//    std::cout << std::endl;
//  }

  double currentMax = 0.0;
  std::string maxTone;

  for(auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > currentMax)
    {
      maxTone = it->first;
      currentMax = it->second;
    }
  }

//          std::cout << "AudioInput::createToneAnimation() - maxTone: " << maxTone << std::endl;

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
//    double r = 0.0;
//    double g = 0.0;
//    double b = 0.0;

//    for (auto iter = tones.begin(); iter != tones.end(); ++iter)
//    {
//      if (iter->first == "C")
//      {
//        r += iter->second;
//      }
//      else if (iter->first == "D")
//      {
//        r += iter->second/2.0;
////        g += iter->second/2.0;
//      }
//      else if (iter->first == "E")
//      {
//        g += iter->second;
//      }
//      else if (iter->first == "F")
//      {
//        g += iter->second/2.0;
////        b += iter->second/2.0;
//      }
//      else if (iter->first == "G")
//      {
//        b += iter->second;
//      }
//      else if (iter->first == "A")
//      {
//        b += iter->second/2.0;
////        r += iter->second/2.0;
//      }
//      else if (iter->first == "B")
//      {
//        r += iter->second;
//      }
//    }

//    int norm = sqrt(r*r+g*g+b*b);
//    int rNorm = r/norm*127;
//    int gNorm = g/norm*127;
//    int bNorm = b/norm*127;

//    Color color(rNorm, gNorm, bNorm);

    Color color;
    if (maxTone == "C")
    {
      color = Color(127, 0, 0);
    }
    else if (maxTone == "D")
    {
      color = Color(127, 127, 0);
    }
    else if (maxTone == "E")
    {
      color = Color(0, 127, 0);
    }
    else if (maxTone == "F")
    {
      color = Color(0, 127, 127);
    }
    else if (maxTone == "G")
    {
      color = Color(0, 0, 127);
    }
    else if (maxTone == "A")
    {
      color = Color(127, 0, 127);
    }
    else if (maxTone == "B")
    {
      color = Color(127, 50, 50);
    }

    LED led(i, color);
    frame.addLED(led);
  }

  animation.addFrame(frame);

  return animation;
}


void
AudioInput::registerObserver(AudioInputObserver* observer)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  assert(observer);
  m_audioObservers.push_back(observer);
}


void
AudioInput::unregisterObserver(AudioInputObserver* observer)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  m_audioObservers.erase( std::find(m_audioObservers.begin(), m_audioObservers.end(), observer) );
}


void
AudioInput::notifyObservers(const std::deque<float>& audioData)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  for (std::size_t i = 0; i < m_audioObservers.size(); ++i)
  {
    m_audioObservers[i]->notifyAudioData(audioData);
  }
}
