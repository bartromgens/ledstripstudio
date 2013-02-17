#ifndef SPECTRUMSETTINGSWIDGET_H
#define SPECTRUMSETTINGSWIDGET_H

#include <memory>
#include <QWidget>

class ControlSettings;

namespace Ui {
class SpectrumSettingsWidget;
}

class SpectrumSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SpectrumSettingsWidget(std::shared_ptr<ControlSettings> spectrumSettings, QWidget *parent = 0);
  ~SpectrumSettingsWidget();

  void updateAudioControlGUI();

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
