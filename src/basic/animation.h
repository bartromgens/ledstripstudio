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

  const std::deque<Frame>& getFrames() const;

  void addFrame(const Frame& frame);
  Animation combineAnimations(const std::list<Animation>& animations);

  AdditionType getAdditionType() const;
  void setAdditionType(AdditionType type);

  Animation combineTwoAnimations(const Animation& animationA, const Animation& animationB) const;

private:
  AdditionType m_additionType;
  std::deque<Frame> m_frames;
};

#endif // ANIMATION_H
