#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include "settings/configurationstate.h"

#include <QSettings>

#include <atomic>
#include <mutex>
#include <vector>

class ControlSettings : public QSettings, ConfigurationState
{

public:
  ControlSettings();
  ~ControlSettings();

  void saveSettings();
  void loadSettings();
  void setStatusFPS(int fps);
  int getStatusFPS() const;

  bool isActive() const;
  void setActive(bool isActive);

  void lock();
  bool try_lock();
  void unlock();

  virtual void acceptSaver(ConfigurationSaveVisitor* visitor);

public:
  std::atomic<int> positionOffest;
  std::atomic<int> brightness;

  int volumeTotal;
  int volumeRed;
  int volumeGreen;
  int volumeBlue;

  int freqRedMin;
  int freqRedMax;
  int freqGreenMin;
  int freqGreenMax;
  int freqBlueMin;
  int freqBlueMax;

private:
  mutable std::mutex m_mutex;
  bool m_isActive;
};

#endif // CONTROLSETTINGS_H
