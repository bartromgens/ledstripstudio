#include "toneanimationfactory.h"


ToneAnimationFactory::ToneAnimationFactory()
: m_toneColorMap()
{
}


ToneAnimationFactory::~ToneAnimationFactory()
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


std::map<Tone, Color>
ToneAnimationFactory::createToneColorMap(const std::map<Tone, double>& tones)
{
  std::map<Tone, Color> toneColorMap;
  for (const auto& tone : tones)
  {
    Color color;
    int brightness = 255;

    switch (tone.first)
    {
      case Tone::C :
        color = Color(0, brightness, brightness);
        break;
      case Tone::D :
        color = Color(brightness, 0, 0);
        break;
      case Tone::E :
        color = Color(0, brightness, 0);
        break;
      case Tone::F :
        color = Color(0, 0, brightness);
        break;
      case Tone::G :
        color = Color(brightness, 34, 4);
        break;
      case Tone::A :
        color = Color(50, brightness, 0);
        break;
      case Tone::B :
        break;
        color = Color(brightness, 75, 0);
    }

    toneColorMap[tone.first] = color;
  }

  return toneColorMap;
}


const std::map<Tone, Color>&
ToneAnimationFactory::getToneColorMap(const std::map<Tone, double>& tones)
{
  if (m_toneColorMap.empty())
  {
    m_toneColorMap = createToneColorMap(tones);
  }
  return m_toneColorMap;
}
