#include "controlsettings.h"

#include "settings/configurationsavevisitor.h"

ControlSettings::ControlSettings()
  : QSettings(),
    ConfigurationState(),
    positionOffest(-15),
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
    m_mutex(),
    m_isActive(false)
{
}


ControlSettings::~ControlSettings()
{
}


void
ControlSettings::saveSettings()
{
  std::lock_guard<std::mutex> locker(m_mutex);

  // volume settings
  setValue("volumeTotal", volumeTotal);
  setValue("volumeRed", volumeRed);
  setValue("volumeGreen", volumeGreen);
  setValue("volumeBlue", volumeBlue);

  // frequency settings
  setValue("freqRedMin", freqRedMin);
  setValue("freqRedMax", freqRedMax);
  setValue("freqGreenMin", freqGreenMin);
  setValue("freqGreenMax", freqGreenMax);
  setValue("freqBlueMin", freqBlueMin);
  setValue("freqBlueMax", freqBlueMax);
}


void
ControlSettings::loadSettings()
{
  std::lock_guard<std::mutex> locker(m_mutex);

  // volume settings
  volumeTotal = value("volumeTotal", "").toInt();
  volumeRed = value("volumeRed", "").toInt();
  volumeGreen = value("volumeGreen", "").toInt();
  volumeBlue = value("volumeBlue", "").toInt();

  // frequency settings
  freqRedMin = value("freqRedMin", "").toInt();
  freqRedMax = value("freqRedMax", "").toInt();
  freqGreenMin = value("freqGreenMin", "").toInt();
  freqGreenMax = value("freqGreenMax", "").toInt();
  freqBlueMin = value("freqBlueMin", "").toInt();
  freqBlueMax = value("freqBlueMax", "").toInt();
}


void
ControlSettings::lock()
{
  m_mutex.lock();
}


bool
ControlSettings::try_lock()
{
  return m_mutex.try_lock();
}


void
ControlSettings::unlock()
{
  m_mutex.unlock();
}


void
ControlSettings::acceptSaver(ConfigurationSaveVisitor* visitor)
{
  visitor->saveMe(*this);
}


bool
ControlSettings::isActive() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_isActive;
}


void ControlSettings::setActive(bool isActive)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_isActive = isActive;
}

