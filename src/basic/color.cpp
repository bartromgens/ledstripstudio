#include "color.h"

#include <cassert>
#include <iostream>
#include <random>


Color::Color(int r, int g, int b)
: r(r),
  g(g),
  b(b)
{
}


Color::Color(const QColor& qcolor)
{
  double factor = 255.0 / 255.0;
  r = qcolor.red() * factor;
  g = qcolor.green() * factor;
  b = qcolor.blue() * factor;
}


void
Color::print() const
{
  std::cout << "(" << r << ", " << g << ", " << b << ")" << std::endl;
}


Color
Color::randomColor()
{
  return Color(rand() % 255, rand() % 255, rand() % 255);
}


void
Color::setBrightness(double brightness)
{
  // convert to LED brightness (non-linear)
  // use a logistic function (https://en.wikipedia.org/wiki/Logistic_function)
  brightness = 1.0 / ( 1.0 + std::exp( (brightness*10.0-6) * -1.0 ) );  // based on emprical model http://electronics.stackexchange.com/a/11100
  assert(brightness >= 0.0);
  unsigned int brightnessInRange = std::min(static_cast<int>(255 * brightness), 255);
  r *= brightnessInRange/255.0;
  g *= brightnessInRange/255.0;
  b *= brightnessInRange/255.0;
}
