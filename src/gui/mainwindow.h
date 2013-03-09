#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic/animation.h"
#include "spectrum/spectrumobserver.h"
#include "spectrum/toneobserver.h"
#include "studio/tonestudio.h"

#include <QMainWindow>
#include <QToolBar>
#include <QColorDialog>
#include <QDockWidget>
#include <QTimer>

#include <boost/thread.hpp>

#include <iostream>
#include <memory>

class AudioInput;
class ControlSettings;
class LedStripEmulator;
class Player;
class ToneAnalyser;
class ToneStudio;
class SpectrumAnalyser;
class SpectrumStudio;
class SpectrumSettingsWidget;
class Studio;

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

  void setNSamples(unsigned int nSamples);

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

  void slotFFT14sizeAct(bool isChecked);
  void slotFFT15sizeAct(bool isChecked);
  void slotFFT16sizeAct(bool isChecked);

  void slotShowSpetrumSettings();

  void slotToggleStepTone(bool isChecked);
  void slotToggleSmoothTone(bool isChecked);
  void slotToggleToneHistory(bool isChecked);
  void slotToggleIndividualTone(bool isChecked);
  void slotToggleCornerTone(bool isChecked);

  void slotOpenColorPicker();
  void slotColorSelected(QColor color);

  void slotToggleDotAnimation(bool isChecked);
  void slotToggleRainbowAnimation(bool isChecked);


private:
  void createMenus();
  void createActions();
  void setActionsDefaults();
  void createToolbars();
  void connectAllSlots() const;
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

  void updateToneSettingsVisibility(ToneStudio::ToneAnimationType type);

private:
  Ui::MainWindow* ui;
  QColorDialog* m_colorDialog;
  std::size_t m_nLedsTotal;

  std::shared_ptr<Player> m_player;
  std::unique_ptr<Studio> m_studio;
  std::unique_ptr<AudioInput> m_audioInput;
  std::shared_ptr<ControlSettings> m_settings;
  std::shared_ptr<SpectrumAnalyser> m_spectrumAnalyser;
  std::shared_ptr<ToneAnalyser> m_toneAnalyser;
  std::unique_ptr<SpectrumStudio> m_spectrumStudio;
  std::unique_ptr<ToneStudio> m_toneStudio;

  QDockWidget* m_spectrumSettingsDialog;
  SpectrumSettingsWidget* m_spectrumSettingsWidget;

  boost::thread* m_audioInputThread;

  QToolBar* m_mainToolBar;
  QToolBar* m_detailsToolBar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;
  QAction* m_audioToggleButton;
  QAction* m_spectrumToggleButton;
  QAction* m_toneToggleButton;
  QAction* m_animationToggleAct;
  QAction* m_colorToggleAct;

  QAction* m_FFT14sizeAct;
  QAction* m_FFT15sizeAct;
  QAction* m_FFT16sizeAct;

  QAction* m_openSpectrumSettingsAct;
  QAction* m_stepToneAct;
  QAction* m_smoothToneAct;
  QAction* m_historyToneAct;
  QAction* m_individualToneAct;
  QAction* m_cornerToneAct;
  QAction* m_dotsAnimationAct;
  QAction* m_rainbowAnimationAct;
  QAction* m_openColorPickerAct;

  QTimer* m_timer;
  QTimer* m_timerEmulator;

  QColor m_lastSingleColor;
};

#endif // MAINWINDOW_H
