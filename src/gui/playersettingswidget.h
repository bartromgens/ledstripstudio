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
  explicit PlayerSettingsWidget(std::shared_ptr<ControlSettings> controlSettings, QWidget *parent = 0);
  ~PlayerSettingsWidget();
  
private:
  void connectAllSlots() const;

private slots:
  void slotOffsetChanged();

private:
  Ui::PlayerSettingsWidget* ui;

  std::shared_ptr<ControlSettings> m_settings;
};

#endif // PLAYERSETTINGSWIDGET_H
