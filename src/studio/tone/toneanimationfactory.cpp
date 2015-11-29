#include "toneanimationfactory.h"


ToneAnimationFactory::ToneAnimationFactory()
: m_toneColorMap()
{
}


void
ToneAnimationFactory::createRandomToneColorMap(const std::map<Tone, double>& tones)
{
  std::map<Tone, Color> toneColorMap;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    Tone tone = iter->first;
    toneColorMap[tone] = Color::randomColor();
  }

  m_toneColorMap = toneColorMap;
}


void
ToneAnimationFactory::createToneColorMap(const std::map<Tone, double>& tones)
{
  std::map<Tone, Color> toneColorMap;
  for (auto iter = tones.cbegin(); iter != tones.cend(); ++iter)
  {
    Color color;
    int brightness = 127;
    Tone tone = iter->first;

    if (tone == Tone::C)
    {
      color = Color(0, brightness, brightness);
    }
    else if (tone == Tone::D)
    {
      color = Color(brightness, 0, 0);
    }
    else if (tone == Tone::E)
    {
      color = Color(0, brightness, 0);
    }
    else if (tone == Tone::F)
    {
      color = Color(0, 0, brightness);
    }
    else if (tone == Tone::G)
    {
      color = Color(brightness, 34, 4);
    }
    else if (tone == Tone::A)
    {
      color = Color(50, brightness, 0);
    }
    else if (tone == Tone::B)
    {
      color = Color(brightness, 75, 0);
    }
    toneColorMap[tone] = color;
  }

  m_toneColorMap = toneColorMap;
}


const std::map<Tone, Color>&
ToneAnimationFactory::getToneColorMap(const std::map<Tone, double>& tones)
{
  if (m_toneColorMap.empty())
  {
    createToneColorMap(tones);
  }
  return m_toneColorMap;
}
