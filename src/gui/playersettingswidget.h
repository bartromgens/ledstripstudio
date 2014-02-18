#ifndef PLAYERSETTINGSWIDGET_H
#define PLAYERSETTINGSWIDGET_H

#include <QWidget>

#include <memory>

class ControlSettings;

namespace Ui {
class PlayerSettingsWidget;
}

class PlayerSettingsWidget : public QWidget
{
  Q_OBJECT
  
public:
  explicit PlayerSettingsWidget(QWidget *parent = 0);
  ~PlayerSettingsWidget();

  void setSettings(ControlSettings* settings);

public slots:
  void slotOffsetChanged(int value);
  void slotBrightnessChanged(int value);

private:
  void connectAllSlots() const;

private:
  Ui::PlayerSettingsWidget* ui;

  ControlSettings* m_settings;
};

#endif // PLAYERSETTINGSWIDGET_H
