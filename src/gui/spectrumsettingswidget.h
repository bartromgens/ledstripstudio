#ifndef SPECTRUMSETTINGSWIDGET_H
#define SPECTRUMSETTINGSWIDGET_H

#include "settings/configurationgroup.h"

#include <QWidget>

#include <memory>

class ControlSettings;

namespace Ui {
class SpectrumSettingsWidget;
}

class SpectrumSettingsWidget : public QWidget, public ConfigurationGroup
{
  Q_OBJECT

public:
  explicit SpectrumSettingsWidget(std::shared_ptr<ControlSettings> spectrumSettings, QWidget *parent = 0);
  ~SpectrumSettingsWidget();

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

private:
  void connectAllSlots() const;

private slots:
  void slotFrequencyChanged();
  void slotVolumeChanged();

private:
  Ui::SpectrumSettingsWidget *ui;

  std::shared_ptr<ControlSettings> m_settings;
};

#endif // SPECTRUMSETTINGSWIDGET_H
