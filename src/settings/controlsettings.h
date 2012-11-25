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
  void setStatusFPS(int fps);
  int getStatusFPS();

  void lock();
  bool try_lock();
  void unlock();

public:
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
  std::mutex m_mutex;
  int statusFPS;
};

#endif // CONTROLSETTINGS_H
