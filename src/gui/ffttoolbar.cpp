#include "ffttoolbar.h"

#include "audioinput/audioinput.h"
#include "spectrum/spectrumanalyser.h"

#include <QAction>
#include <QDebug>
#include <QComboBox>
#include <QLabel>
#include <QToolBar>


FFTToolbar::FFTToolbar(AudioInput& audioInput, SpectrumAnalyser& spectrumAnalyser, QWidget* parent)
: QToolBar(parent),
  ConfigurationGroup(),
  m_audioInput(audioInput),
  m_spectrumAnalyser(spectrumAnalyser)
{
  initialise();
}


FFTToolbar::~FFTToolbar()
{
}


void
FFTToolbar::initialise()
{
  const int minSampleSize = 11;
  const int maxSampleSize = 18;
  m_sampleSizeCombo = new QComboBox(this);
  for (int i = minSampleSize; i < maxSampleSize; i++)
  {
    int sampleSize = std::pow(2.0, i);
    m_sampleSizeCombo->addItem( QString::number(sampleSize), QVariant(sampleSize) );
  }
  m_sampleSizeCombo->setToolTip("Fourier transform sample size");

  connect(m_sampleSizeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSampleSizeChanged()));

  m_hannWindowingAct = new QAction(this);
  m_hannWindowingAct->setIcon(QIcon("./icons/hann-windowing.png"));
  m_hannWindowingAct->setStatusTip(tr("Set Hann windowing function."));
  m_hannWindowingAct->setCheckable(true);
  connect(m_hannWindowingAct, SIGNAL(toggled(bool)), this, SLOT(slotWindowingAct()));

  m_linearWindowingAct = new QAction(this);
  m_linearWindowingAct->setIcon(QIcon("./icons/linear-windowing.png"));
  m_linearWindowingAct->setStatusTip(tr("Set linear windowing function."));
  m_linearWindowingAct->setCheckable(true);
  connect(m_linearWindowingAct, SIGNAL(toggled(bool)), this, SLOT(slotWindowingAct()));

  QActionGroup* windowingGroup = new QActionGroup(this);
  windowingGroup->addAction(m_hannWindowingAct);
  windowingGroup->addAction(m_linearWindowingAct);

  addWidget(new QLabel("FFT "));
  addWidget(m_sampleSizeCombo);
  addWidget(new QLabel(" bit"));
  addSeparator();
  addAction(m_linearWindowingAct);
  addAction(m_hannWindowingAct);

  m_linearWindowingAct->setChecked(true);
}


void
FFTToolbar::setSampleSize(unsigned int nSamples)
{
  m_audioInput.setSampleSize(nSamples);
  m_spectrumAnalyser.setSampleSize(nSamples);
}


void
FFTToolbar::slotSampleSizeChanged()
{
  int nSamples = m_sampleSizeCombo->currentData().toInt();
  setSampleSize(nSamples);
}


void
FFTToolbar::slotWindowingAct()
{
  if (m_hannWindowingAct->isChecked())
  {
    m_spectrumAnalyser.setWindowingType(SpectrumAnalyser::hann);
  }
  else if (m_linearWindowingAct->isChecked())
  {
    m_spectrumAnalyser.setWindowingType(SpectrumAnalyser::linear);
  }
}


void
FFTToolbar::saveConfiguration(QSettings& config) const
{
  config.beginGroup( "FFT" );

  FFTToolbar::WindowingFunctionType windowFunc = WindowingFunctionType::Linear;
  if (m_linearWindowingAct->isChecked())
  {
    windowFunc = WindowingFunctionType::Linear;
  }
  else if (m_hannWindowingAct->isChecked())
  {
    windowFunc = WindowingFunctionType::Hann;
  }

  config.setValue("FFTSampleSize", static_cast<int>(m_sampleSizeCombo->currentData().toInt()));
  config.setValue("WindowingFunction", static_cast<int>(windowFunc));

  config.endGroup();
}


void
FFTToolbar::loadConfiguration(QSettings& config)
{
  config.beginGroup( "FFT" );

  int sampleSize = static_cast<int>(config.value("FFTSampleSize", "").toInt());
  WindowingFunctionType windowFunc =
      static_cast<WindowingFunctionType>(config.value("WindowingFunction", "").toInt());

  int comboIndex = m_sampleSizeCombo->findData(QVariant(sampleSize));

  if (comboIndex != -1)
  {
    m_sampleSizeCombo->setCurrentIndex(comboIndex);
  }

  switch (windowFunc)
  {
    case WindowingFunctionType::Hann:
    {
      m_hannWindowingAct->setChecked(true);
      break;
    }
    case WindowingFunctionType::Linear:
    {
      m_linearWindowingAct->setChecked(true);
      break;
    }
  }

  config.endGroup();
}
