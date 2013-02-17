#include "spectrumsettingswidget.h"
#include "ui_spectrumsettingswidget.h"

#include "settings/controlsettings.h"

SpectrumSettingsWidget::SpectrumSettingsWidget(std::shared_ptr<ControlSettings> spectrumSettings, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SpectrumSettingsWidget),
  m_settings(spectrumSettings)
{
  ui->setupUi(this);
  connectAllSlots();
}

SpectrumSettingsWidget::~SpectrumSettingsWidget()
{
  delete ui;
}

void
SpectrumSettingsWidget::connectAllSlots() const
{
  connect( ui->volumeTotalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );
  connect( ui->volumeRedSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );
  connect( ui->volumeGreenSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );
  connect( ui->volumeBlueSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );

  connect( ui->freqRmaxSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqRminSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqGmaxSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqGminSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqBmaxSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqBminSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );

  connect( ui->freqRminSlider, SIGNAL( valueChanged(int) ), ui->freqRminSpin, SLOT(setValue(int)) );
  connect( ui->freqRminSpin, SIGNAL( valueChanged(int) ), ui->freqRminSlider, SLOT(setValue(int)) );
  connect( ui->freqRmaxSlider, SIGNAL( valueChanged(int) ), ui->freqRmaxSpin, SLOT(setValue(int)) );
  connect( ui->freqRmaxSpin, SIGNAL( valueChanged(int) ), ui->freqRmaxSlider, SLOT(setValue(int)) );

  connect( ui->freqGminSlider, SIGNAL( valueChanged(int) ), ui->freqGminSpin, SLOT(setValue(int)) );
  connect( ui->freqGminSpin, SIGNAL( valueChanged(int) ), ui->freqGminSlider, SLOT(setValue(int)) );
  connect( ui->freqGmaxSlider, SIGNAL( valueChanged(int) ), ui->freqGmaxSpin, SLOT(setValue(int)) );
  connect( ui->freqGmaxSpin, SIGNAL( valueChanged(int) ), ui->freqGmaxSlider, SLOT(setValue(int)) );

  connect( ui->freqBminSlider, SIGNAL( valueChanged(int) ), ui->freqBminSpin, SLOT(setValue(int)) );
  connect( ui->freqBminSpin, SIGNAL( valueChanged(int) ), ui->freqBminSlider, SLOT(setValue(int)) );
  connect( ui->freqBmaxSlider, SIGNAL( valueChanged(int) ), ui->freqBmaxSpin, SLOT(setValue(int)) );
  connect( ui->freqBmaxSpin, SIGNAL( valueChanged(int) ), ui->freqBmaxSlider, SLOT(setValue(int)) );
}


void
SpectrumSettingsWidget::updateAudioControlGUI()
{
  m_settings->lock();
  ui->volumeTotalSlider->setValue(m_settings->volumeTotal);
  ui->volumeRedSlider->setValue(m_settings->volumeRed);
  ui->volumeGreenSlider->setValue(m_settings->volumeGreen);
  ui->volumeBlueSlider->setValue(m_settings->volumeBlue);

  ui->freqRminSlider->setValue(m_settings->freqRedMin);
  ui->freqRmaxSlider->setValue(m_settings->freqRedMax);
  ui->freqGminSlider->setValue(m_settings->freqGreenMin);
  ui->freqGmaxSlider->setValue(m_settings->freqGreenMax);
  ui->freqBminSlider->setValue(m_settings->freqBlueMin);
  ui->freqBmaxSlider->setValue(m_settings->freqBlueMax);
  m_settings->unlock();
}


void
SpectrumSettingsWidget::slotVolumeChanged()
{
  if (m_settings->try_lock())
  {
    m_settings->volumeTotal = ui->volumeTotalSlider->value();
    m_settings->volumeRed = ui->volumeRedSlider->value();
    m_settings->volumeGreen = ui->volumeGreenSlider->value();
    m_settings->volumeBlue = ui->volumeBlueSlider->value();

    m_settings->unlock();
  }
}


void
SpectrumSettingsWidget::slotFrequencyChanged()
{
  if (m_settings->try_lock())
  {
    m_settings->freqRedMin = ui->freqRminSpin->value();
    m_settings->freqRedMax = ui->freqRmaxSpin->value();
    m_settings->freqGreenMin = ui->freqGminSpin->value();
    m_settings->freqGreenMax = ui->freqGmaxSpin->value();
    m_settings->freqBlueMin = ui->freqBminSpin->value();
    m_settings->freqBlueMax = ui->freqBmaxSpin->value();

    m_settings->unlock();
  }
}
