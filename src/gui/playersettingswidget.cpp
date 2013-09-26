#include "playersettingswidget.h"
#include "ui_playersettingswidget.h"

#include "settings/controlsettings.h"

#include <cassert>

PlayerSettingsWidget::PlayerSettingsWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PlayerSettingsWidget),
  m_settings(0)
{
  ui->setupUi(this);
  connectAllSlots();
}


PlayerSettingsWidget::~PlayerSettingsWidget()
{
  delete ui;
}


void
PlayerSettingsWidget::setSettings(ControlSettings* settings)
{
  m_settings = settings;
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
  m_settings->positionOffest = value;
}


void
PlayerSettingsWidget::slotBrightnessChanged(int value)
{
  assert(m_settings);
  m_settings->brightness = value;
}
