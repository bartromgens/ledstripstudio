#include "color.h"

#include <iostream>

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
