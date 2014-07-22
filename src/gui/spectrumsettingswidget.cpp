#include "spectrumsettingswidget.h"
#include "ui_spectrumsettingswidget.h"

#include "settings/controlsettings.h"

#include <iostream>

SpectrumSettingsWidget::SpectrumSettingsWidget(ControlSettings& spectrumSettings, QWidget *parent) :
  QWidget(parent),
  ConfigurationGroup(),
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
SpectrumSettingsWidget::slotVolumeChanged()
{
  m_settings.volumeTotal = ui->volumeTotalSlider->value();
  m_settings.volumeRed = ui->volumeRedSlider->value();
  m_settings.volumeGreen = ui->volumeGreenSlider->value();
  m_settings.volumeBlue = ui->volumeBlueSlider->value();
}


void
SpectrumSettingsWidget::slotFrequencyChanged()
{
  m_settings.freqRedMin = ui->freqRminSpin->value();
  m_settings.freqRedMax = ui->freqRmaxSpin->value();
  m_settings.freqGreenMin = ui->freqGminSpin->value();
  m_settings.freqGreenMax = ui->freqGmaxSpin->value();
  m_settings.freqBlueMin = ui->freqBminSpin->value();
  m_settings.freqBlueMax = ui->freqBmaxSpin->value();
}


void
SpectrumSettingsWidget::saveConfiguration(QSettings& config) const
{
  config.beginGroup( "SpectrumSettings" );

  // volume settings
  config.setValue("volumeTotal", ui->volumeTotalSlider->value());
  config.setValue("volumeRed", ui->volumeRedSlider->value());
  config.setValue("volumeGreen", ui->volumeGreenSlider->value());
  config.setValue("volumeBlue", ui->volumeBlueSlider->value());

  // frequency settings
  config.setValue("freqRedMin", ui->freqRminSpin->value());
  config.setValue("freqRedMax", ui->freqRmaxSpin->value());
  config.setValue("freqGreenMin", ui->freqGminSpin->value());
  config.setValue("freqGreenMax", ui->freqGmaxSpin->value());
  config.setValue("freqBlueMin", ui->freqBminSpin->value());
  config.setValue("freqBlueMax", ui->freqBmaxSpin->value());

  config.endGroup();
}


void
SpectrumSettingsWidget::loadConfiguration(QSettings& config)
{
  config.beginGroup( "SpectrumSettings" );

  ui->volumeTotalSlider->setValue(config.value("volumeTotal", "").toInt());
  ui->volumeRedSlider->setValue(config.value("volumeRed", "").toInt());
  ui->volumeGreenSlider->setValue(config.value("volumeGreen", "").toInt());
  ui->volumeBlueSlider->setValue(config.value("volumeBlue", "").toInt());

  ui->freqRminSlider->setValue(config.value("freqRedMin", "").toInt());
  ui->freqRmaxSlider->setValue(config.value("freqRedMax", "").toInt());
  ui->freqGminSlider->setValue(config.value("freqGreenMin", "").toInt());
  ui->freqGmaxSlider->setValue(config.value("freqGreenMax", "").toInt());
  ui->freqBminSlider->setValue(config.value("freqBlueMin", "").toInt());
  ui->freqBmaxSlider->setValue(config.value("freqBlueMax", "").toInt());

  config.endGroup();
}
