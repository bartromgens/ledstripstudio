#include "toneanimationfactoryhistory.h"

#include "studio/studio.h"

#include <QDebug>

#include <cassert>


ToneAnimationFactoryHistory::ToneAnimationFactoryHistory()
: ToneAnimationFactory(),
  m_toneHistory()
{
}


ToneAnimationFactoryHistory::~ToneAnimationFactoryHistory()
{
}


Animation
ToneAnimationFactoryHistory::createToneAnimation(unsigned int nLEDs, const ToneData& toneData)
{
  m_toneHistory.push_front(std::make_pair(toneData.maxTone, toneData.maxToneAmplitude));
  if (m_toneHistory.size() > nLEDs)
  {
    m_toneHistory.pop_back();
  }

  return doCreateToneAnimation(nLEDs, toneData);
}


const
std::deque<std::pair<Tone, double> >&ToneAnimationFactoryHistory::getToneHistory() const
{
  return m_toneHistory;
}


Color
ToneAnimationFactoryHistory::getToneColor(Tone tone, double colorWheelOffset)
{
  unsigned int i = 0;
  switch (tone)
  {
    case Tone::C :
      i = 0;
      break;
    case Tone::D :
      i = 1;
      break;
    case Tone::E :
      i = 2;
      break;
    case Tone::F :
      i = 3;
      break;
    case Tone::G :
      i = 4;
      break;
    case Tone::A :
      i = 5;
      break;
    case Tone::B :
      i = 6;
      break;
  }

  unsigned int colorInt = static_cast<int>( 3.0*127.0/7.0* (i + colorWheelOffset) ) % (3*128);
  return Studio::wheel(colorInt);
}


double
ToneAnimationFactoryHistory::getNormalisedBrightness(double toneAmplitude, const ToneData& toneData)
{
//  double toneAmplitudeSum = 0.0;
//  for (const auto& toneAmplitude : toneData.currentTones)
//  {
//    toneAmplitudeSum += toneAmplitude.second;
//  }
//  double toneAmplitudeAverage = toneAmplitudeSum/toneData.currentTones.size();

  double brightnessRelative = 0.0;
  if (toneData.toneMaxAverage > 0.01)
  {
//    brightnessRelative = std::log( toneAmplitude / ((toneData.toneMaxAverage + toneData.toneMinAverage)) ) * 0.5;  // see also https://en.wikipedia.org/wiki/Weber-Fechner_law
    brightnessRelative = std::log( toneAmplitude / (toneData.toneMaxAverage/3.0) ) * 0.5;  // see also https://en.wikipedia.org/wiki/Weber-Fechner_law
    if (brightnessRelative < 0.0)
    {
      brightnessRelative = 0.0;
    }
    assert(brightnessRelative >= 0.0);
  }

  return brightnessRelative;
}
