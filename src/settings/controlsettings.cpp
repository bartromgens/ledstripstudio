#include "controlsettings.h"

ControlSettings::ControlSettings()
  : positionOffest(-15),
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
ControlSettings::saveSettings(QSettings& settings)
{
  std::lock_guard<std::mutex> locker(m_mutex);

  settings.beginGroup( "ControlSettings" );

  // volume settings
  settings.setValue("volumeTotal", volumeTotal);
  settings.setValue("volumeRed", volumeRed);
  settings.setValue("volumeGreen", volumeGreen);
  settings.setValue("volumeBlue", volumeBlue);

  // frequency settings
  settings.setValue("freqRedMin", freqRedMin);
  settings.setValue("freqRedMax", freqRedMax);
  settings.setValue("freqGreenMin", freqGreenMin);
  settings.setValue("freqGreenMax", freqGreenMax);
  settings. setValue("freqBlueMin", freqBlueMin);
  settings.setValue("freqBlueMax", freqBlueMax);

  settings.endGroup();
}


void
ControlSettings::loadSettings(QSettings& settings)
{
  std::lock_guard<std::mutex> locker(m_mutex);

  settings.beginGroup( "ControlSettings" );

  // volume settings
  volumeTotal = settings.value("volumeTotal", "").toInt();
  volumeRed = settings.value("volumeRed", "").toInt();
  volumeGreen = settings.value("volumeGreen", "").toInt();
  volumeBlue = settings.value("volumeBlue", "").toInt();

  // frequency settings
  freqRedMin = settings.value("freqRedMin", "").toInt();
  freqRedMax = settings.value("freqRedMax", "").toInt();
  freqGreenMin = settings.value("freqGreenMin", "").toInt();
  freqGreenMax = settings.value("freqGreenMax", "").toInt();
  freqBlueMin = settings.value("freqBlueMin", "").toInt();
  freqBlueMax = settings.value("freqBlueMax", "").toInt();

  settings.endGroup();
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

