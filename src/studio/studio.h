#ifndef STUDIO_H
#define STUDIO_H

#include "src/basic/animation.h"

class Studio
{
public:
  Studio(int nLEDs);
  ~Studio();

  Animation createSingleColorSingleFrameAnimation(const Color& color) const;

  Animation createMovingLine(int nFrames, const Color& color, double speed);
  Animation createMovingDot(int startPos, int nFrames, const Color& color, double speed);
private:

private:
  int m_nLEDs;

};

#endif // STUDIO_H
