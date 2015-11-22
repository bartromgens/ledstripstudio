#ifndef FFTTOOLBAR_H
#define FFTTOOLBAR_H

#include "settings/configurationgroup.h"

#include <QToolBar>

#include <memory>

class AudioInput;
class SpectrumAnalyser;

class QAction;
class QActionGroup;
class QComboBox;
class QToolBar;

class FFTToolbar : public QToolBar, public ConfigurationGroup
{
  Q_OBJECT

public:

  FFTToolbar(AudioInput& audioInput, SpectrumAnalyser& spectrumAnalyser, QWidget* parent);
  virtual ~FFTToolbar();


//  void setVisible(bool isChecked);

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

private:

  enum class WindowingFunctionType
  {
    Linear = 0,
    Hann = 1
  };

  void initialise();

  void setSampleSize(unsigned int nSamples);

private slots:

  void slotSampleSizeChanged();
  void slotWindowingAct();

private:

  AudioInput& m_audioInput;
  SpectrumAnalyser& m_spectrumAnalyser;

  QComboBox* m_sampleSizeCombo;

  QAction* m_hannWindowingAct;
  QAction* m_linearWindowingAct;
};

#endif // FFTTOOLBAR_H
