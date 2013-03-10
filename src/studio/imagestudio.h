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

  Animation createImageAnimation(const std::string& filename) const;

  void createImageFromAnimation(const Animation& animation, const std::string& filename) const;

private:
  std::vector<std::vector<Color> > loadImageFromFile(const std::string& filename) const;
  static std::string getUniqueFilename(const std::string &filename, const std::string &extension);

private:
  int m_nLEDs;
};

#endif // IMAGESTUDIO_H
