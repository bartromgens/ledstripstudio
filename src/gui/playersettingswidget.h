#ifndef PLAYERSETTINGSWIDGET_H
#define PLAYERSETTINGSWIDGET_H

#include "settings/configurationgroup.h"

#include <QWidget>

#include <memory>

class ControlSettings;

namespace Ui {
class PlayerSettingsWidget;
}

class PlayerSettingsWidget : public QWidget, public ConfigurationGroup
{
  Q_OBJECT
  
public:
  explicit PlayerSettingsWidget(ControlSettings& settings, QWidget* parent = 0);
  virtual ~PlayerSettingsWidget();

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

public slots:
  void slotOffsetChanged(int value);
  void slotBrightnessChanged(int value);

private:
  void connectAllSlots() const;

private:
  Ui::PlayerSettingsWidget* ui;

  ControlSettings& m_settings;
};

#endif // PLAYERSETTINGSWIDGET_H
