#ifndef IMAGESTUDIO_H
#define IMAGESTUDIO_H

#include "basic/color.h"

#include <vector>
#include <string>

class ImageStudio
{
public:
  ImageStudio();
  ~ImageStudio();

  std::vector<std::vector<Color> > loadImageFromFile(std::string filename);

private:
  int m_width;
  int m_height;
};

#endif // IMAGESTUDIO_H
