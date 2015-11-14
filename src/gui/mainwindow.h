#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic/animation.h"
#include "gui/actionconsistency.h"
#include "gui/applicationsettingsdialog.h"
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
#include <QSlider>
#include <QTimer>

#include <boost/thread.hpp>
#include <thread>

#include <iostream>
#include <memory>

class AudioInput;
class BeatAnalyser;
class SpectrumAnalyser;
class ToneAnalyser;
class Player;

class Studio;
class ImageStudio;
class ToneStudio;
class SpectrumStudio;

class ControlSettings;
class LedStripStatusWidget;
class SpectrumSettingsWidget;
class PlayerSettingsWidget;
class LedStripEmulator;

class QSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SpectrumObserver, public ToneObserver, public ConfigurationGroup
{
  Q_OBJECT

public:

  explicit MainWindow(QWidget* parent = 0);
  virtual ~MainWindow();

  virtual void notifySpectrum(const std::vector<std::pair<double, double>>& spectrum);
  virtual void notifyTone(const std::map<Tone, double>& toneAmplitudes);

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

  Animation createAnimationTest1();
  Animation createAnimationTest2();

  void createConfigurationComboBox();

public slots:

  void slotPlayerPlayed();

  void slotSaveConfiguration(const QString& filename);
  void slotConfigComboChanged(QString comboText);

  void slotSaveConfigurationAs();

protected:

  virtual void closeEvent(QCloseEvent* event);

private slots:

  void update();

  void slotClearStrip();

  void slotToggleAudioInput(bool isChecked);

  void slotToggleBeatAnalysis(bool isChecked);
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
  void slotShowSettingsDialog();

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
  void startBeatAnalyser() const;
  void stopBeatAnalyser() const;
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
  const std::size_t m_nLedsTotal;

  std::unique_ptr<ControlSettings> m_settings;
  std::unique_ptr<Player> m_player;
  std::unique_ptr<AudioInput> m_audioInput;
  std::unique_ptr<Studio> m_studio;
  std::unique_ptr<BeatAnalyser> m_beatAnalyser;
  std::unique_ptr<SpectrumAnalyser> m_spectrumAnalyser;
  std::unique_ptr<ToneAnalyser> m_toneAnalyser;
  std::unique_ptr<SpectrumStudio> m_spectrumStudio;
  std::unique_ptr<ToneStudio> m_toneStudio;
  std::unique_ptr<ImageStudio> m_imageStudio;

  std::vector<ConfigurationGroup*> m_configurationGroups;

  SpectrumSettingsWidget* m_spectrumSettingsWidget;
  LedStripStatusWidget* m_ledStripStatusWidget;
  PlayerSettingsWidget* m_playerSettingsWidget;

  std::unique_ptr<boost::thread> m_audioInputThread;

  QToolBar* m_mainToolBar;
  QToolBar* m_detailsToolBar;

  ToneToolbar* m_toneToolbar;
  FFTToolbar* m_fftToolbar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* m_stripToggleButton;
  QAction* m_audioToggleButton;
  QAction* m_beatToggleButton;
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

  QAction* m_applicationSettingsAct;
  ApplicationSettingsDialog* m_applicationSettingsDialog;

  ActionConsistency* m_actionConsistency;

  std::unique_ptr<QTimer> m_timer;
  std::unique_ptr<QTimer> m_timerEmulator;

  QColor m_lastSingleColor;

  static QString m_defaultConfigFilename;
  static QString m_userConfigFilename;
};

#endif // MAINWINDOW_H
