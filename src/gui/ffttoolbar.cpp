#include "ffttoolbar.h"

#include "audioinput/audioinput.h"
#include "spectrum/spectrumanalyser.h"

#include <QToolBar>
#include <QAction>

FFTToolbar::FFTToolbar(std::shared_ptr<AudioInput> audioInput, std::shared_ptr<SpectrumAnalyser> spectrumAnalyser)
  : QObject(),
    m_audioInput(audioInput),
    m_spectrumAnalyser(spectrumAnalyser)
{
}


FFTToolbar::~FFTToolbar()
{
}


void
FFTToolbar::initialise(QToolBar* parentToolbar)
{
  m_FFT14sizeAct = new QAction(parentToolbar);
  m_FFT14sizeAct->setIcon(QIcon("./icons/fft14.png"));
  m_FFT14sizeAct->setStatusTip(tr("Set FFT sample size to 16k."));
  m_FFT14sizeAct->setCheckable(true);
  connect(m_FFT14sizeAct, SIGNAL(toggled(bool)), this, SLOT(slotFFT14sizeAct(bool)));

  m_FFT15sizeAct = new QAction(parentToolbar);
  m_FFT15sizeAct->setIcon(QIcon("./icons/fft15.png"));
  m_FFT15sizeAct->setStatusTip(tr("Set FFT sample size to 32k."));
  m_FFT15sizeAct->setCheckable(true);
  connect(m_FFT15sizeAct, SIGNAL(toggled(bool)), this, SLOT(slotFFT15sizeAct(bool)));

  m_FFT16sizeAct = new QAction(parentToolbar);
  m_FFT16sizeAct->setIcon(QIcon("./icons/fft16.png"));
  m_FFT16sizeAct->setStatusTip(tr("Set FFT sample size to 64k."));
  m_FFT16sizeAct->setCheckable(true);
  connect(m_FFT16sizeAct, SIGNAL(toggled(bool)), this, SLOT(slotFFT16sizeAct(bool)));

  m_hannWindowingAct = new QAction(parentToolbar);
  m_hannWindowingAct->setIcon(QIcon("./icons/hann-windowing.png"));
  m_hannWindowingAct->setStatusTip(tr("Set Hann windowing function."));
  m_hannWindowingAct->setCheckable(true);
  connect(m_hannWindowingAct, SIGNAL(toggled(bool)), this, SLOT(slotHannWindowingAct(bool)));

  m_linearWindowingAct = new QAction(parentToolbar);
  m_linearWindowingAct->setIcon(QIcon("./icons/linear-windowing.png"));
  m_linearWindowingAct->setStatusTip(tr("Set linear windowing function."));
  m_linearWindowingAct->setCheckable(true);
  connect(m_linearWindowingAct, SIGNAL(toggled(bool)), this, SLOT(slotLinearWindowingAct(bool)));

  parentToolbar->addAction(m_FFT14sizeAct);
  parentToolbar->addAction(m_FFT15sizeAct);
  parentToolbar->addAction(m_FFT16sizeAct);
  parentToolbar->addAction(m_linearWindowingAct);
  parentToolbar->addAction(m_hannWindowingAct);
}


void
FFTToolbar::toggleFFTSettings(bool isChecked)
{
  m_FFT14sizeAct->setVisible(isChecked);
  m_FFT15sizeAct->setVisible(isChecked);
  m_FFT16sizeAct->setVisible(isChecked);

  m_linearWindowingAct->setVisible(isChecked);
  m_hannWindowingAct->setVisible(isChecked);
}


void
FFTToolbar::setNSamples(unsigned int nSamples)
{
  m_audioInput->setNSamples(nSamples);
  m_spectrumAnalyser->setNSamples(nSamples);
}


void
FFTToolbar::slotFFT14sizeAct(bool isChecked)
{
  if (isChecked)
  {
    m_FFT15sizeAct->setChecked(false);
    m_FFT16sizeAct->setChecked(false);
    int samples = static_cast<int>(std::pow(2.0, 14));
    setNSamples(samples);
  }
}


void
FFTToolbar::slotFFT15sizeAct(bool isChecked)
{
  if (isChecked)
  {
    m_FFT14sizeAct->setChecked(false);
    m_FFT16sizeAct->setChecked(false);
    int samples = static_cast<int>(std::pow(2.0, 15));
    setNSamples(samples);
  }
}


void
FFTToolbar::slotFFT16sizeAct(bool isChecked)
{
  if (isChecked)
  {
    m_FFT14sizeAct->setChecked(false);
    m_FFT15sizeAct->setChecked(false);
    int samples = static_cast<int>(std::pow(2.0, 16));
    setNSamples(samples);
  }
}


void
FFTToolbar::slotHannWindowingAct(bool isChecked)
{
  if (isChecked)
  {
    m_linearWindowingAct->setChecked(false);
    m_spectrumAnalyser->setWindowingType(SpectrumAnalyser::hann);
  }
}


void
FFTToolbar::slotLinearWindowingAct(bool isChecked)
{
  if (isChecked)
  {
    m_hannWindowingAct->setChecked(false);
    m_spectrumAnalyser->setWindowingType(SpectrumAnalyser::linear);
  }
}
