#ifndef FFTTOOLBAR_H
#define FFTTOOLBAR_H

#include "settings/configurationgroup.h"

#include <QObject>

#include <memory>

class AudioInput;
class SpectrumAnalyser;

class QToolBar;
class QAction;

class FFTToolbar : public QObject, public ConfigurationGroup
{
  Q_OBJECT

public:
  FFTToolbar(AudioInput& audioInput, SpectrumAnalyser& spectrumAnalyser);
  ~FFTToolbar();

  void initialise(QToolBar* parentToolbar);

  void setVisible(bool isChecked);

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

private:

  enum FFTSampleSize
  {
    FFT14 = 0,
    FFT15 = 1,
    FFT16 = 2
  };

  enum WindowingFunctionType
  {
    Linear = 0,
    Hann = 1
  };

  void setNSamples(unsigned int nSamples);

private slots:
  void slotFFT14sizeAct(bool isChecked);
  void slotFFT15sizeAct(bool isChecked);
  void slotFFT16sizeAct(bool isChecked);
  void slotHannWindowingAct(bool isChecked);
  void slotLinearWindowingAct(bool isChecked);

private:
  AudioInput& m_audioInput;
  SpectrumAnalyser& m_spectrumAnalyser;

  QAction* m_FFT14sizeAct;
  QAction* m_FFT15sizeAct;
  QAction* m_FFT16sizeAct;
  QAction* m_hannWindowingAct;
  QAction* m_linearWindowingAct;
};

#endif // FFTTOOLBAR_H
