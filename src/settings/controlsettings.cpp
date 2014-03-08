#include "controlsettings.h"

#include <iostream>

ControlSettings::ControlSettings()
  : positionOffset(-15),
    brightness(100),
    volumeTotal(0),
    volumeRed(0),
    volumeGreen(0),
    volumeBlue(0),
    freqRedMin(150),
    freqRedMax(220),
    freqGreenMin(220),
    freqGreenMax(440),
    freqBlueMin(440),
    freqBlueMax(2000),
    m_isActive(false)
{
}


ControlSettings::~ControlSettings()
{
}


bool
ControlSettings::isActive() const
{
  return m_isActive;
}


void ControlSettings::setActive(bool isActive)
{
  m_isActive = isActive;
}

