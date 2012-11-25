#include "controlsettings.h"

#include <iostream>

ControlSettings::ControlSettings()
  : volumeTotal(0),
    volumeRed(0),
    volumeGreen(0),
    volumeBlue(0),
    freqRedMin(150),
    freqRedMax(220),
    freqGreenMin(220),
    freqGreenMax(440),
    freqBlueMin(440),
    freqBlueMax(2000),
    m_mutex(),
    statusFPS(0),
    isActive(false)
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

  setValue("freqRedMin", freqRedMin);
  setValue("freqRedMax", freqRedMax);
  setValue("freqGreenMin", freqGreenMin);
  setValue("freqGreenMax", freqGreenMax);
  setValue("freqBlueMin", freqBlueMin);
  setValue("freqBlueMax", freqBlueMax);
  std::cout << "ControlSettings::saveSettings() - freqBlueMax: " << freqBlueMax << std::endl;
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

  freqRedMin = value("freqRedMin", "").toInt();
  freqRedMax = value("freqRedMax", "").toInt();
  freqGreenMin = value("freqGreenMin", "").toInt();
  freqGreenMax = value("freqGreenMax", "").toInt();
  freqBlueMin = value("freqBlueMin", "").toInt();
  freqBlueMax = value("freqBlueMax", "").toInt();

  std::cout << "ControlSettings::loadSettings() - freqBlueMax: " << freqBlueMax << std::endl;
  m_mutex.unlock();
}


void
ControlSettings::setStatusFPS(int fps)
{
  m_mutex.lock();
  statusFPS = fps;
  m_mutex.unlock();
}


int
ControlSettings::getStatusFPS()
{
  m_mutex.lock();
  int fps = statusFPS;
  m_mutex.unlock();
  return fps;
}

void
ControlSettings::lock()
{
//  std::cout << "ControlSettings::lock()" << std::endl;
  m_mutex.lock();
}

bool
ControlSettings::try_lock()
{
//  std::cout << "ControlSettings::try_lock()" << std::endl;
  return m_mutex.try_lock();
}

void ControlSettings::unlock()
{
//  std::cout << "ControlSettings::lock()" << std::endl;
  m_mutex.unlock();
}

