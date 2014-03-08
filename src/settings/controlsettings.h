#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <QSettings>

#include <atomic>
#include <mutex>
#include <vector>

class ControlSettings
{
public:
  ControlSettings();
  ~ControlSettings();

  bool isActive() const;
  void setActive(bool isActive);

public:
  std::atomic<int> positionOffset;
  std::atomic<int> brightness;

  std::atomic<int> volumeTotal;
  std::atomic<int> volumeRed;
  std::atomic<int> volumeGreen;
  std::atomic<int> volumeBlue;

  std::atomic<int> freqRedMin;
  std::atomic<int> freqRedMax;
  std::atomic<int> freqGreenMin;
  std::atomic<int> freqGreenMax;
  std::atomic<int> freqBlueMin;
  std::atomic<int> freqBlueMax;

private:
  std::atomic<bool> m_isActive;
};

#endif // CONTROLSETTINGS_H
