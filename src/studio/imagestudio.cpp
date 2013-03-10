#include "imagestudio.h"

#include <QImage>
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

  createImageFromAnimation(animation);

  return animation;
}


void
ImageStudio::createImageFromAnimation(const Animation& animation) const
{
  const std::deque<Frame>& frames = animation.getFrames();

  QImage image(m_nLEDs, frames.size(), QImage::Format_RGB32);

  for (std::size_t i = 0; i < frames.size(); ++i)
  {
    Frame frame = frames[i];
    const std::vector<LED>& leds = frame.getLEDs();
    for (std::size_t j = 0; j < leds.size(); ++j)
    {
      QColor qcolor = leds[j].getQColor();
      image.setPixel(int(j), int(i), qcolor.rgb());
    }
  }

  image.save("out.png");
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

