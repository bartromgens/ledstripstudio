#ifndef IMAGESTUDIO_H
#define IMAGESTUDIO_H

#include "basic/color.h"
#include "basic/animation.h"

#include <vector>
#include <string>

class ImageStudio
{
public:
  ImageStudio(unsigned int nLEDs);
  ~ImageStudio();

  Animation createImageAnimation(const std::string& filename);

private:
  std::vector<std::vector<Color> > loadImageFromFile(const std::string& filename);

private:
  int m_nLEDs;
  int m_width;
  int m_height;
};

#endif // IMAGESTUDIO_H
