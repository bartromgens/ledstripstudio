#include "imagestudio.h"

#include <iostream>

#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QColor>


ImageStudio::ImageStudio(unsigned int nLEDs)
  : m_nLEDs(nLEDs)
{
}


ImageStudio::~ImageStudio()
{
}


Animation
ImageStudio::createImageAnimation(const std::string& filename) const
{
  Animation animation;

  std::vector<std::vector<Color> > colorMatrix = loadImageFromFile(filename);

  for (std::size_t i = 0; i < colorMatrix.size(); ++i)
  {
    Frame frame(m_nLEDs);
    for (std::size_t j = 0; j < colorMatrix[i].size(); ++j)
    {
      Color color = colorMatrix[i][j];
      LED led(j, color);
      frame.addLED(led);
    }
    animation.addFrame(frame);
  }

  createImageFromAnimation(animation, "test");

  return animation;
}


void
ImageStudio::createImageFromAnimation(const Animation& animation, const std::string& filename) const
{
  const std::deque<Frame>& frames = animation.getFrames();

  // landscape image, height is number of LEDs
  QImage image(frames.size(), m_nLEDs, QImage::Format_RGB32);

  for (std::size_t i = 0; i < frames.size(); ++i)
  {
    Frame frame = frames[i];
    const std::vector<LED>& leds = frame.getLEDs();
    for (std::size_t j = 0; j < leds.size(); ++j)
    {
      QColor qcolor = leds[j].getQColor();
      image.setPixel(int(i), int(j), qcolor.rgb()); // x = i, y = j (not matrix index notation)
    }
  }

  std::string extension = ".png";
  std::string uniqueFilename = getUniqueFilename(filename, extension);

  image.save(QString::fromStdString(uniqueFilename + extension));
}


std::vector<std::vector<Color> >
ImageStudio::loadImageFromFile(const std::string& filename) const
{
  QImage image;
  image.load(QString::fromStdString(filename));
  image = image.scaledToWidth(m_nLEDs);

  unsigned int width = image.width();
   unsigned int height = image.height();

  std::vector<std::vector<Color> > colorMatrix;

  for (std::size_t i = 0; i < height; ++i)
  {
    std::vector<Color> colorRow;
    for (std::size_t j = 0; j < width; ++j)
    {
      QColor qcolor = QColor::fromRgb(image.pixel(j,i) );
      Color color(qcolor);
      colorRow.push_back(color);
    }
    colorMatrix.push_back(colorRow);
  }

  return colorMatrix;
}


std::string
ImageStudio::getUniqueFilename(const std::string& filename, const std::string& extension)
{
  unsigned int counter = 1;

  std::string uniqueFilename = filename  + std::to_string(counter);
  QFile file(QString::fromStdString(uniqueFilename + extension));
  QFileInfo fileInfo(file);

  while (fileInfo.exists())
  {
    counter++;
    uniqueFilename = filename + std::to_string(counter);
    QFile file(QString::fromStdString(uniqueFilename + extension));
    fileInfo.setFile(file);
  }

  return uniqueFilename;
}
