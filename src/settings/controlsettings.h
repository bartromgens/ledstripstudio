#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <QSettings>

#include <atomic>
#include <mutex>
#include <vector>

class ControlSettings : public QSettings
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
  std::atomic<int> m_statusFPS;
  std::atomic<bool> m_isActive;
};

#endif // CONTROLSETTINGS_H
