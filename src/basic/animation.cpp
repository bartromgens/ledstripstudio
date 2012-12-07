#include "animation.h"

#include "assert.h"

Animation::Animation()
  : m_additionType(Animation::Combine),
    m_frames()
{
}

const std::deque<Frame>&
Animation::getFrames() const
{
  return m_frames;
}

void
Animation::addFrame(const Frame& frame)
{
  m_frames.push_back(frame);
}

void
Animation::setAdditionType(AdditionType type)
{
  m_additionType = type;
}

Animation::AdditionType
Animation::getAdditionType() const
{
  return m_additionType;
}

Animation
Animation::combineAnimations(const std::list<Animation>& animations)
{
  animations.back();
  Animation animation;
  return animation;
}

Animation
Animation::combineTwoAnimations(const Animation& animationA, const Animation& animationB) const
{
  Animation animation;

  const std::deque<Frame>& framesA = animationA.getFrames();
  const std::deque<Frame>& framesB = animationB.getFrames();
  std::size_t maxFrames = std::max(framesA.size(), framesB.size());
  std::size_t minFrames = std::min(framesA.size(), framesB.size());

  std::cout << "Animation::combineTwoAnimations() - minframes: " << minFrames << std::endl;
  std::cout << "Animation::combineTwoAnimations() - maxframes: " << maxFrames << std::endl;

  for (std::size_t i = 0 ; i < minFrames; ++i)
  {
    const std::vector<LED>& ledsA = framesA[i].getLEDs();
    const std::vector<LED>& ledsB = framesB[i].getLEDs();
    assert(ledsB.size() == ledsA.size());

    Frame combinedFrame(ledsA.size());

    for (std::size_t j = 0 ; j < ledsA.size(); ++j)
    {
      LED ledA = ledsA.at(j);
      LED ledB = ledsB.at(j);

      Color colorA = ledA.getColor();
      Color colorB = ledB.getColor();

      Color combinedColor(colorA.r + colorB.r, colorA.g + colorB.g, colorA.b + colorB.b);

      LED ledNew(j, combinedColor);

      combinedFrame.addLED(ledNew);
    }
    animation.addFrame(combinedFrame);
  }

  return animation;
}

