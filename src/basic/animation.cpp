#include "animation.h"

#include <QTime>

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

  QTime time;
  time.start();

  const std::deque<Frame>& framesA = animationA.getFrames();
  const std::deque<Frame>& framesB = animationB.getFrames();
  std::size_t maxFrames = std::max(framesA.size(), framesB.size());
  std::size_t minFrames = std::min(framesA.size(), framesB.size());

  // combine the frames
  for (std::size_t i = 0 ; i < minFrames; ++i)
  {
    const std::vector<LED>& ledsA = framesA[i].getLEDs();
    const std::vector<LED>& ledsB = framesB[i].getLEDs();
    assert(ledsB.size() == ledsA.size());

    Frame combinedFrame(ledsA.size());

    for (std::size_t j = 0 ; j < ledsA.size(); ++j)
    {
      Color colorA = ledsA.at(j).getColor();
      Color colorB = ledsB.at(j).getColor();

      Color combinedColor( std::min(colorA.r + colorB.r, 127), std::min(colorA.g + colorB.g, 127), std::min(colorA.b + colorB.b, 127) );

      LED ledNew(j, combinedColor);

      combinedFrame.addLED(ledNew);
    }
    animation.addFrame(combinedFrame);
  }

  // check who has more frames
  // fill the frames with the one that still has some left
  time.restart();
  // TODO BR: This is very inefficient when one animation has . Do not create a new animation.
  if (framesA.size() > framesB.size())
  {
    const std::deque<Frame>& frames = animationA.getFrames();
    for (std::size_t i = minFrames ; i < maxFrames; ++i)
    {
      animation.addFrame(frames[i]);
    }
  }
  else
  {
    const std::deque<Frame>& frames = animationB.getFrames();
    for (std::size_t i = minFrames ; i < maxFrames; ++i)
    {
      animation.addFrame(frames[i]);
    }
  }

  std::cout << "Animation::combineTwoAnimations - fill last frames time: " << time.elapsed() << std::endl;

  return animation;
}


void
Animation::pop_frontFrame()
{
  m_frames.pop_front();
}
