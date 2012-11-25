#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <QSettings>

#include <vector>

class ControlSettings : public QSettings
{

public:
  ControlSettings();
  ~ControlSettings();

  void saveSettings();
  void loadSettings();

public:
  int volumeTotal;
  int volumeRed;
  int volumeGreen;
  int volumeBlue;
};

#endif // CONTROLSETTINGS_H
