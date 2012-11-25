#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <QSettings>

#include <vector>
#include <mutex>

class ControlSettings : public QSettings
{

public:
  ControlSettings();
  ~ControlSettings();

  void saveSettings();
  void loadSettings();

public:
  std::mutex m_mutex;
  int volumeTotal;
  int volumeRed;
  int volumeGreen;
  int volumeBlue;
};

#endif // CONTROLSETTINGS_H
