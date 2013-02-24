#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"

#include <deque>
#include <list>
#include <vector>

class Animation
{
public:
  enum AdditionType
  {
    Combine,
    Foreground,
    Background
  };

  Animation();

  void addFrame(const Frame& frame);
  void pop_frontFrame();

  const std::deque<Frame>& getFrames() const;

  void clearFrames();

  Animation combineAnimations(const std::list<Animation>& animations);
  Animation combineTwoAnimations(const Animation& animationA, const Animation& animationB) const;

  void setAdditionType(AdditionType type);
  AdditionType getAdditionType() const;

private:
  AdditionType m_additionType;
  std::deque<Frame> m_frames;
};

#endif // ANIMATION_H
