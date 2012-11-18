#ifndef STUDIO_H
#define STUDIO_H

#include "src/basic/animation.h"

class Studio
{
public:
  Studio(int nLEDs);
  ~Studio();

  Animation createSingleColorSingleFrameAnimation(const Color& color, int nLEDs) const;

  Animation createMovingLine(const Color& color);
private:

private:
  int m_nLEDs;

};

#endif // STUDIO_H
