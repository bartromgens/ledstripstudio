#include "tonestudio.h"

#include "spectrum/toneanalyser.h"
#include "studio.h"
#include "studio/tone/toneanimationfactoryloudest.h"
#include "studio/tone/toneanimationfactoryindividual.h"
#include "studio/tone/toneanimationfactoryhistorystatic.h"
#include "studio/tone/toneanimationfactoryhistorydynamic.h"
#include "studio/tone/toneanimationfactoryloudestsmooth.h"
#include "studio/tone/toneanimationfactoryrange.h"

#include <QDebug>

#include <cassert>
#include <cmath>
#include <string>


unsigned int ToneStudio::m_historySize = 30;


ToneStudio::ToneStudio()
: m_toneAnimationFactory(new ToneAnimationFactoryHistoryStatic()),
  m_toneHistoryFrame(0),
  m_toneData()
{
}


ToneStudio::~ToneStudio()
{
}


void
ToneStudio::writeToneToConsole(const std::map<std::string, double>& tones)
{
  for (auto iter = tones.begin(); iter != tones.end(); ++iter)
  {
    std::cout << "Tone: " << iter->first << " : " ;
    for (std::size_t i = 0; i < iter->second/15; ++i)
    {
      std::cout << "-";
    }
    std::cout << std::endl;
  }
}


void
ToneStudio::calcMaxAndMinTone(const std::map<Tone, double>& tones)
{
  m_toneData.maxToneAmplitude = std::numeric_limits<double>::min();
  m_toneData.minToneAmplitude = std::numeric_limits<double>::max();

  for (auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > m_toneData.maxToneAmplitude)
    {
      m_toneData.maxTone = it->first;
      m_toneData.maxToneAmplitude = it->second;
    }

    if (it->second < m_toneData.minToneAmplitude)
    {
      m_toneData.minToneAmplitude = it->second;
    }
  }
}


void
ToneStudio::calcToneMaxAverage()
{
  m_toneData.maxToneHistory.push_back(m_toneData.maxToneAmplitude);
  m_toneData.toneMaxAverage += m_toneData.maxToneHistory.back()/m_historySize;

  if (m_toneData.maxToneHistory.size() > m_historySize)
  {
    m_toneData.toneMaxAverage -= m_toneData.maxToneHistory.front()/m_historySize;
    m_toneData.maxToneHistory.pop_front();
  }
}


void
ToneStudio::calcToneMinAverage()
{
  m_toneData.minToneHistory.push_back(m_toneData.minToneAmplitude);
  m_toneData.toneMinAverage += m_toneData.minToneHistory.back()/m_historySize;

  if (m_toneData.minToneHistory.size() > m_historySize)
  {
    m_toneData.toneMinAverage -= m_toneData.minToneHistory.front()/m_historySize;
    m_toneData.minToneHistory.pop_front();
  }
}


Animation
ToneStudio::createToneAnimation(unsigned int nLEDs,
                                const std::map<Tone, double>& tones)
{
  m_toneData.currentTones = tones;
  m_toneData.maxToneAmplitude = 0.0;

  calcMaxAndMinTone(tones);

  calcToneMaxAverage();
  calcToneMinAverage();

  return m_toneAnimationFactory->createToneAnimation(nLEDs, m_toneData);
}


void
ToneStudio::setToneAnimationType(ToneStudio::AnimationType animationType)
{
  switch (animationType)
  {
  case ToneStudio::AnimationType::HistoryStatic :
    m_toneAnimationFactory.reset( new ToneAnimationFactoryHistoryStatic() );
    break;
  case ToneStudio::AnimationType::HistoryDynamic :
    m_toneAnimationFactory.reset( new ToneAnimationFactoryHistoryDynamic() );
    break;
  case ToneStudio::AnimationType::Individual :
    m_toneAnimationFactory.reset( new ToneAnimationFactoryIndividual() );
    break;
  case ToneStudio::AnimationType::Loudest :
    m_toneAnimationFactory.reset( new ToneAnimationFactoryLoudest() );
    break;
  case ToneStudio::AnimationType::Range :
    m_toneAnimationFactory.reset( new ToneAnimationFactoryRange() );
    break;
  case ToneStudio::AnimationType::SmoothSum :
    m_toneAnimationFactory.reset( new ToneAnimationFactoryLoudestSmooth() );
    break;
  case ToneStudio::AnimationType::None :
    assert(false);
    break;
  }
}
