#include "playersettingswidget.h"
#include "ui_playersettingswidget.h"

#include "settings/controlsettings.h"

#include <cassert>

PlayerSettingsWidget::PlayerSettingsWidget(std::shared_ptr<ControlSettings> settings, QWidget *parent) :
  QWidget(parent),
  ConfigurationGroup(),
  ui(new Ui::PlayerSettingsWidget),
  m_settings(settings)
{
  ui->setupUi(this);
  connectAllSlots();
}


PlayerSettingsWidget::~PlayerSettingsWidget()
{
  delete ui;
}


void
PlayerSettingsWidget::connectAllSlots() const
{
  connect( ui->offsetHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotOffsetChanged(int) ) );
  connect( ui->brightnessHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged(int) ) );
}


void
PlayerSettingsWidget::slotOffsetChanged(int value)
{
  assert(m_settings);
  m_settings->positionOffset = value;
}


void
PlayerSettingsWidget::slotBrightnessChanged(int value)
{
  assert(m_settings);
  m_settings->brightness = value;
}


void
PlayerSettingsWidget::saveConfiguration(QSettings& config) const
{
  config.beginGroup( "PlayerSettings" );

  config.setValue("positionOffset", static_cast<int>(ui->offsetHorizontalSlider->value()));
  config.setValue("brightness", static_cast<int>(ui->brightnessHorizontalSlider->value()));

  config.endGroup();
}


void
PlayerSettingsWidget::loadConfiguration(QSettings& config)
{
  config.beginGroup( "PlayerSettings" );

  m_settings->positionOffset = config.value("positionOffset", "").toInt();
  m_settings->brightness = config.value("brightness", "").toInt();
  ui->offsetHorizontalSlider->setValue(m_settings->positionOffset);
  ui->brightnessHorizontalSlider->setValue(m_settings->brightness);

  config.endGroup();
}
