#ifndef FFTTOOLBAR_H
#define FFTTOOLBAR_H

#include "settings/configurationgroup.h"

#include <QObject>

#include <memory>

class AudioInput;
class SpectrumAnalyser;

class QAction;
class QActionGroup;
class QToolBar;

class FFTToolbar : public QObject, public ConfigurationGroup
{
  Q_OBJECT

public:

  FFTToolbar(AudioInput& audioInput, SpectrumAnalyser& spectrumAnalyser);
  virtual ~FFTToolbar();

  void initialise(QToolBar* parentToolbar);

  void setVisible(bool isChecked);

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

private:

  enum class FFTSampleSize
  {
    FFT14 = 0,
    FFT15 = 1,
    FFT16 = 2
  };

  enum class WindowingFunctionType
  {
    Linear = 0,
    Hann = 1
  };

  void setNSamples(unsigned int nSamples);

private slots:

  void slotFFTsizeAct();
  void slotWindowingAct();

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
