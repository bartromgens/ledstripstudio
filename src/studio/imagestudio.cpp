#include "imagestudio.h"

#include <QImage>
#include <QColor>

ImageStudio::ImageStudio()
  : m_width(0),
    m_height(0)
{
}


ImageStudio::~ImageStudio()
{
}


std::vector<std::vector<Color> >
ImageStudio::loadImageFromFile(std::string filename)
{
  QImage image;
  image.load(QString::fromStdString(filename));

  m_width = image.width();
  m_height = image.height();

  std::vector<std::vector<Color> > colorMatrix;

  for (int i = 0; i < m_height; ++i)
  {
    std::vector<Color> colorRow;
    for (int j = 0; j < m_width; ++j)
    {
      QColor c = QColor::fromRgb(image.pixel(j,i) );
      Color color(c.red() * 127.0 / 256.0, c.green() * 127.0 / 256.0, c.blue() * 127.0 / 256.0);
      colorRow.push_back(color);
    }
    colorMatrix.push_back(colorRow);
  }

  return colorMatrix;
}
