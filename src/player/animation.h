#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"

#include <list>
#include <vector>

class Animation
{
public:
  Animation();

  const std::list<Frame>& getFrames() const;

  void addFrame(const Frame& frame);
  Animation combineAnimations(const std::list<Animation>& animations);
private:
  std::list<Frame> m_frames;
};

#endif // ANIMATION_H
