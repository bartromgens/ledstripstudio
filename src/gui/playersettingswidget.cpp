#include "playersettingswidget.h"
#include "ui_playersettingswidget.h"

#include "settings/controlsettings.h"

PlayerSettingsWidget::PlayerSettingsWidget(std::shared_ptr<ControlSettings> controlSettings, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PlayerSettingsWidget),
  m_settings(controlSettings)
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
  connect( ui->offsetHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotOffsetChanged() ) );
}


void
PlayerSettingsWidget::slotOffsetChanged()
{
  m_settings->positionOffest =  ui->offsetHorizontalSlider->value();
}
