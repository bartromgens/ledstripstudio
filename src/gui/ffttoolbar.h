#ifndef FFTTOOLBAR_H
#define FFTTOOLBAR_H

#include <QObject>

#include <memory>

class AudioInput;
class SpectrumAnalyser;

class QToolBar;
class QAction;

class FFTToolbar : public QObject
{
  Q_OBJECT

public:
  FFTToolbar(std::shared_ptr<AudioInput> audioInput, std::shared_ptr<SpectrumAnalyser> spectrumAnalyser);
  ~FFTToolbar();

  void initialise(QToolBar* parentToolbar);

  void toggleFFTSettings(bool isChecked);

private:
  void setNSamples(unsigned int nSamples);

private slots:
  void slotFFT14sizeAct(bool isChecked);
  void slotFFT15sizeAct(bool isChecked);
  void slotFFT16sizeAct(bool isChecked);
  void slotHannWindowingAct(bool isChecked);
  void slotLinearWindowingAct(bool isChecked);

private:
  std::shared_ptr<AudioInput> m_audioInput;
  std::shared_ptr<SpectrumAnalyser> m_spectrumAnalyser;

  QAction* m_FFT14sizeAct;
  QAction* m_FFT15sizeAct;
  QAction* m_FFT16sizeAct;
  QAction* m_hannWindowingAct;
  QAction* m_linearWindowingAct;
};

#endif // FFTTOOLBAR_H
