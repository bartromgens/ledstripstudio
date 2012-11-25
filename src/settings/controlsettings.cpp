#include "controlsettings.h"

#include <iostream>

ControlSettings::ControlSettings()
  : volumeTotal(0),
    volumeRed(0),
    volumeGreen(0),
    volumeBlue(0)
{
}


ControlSettings::~ControlSettings()
{
}


void
ControlSettings::saveSettings()
{
  setValue("volumeTotal", volumeTotal);
  setValue("volumeRed", volumeRed);
  setValue("volumeGreen", volumeGreen);
  setValue("volumeBlue", volumeBlue);
  std::cout << "ControlSettings::saveSettings() - volumeTotal: " << volumeTotal << std::endl;
}


void
ControlSettings::loadSettings()
{
  volumeTotal = value("volumeTotal", "").toInt();
  volumeRed = value("volumeRed", "").toInt();
  volumeGreen = value("volumeGreen", "").toInt();
  volumeBlue = value("volumeBlue", "").toInt();
  std::cout << "ControlSettings::loadSettings() - volumeTotal: " << volumeTotal << std::endl;
}
