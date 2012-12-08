#ifndef FRAME_H
#define FRAME_H

#include "led.h"

#include <map>
#include <vector>
#include <iostream>

class Frame
{
public:
  Frame(int nLEDs);
  ~Frame();

  const std::vector<LED>& getLEDs() const;

  void addLED(const LED& led);

  void print();

  void setAllLedsOff();

  void amplifyRGB(double facR, double facG, double facB);

private:
  int m_nLEDs;
  std::vector<LED> m_leds;
};

#endif // FRAME_H
