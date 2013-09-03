#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic/animation.h"
#include "gui/tonetoolbar.h"
#include "gui/ffttoolbar.h"
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
class SpectrumSettingsWidget;
class PlayerSettingsWidget;

class BitcoinExchangeClient;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, SpectrumObserver, ToneObserver
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  Animation createAnimationTest1();
  Animation createAnimationTest2();

  virtual void notifySpectrum(std::map<double, double> spectrum);

  virtual void notifyTone(std::map<std::string, double> toneAmplitudes);

  void updateLEDs(const std::map<double, double> &spectrum);

  void getMtGox();

public slots:
  void slotPlayerPlayed();

protected:
  virtual void closeEvent(QCloseEvent* event);

private slots:
  void update();

  void slotToggleAudioInput(bool isChecked);

  void slotToggleSpectrumAnalysis(bool isChecked);
  void slotToggleToneAnalysis(bool isChecked);
  void slotToggleAnimation(bool isChecked);
  void slotToggleSingleColor(bool isChecked);

  void slotShowSpetrumSettings();

  void slotOpenColorPicker();
  void slotColorSelected(QColor color);

  void slotToggleDotAnimation(bool isChecked);
  void slotToggleRainbowAnimation(bool isChecked);
  void slotToggleImageAnimation(bool isChecked);

  void slotToggleRecording(bool isChecked);

private:
  void createMenus();
  void createActions();
  void setActionsDefaults();
  void createToolbars();
  void connectAllSlots();
  void createTimers();

  void startAudioInputThread();
  void startAnimationThread() const;
  void startAnimation() const;
  void stopAnimation();

  void startAudioInput();
  void stopAudioInput();
  void startSpectrumAnalyser() const;
  void stopSpectrumAnalyser() const;
  void startToneAnalyser() const;
  void stopToneAnalyser() const;

  void playDotAnimation();
  void startDotAnimationThread();

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

  std::unique_ptr<BitcoinExchangeClient> m_mtgoxState;

  QDockWidget* m_spectrumSettingsDialog;
  SpectrumSettingsWidget* m_spectrumSettingsWidget;

//  QDockWidget* m_playerSettingsDialog;
//  PlayerSettingsWidget* m_playerSettingsWidget;

  std::unique_ptr<boost::thread> m_audioInputThread;

  QToolBar* m_mainToolBar;
  QToolBar* m_detailsToolBar;

  ToneToolbar m_toneToolbar;
  FFTToolbar m_fftToolbar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* m_audioToggleButton;
  QAction* m_spectrumToggleButton;
  QAction* m_toneToggleButton;
  QAction* m_animationToggleAct;
  QAction* m_colorToggleAct;

  QAction* m_openSpectrumSettingsAct;

  QAction* m_dotsAnimationAct;
  QAction* m_rainbowAnimationAct;
  QAction* m_imageAnimationAct;
  QAction* m_recordAnimationAct;
  QAction* m_openColorPickerAct;

  std::unique_ptr<QTimer> m_timer;
  std::unique_ptr<QTimer> m_timerEmulator;

  QColor m_lastSingleColor;
};

#endif // MAINWINDOW_H
