#include "controlsettings.h"

ControlSettings::ControlSettings()
  : ConfigurationGroup(),
    positionOffset(-15),
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
ControlSettings::saveConfiguration(QSettings& config) const
{
  std::lock_guard<std::mutex> locker(m_mutex);

  config.beginGroup( "ControlSettings" );

  // volume settings
  config.setValue("volumeTotal", volumeTotal);
  config.setValue("volumeRed", volumeRed);
  config.setValue("volumeGreen", volumeGreen);
  config.setValue("volumeBlue", volumeBlue);

  // frequency settings
  config.setValue("freqRedMin", freqRedMin);
  config.setValue("freqRedMax", freqRedMax);
  config.setValue("freqGreenMin", freqGreenMin);
  config.setValue("freqGreenMax", freqGreenMax);
  config.setValue("freqBlueMin", freqBlueMin);
  config.setValue("freqBlueMax", freqBlueMax);

  config.endGroup();
}


void
ControlSettings::loadConfiguration(QSettings& config)
{
  std::lock_guard<std::mutex> locker(m_mutex);

  config.beginGroup( "ControlSettings" );

  // volume settings
  volumeTotal = config.value("volumeTotal", "").toInt();
  volumeRed = config.value("volumeRed", "").toInt();
  volumeGreen = config.value("volumeGreen", "").toInt();
  volumeBlue = config.value("volumeBlue", "").toInt();

  // frequency settings
  freqRedMin = config.value("freqRedMin", "").toInt();
  freqRedMax = config.value("freqRedMax", "").toInt();
  freqGreenMin = config.value("freqGreenMin", "").toInt();
  freqGreenMax = config.value("freqGreenMax", "").toInt();
  freqBlueMin = config.value("freqBlueMin", "").toInt();
  freqBlueMax = config.value("freqBlueMax", "").toInt();

  config.endGroup();
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

