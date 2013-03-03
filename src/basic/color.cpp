#include "color.h"

#include <iostream>
#include <random>

Color::Color(int r, int g, int b)
  : r(r),
    g(g),
    b(b)
{
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
