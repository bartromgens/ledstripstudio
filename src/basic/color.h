#ifndef COLOR_H
#define COLOR_H

struct Color
{
  Color(int r = 0, int g = 0, int b = 0);

  int r;
  int g;
  int b;

public:
  void print() const;
  static Color randomColor();
};

#endif // COLOR_H
