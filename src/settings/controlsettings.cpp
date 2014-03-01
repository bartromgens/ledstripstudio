#include "controlsettings.h"

ControlSettings::ControlSettings()
  : ConfigurationGroup(),
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
ControlSettings::saveConfiguration(QSettings& configuration)
{
  std::lock_guard<std::mutex> locker(m_mutex);

  configuration.beginGroup( "ControlSettings" );

  // volume settings
  configuration.setValue("volumeTotal", volumeTotal);
  configuration.setValue("volumeRed", volumeRed);
  configuration.setValue("volumeGreen", volumeGreen);
  configuration.setValue("volumeBlue", volumeBlue);

  // frequency settings
  configuration.setValue("freqRedMin", freqRedMin);
  configuration.setValue("freqRedMax", freqRedMax);
  configuration.setValue("freqGreenMin", freqGreenMin);
  configuration.setValue("freqGreenMax", freqGreenMax);
  configuration. setValue("freqBlueMin", freqBlueMin);
  configuration.setValue("freqBlueMax", freqBlueMax);

  configuration.endGroup();
}


void
ControlSettings::loadConfiguration(QSettings& configuration)
{
  std::lock_guard<std::mutex> locker(m_mutex);

  configuration.beginGroup( "ControlSettings" );

  // volume settings
  volumeTotal = configuration.value("volumeTotal", "").toInt();
  volumeRed = configuration.value("volumeRed", "").toInt();
  volumeGreen = configuration.value("volumeGreen", "").toInt();
  volumeBlue = configuration.value("volumeBlue", "").toInt();

  // frequency settings
  freqRedMin = configuration.value("freqRedMin", "").toInt();
  freqRedMax = configuration.value("freqRedMax", "").toInt();
  freqGreenMin = configuration.value("freqGreenMin", "").toInt();
  freqGreenMax = configuration.value("freqGreenMax", "").toInt();
  freqBlueMin = configuration.value("freqBlueMin", "").toInt();
  freqBlueMax = configuration.value("freqBlueMax", "").toInt();

  configuration.endGroup();
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

