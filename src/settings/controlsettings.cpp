#include "controlsettings.h"

#include <iostream>

ControlSettings::ControlSettings()
  : m_mutex(),
    volumeTotal(0),
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
  m_mutex.lock();
  setValue("volumeTotal", volumeTotal);
  setValue("volumeRed", volumeRed);
  setValue("volumeGreen", volumeGreen);
  setValue("volumeBlue", volumeBlue);
  std::cout << "ControlSettings::saveSettings() - volumeTotal: " << volumeTotal << std::endl;
  m_mutex.unlock();
}


void
ControlSettings::loadSettings()
{
  m_mutex.lock();
  volumeTotal = value("volumeTotal", "").toInt();
  volumeRed = value("volumeRed", "").toInt();
  volumeGreen = value("volumeGreen", "").toInt();
  volumeBlue = value("volumeBlue", "").toInt();
  std::cout << "ControlSettings::loadSettings() - volumeTotal: " << volumeTotal << std::endl;
  m_mutex.unlock();
}
