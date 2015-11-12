#include "applicationsettingsdialog.h"
#include "ui_applicationsettingsdialog.h"

#include <QSettings>

ApplicationSettingsDialog::ApplicationSettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ApplicationSettingsDialog)
{
  ui->setupUi(this);
}


ApplicationSettingsDialog::~ApplicationSettingsDialog()
{
  delete ui;
}


void
ApplicationSettingsDialog::saveConfiguration(QSettings& config) const
{
  config.beginGroup("ApplicationSettings");

  config.setValue("nLeds", static_cast<int>(ui->nLedsSpinbox->value()));

  config.endGroup();
}


void
ApplicationSettingsDialog::loadConfiguration(QSettings& config)
{
  config.beginGroup("ApplicationSettings");

  int nLeds = config.value("nLeds", "").toInt();
  ui->nLedsSpinbox->setValue(nLeds);

  config.endGroup();
}
