#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic/animation.h"
#include "gui/tonetoolbar.h"
#include "gui/ffttoolbar.h"
#include "settings/configurationgroup.h"
#include "spectrum/spectrumobserver.h"
#include "spectrum/toneobserver.h"
#include "studio/tonestudio.h"

#include <QMainWindow>
#include <QToolBar>
#include <QColorDialog>
#include <QDockWidget>
#include <QTimer>

#include <boost/thread.hpp>
#include <thread>

#include <iostream>
#include <memory>

class AudioInput;
class Player;
class ToneAnalyser;
class SpectrumAnalyser;
class LedStripEmulator;

class Studio;
class ImageStudio;
class ToneStudio;
class SpectrumStudio;

class ControlSettings;
class LedStripStatusWidget;
class SpectrumSettingsWidget;
class PlayerSettingsWidget;

class QSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SpectrumObserver, public ToneObserver, public ConfigurationGroup
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();


  virtual void notifySpectrum(std::map<double, double> spectrum);
  virtual void notifyTone(std::map<std::string, double> toneAmplitudes);

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

  void updateLEDs(const std::map<double, double> &spectrum);

  Animation createAnimationTest1();
  Animation createAnimationTest2();

public slots:
  void slotPlayerPlayed();

  void slotSaveConfiguration(const QString& filename);
  void slotConfigComboChanged(QString comboText);

protected:
  virtual void closeEvent(QCloseEvent* event);

private slots:
  void update();

  void slotClearStrip();

  void slotToggleAudioInput(bool isChecked);

  void slotToggleSpectrumAnalysis(bool isChecked);
  void slotToggleToneAnalysis(bool isChecked);
  void slotToggleAnimation(bool isChecked);
  void slotToggleSingleColor(bool isChecked);

  void slotToggleSpectrumSettings(bool isChecked);

  void slotOpenColorPicker();
  void slotColorSelected(QColor color);

  void slotToggleDotAnimation(bool isChecked);
  void slotToggleRainbowAnimation(bool isChecked);
  void slotToggleImageAnimation(bool isChecked);

  void slotToggleRecording(bool isChecked);

private:
  // create GUI
  void createMenus();
  void createActions();
  void createToolbars();
  void connectAllSlots();
  void createTimers();

  // audio input
  void startAudioInputThread();
  void startAudioInput();
  void stopAudioInput();
  void startSpectrumAnalyser() const;
  void stopSpectrumAnalyser() const;
  void startToneAnalyser() const;
  void stopToneAnalyser() const;

  // animation
  void startAnimationThread() const;
  void startAnimation() const;
  void stopAnimation();
  void playDotAnimation();
  void startDotAnimationThread();

  // configuration
  void loadUserOrDefaultConfig();
  void loadConfigurationAll(QSettings& config);
  void saveConfigurationAll(QSettings& config) const;

private:
  Ui::MainWindow* ui;
  QColorDialog* m_colorDialog;
  std::size_t m_nLedsTotal;

  std::shared_ptr<ControlSettings> m_settings;
  std::shared_ptr<Player> m_player;
  std::unique_ptr<Studio> m_studio;
  std::shared_ptr<AudioInput> m_audioInput;
  std::shared_ptr<SpectrumAnalyser> m_spectrumAnalyser;
  std::shared_ptr<ToneAnalyser> m_toneAnalyser;
  std::unique_ptr<SpectrumStudio> m_spectrumStudio;
  std::shared_ptr<ToneStudio> m_toneStudio;
  std::unique_ptr<ImageStudio> m_imageStudio;

  std::vector<std::shared_ptr<ConfigurationGroup>> m_configurationGroups;

  SpectrumSettingsWidget* m_spectrumSettingsWidget;
  LedStripStatusWidget* m_ledStripStatusWidget;
  PlayerSettingsWidget* m_playerSettingsWidget;

  std::unique_ptr<boost::thread> m_audioInputThread;

  QToolBar* m_mainToolBar;
  QToolBar* m_detailsToolBar;

  ToneToolbar m_toneToolbar;
  FFTToolbar m_fftToolbar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* m_stripToggleButton;
  QAction* m_audioToggleButton;
  QAction* m_spectrumToggleButton;
  QAction* m_toneToggleButton;
  QAction* m_animationToggleAct;
  QAction* m_colorToggleAct;

  QAction* m_spectrumSettingsToggleAct;

  QAction* m_dotsAnimationAct;
  QAction* m_rainbowAnimationAct;
  QAction* m_imageAnimationAct;
  QAction* m_recordAnimationAct;
  QAction* m_openColorPickerAct;

  std::unique_ptr<QTimer> m_timer;
  std::unique_ptr<QTimer> m_timerEmulator;

  QColor m_lastSingleColor;

  static QString m_defaultConfigFilename;
  static QString m_userConfigFilename;
};

#endif // MAINWINDOW_H
