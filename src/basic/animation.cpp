#include "animation.h"

Animation::Animation()
{
}

const std::list<Frame>&
Animation::getFrames() const
{
  return m_frames;
}

void
Animation::addFrame(const Frame& frame)
{
  m_frames.push_back(frame);
}

Animation
Animation::combineAnimations(const std::list<Animation>& animations)
{
  animations.back();
  Animation animation;
  return animation;
}
