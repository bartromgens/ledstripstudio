#ifndef STUDIO_H
#define STUDIO_H

#include "basic/animation.h"

class Studio
{
public:
  Studio(int nLEDs);
  ~Studio();

  Animation createSingleColorSingleFrameAnimation(const Color& color) const;

  Animation createMovingLine(int nFrames, const Color& color, double speed);
  Animation createMovingDot(int startPos, int nFrames, const Color& color, double speed);
  Animation createMovingRainbow();
  Color wheel(int wheelPos);
  Animation createRandomMovingDots(int nDots, int nFrames);
private:

private:
  int m_nLEDs;

};

#endif // STUDIO_H
