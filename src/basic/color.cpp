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
  double factor = 127.0 / 255.0;
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
  return Color(rand() % 127, rand() % 127, rand() % 127);
}


void
Color::setBrightness(double brightness)
{
  assert(brightness >= 0.0);
  unsigned int brightnessInRange = std::min(static_cast<int>(127 * brightness), 127);
  r *= brightnessInRange/127.0;
  g *= brightnessInRange/127.0;
  b *= brightnessInRange/127.0;
}
