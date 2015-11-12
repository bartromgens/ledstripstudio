#ifndef APPLICATIONSETTINGSDIALOG_H
#define APPLICATIONSETTINGSDIALOG_H

#include "settings/configurationgroup.h"

#include <QDialog>


namespace Ui {
class ApplicationSettingsDialog;
}

class ApplicationSettingsDialog : public QDialog, public ConfigurationGroup
{
  Q_OBJECT

public:
  explicit ApplicationSettingsDialog(QWidget *parent = 0);
  ~ApplicationSettingsDialog();

  virtual void saveConfiguration(QSettings& configuration) const;
  virtual void loadConfiguration(QSettings& configuration);

private:
  Ui::ApplicationSettingsDialog *ui;
};

#endif // APPLICATIONSETTINGSDIALOG_H
