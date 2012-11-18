#ifndef LED_H
#define LED_H

#include "src/basic/color.h"

class LED
{
public:
  LED();

  const Color& getColor() const;
  int getLEDnr() const;

  void setColor(const Color &color);
  void setLEDnr(int letNr);

private:
  int m_ledNr;
  Color m_color;
};

#endif // LED_H
