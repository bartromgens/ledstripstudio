#ifndef LED_H
#define LED_H

#include "basic/color.h"

class LED
{
public:
  LED(int ledNr, const Color& color);

  const Color& getColor() const;
  int getLEDnr() const;

  void setColor(const Color &color);
  void setLEDnr(int letNr);

  void print() const;

private:
  int m_ledNr;
  Color m_color;
};

#endif // LED_H
