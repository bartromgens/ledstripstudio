#ifndef FRAME_H
#define FRAME_H

#include "led.h"

#include <map>
#include <iostream>

class Frame
{
public:
  Frame(int nLEDs);

  const std::map<int, LED>& getLEDs() const;

  void addLED(LED led);

  void print();

  void setAllLedsOff();

  void amplifyRGB(double facR, double facG, double facB);

private:
  int m_nLEDs;
  std::map<int, LED> m_leds;
};

#endif // FRAME_H
