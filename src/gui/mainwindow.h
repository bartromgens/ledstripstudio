#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic/animation.h"
#include "spectrum/spectrumobserver.h"
#include "spectrum/toneobserver.h"

#include <QMainWindow>
#include <QToolBar>
#include <QColorDialog>
#include <QTimer>

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

public slots:
  void slotPlayerPlayed();

protected:
  virtual void closeEvent(QCloseEvent* event);

private slots:
  void update();

  void slotToggleSingleColor(bool);
  void slotColorSelected(const QColor& color);

  void slotToggleAudioInput(bool isChecked);
  void slotToggleSpectrumAnalysis(bool isChecked);
  void slotToggleSpetrumSettings(bool isChecked);

  void slotToggleToneAnalysis(bool isChecked);
  void slotToggleStepTone(bool isChecked);
  void slotToggleSmoothTone(bool isChecked);


private:
  void createMenus();
  void createActions();
  void createToolbars();
  void connectAllSlots() const;

  void startAudioInputThread();
  void startAnimationThread() const;
  void startAnimation() const;

  void startAudioInput();
  void stopAudioInput();
  void startSpectrumAnalyser() const;
  void stopSpectrumAnalyser() const;
  void startToneAnalyser() const;
  void stopToneAnalyser() const;

private:
  Ui::MainWindow* ui;
  QColorDialog m_colorDialog;
  std::size_t m_nLedsTotal;

  std::shared_ptr<Player> m_player;
  std::unique_ptr<Studio> m_studio;
  std::unique_ptr<AudioInput> m_audioInput;
  std::shared_ptr<ControlSettings> m_settings;
  std::shared_ptr<SpectrumAnalyser> m_spectrumAnalyser;
  std::shared_ptr<ToneAnalyser> m_toneAnalyser;
  std::unique_ptr<SpectrumStudio> m_spectrumStudio;
  std::unique_ptr<ToneStudio> m_toneStudio;

  SpectrumSettingsWidget* m_spectrumSettingsWidget;
  QDialog* m_spectrumSettingsDialog;

  QToolBar* m_mainToolBar;
  QToolBar* m_detailsToolBar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;
  QAction* m_audioToggleButton;
  QAction* m_spectrumToggleButton;
  QAction* m_toneToggleButton;
  QAction* m_stepToneAct;
  QAction* m_smoothToneAct;
  QAction* m_openColorPickerAct;
  QAction* m_openSpectrumSettingsAct;

  bool m_isAudioOn;

  QTimer* m_timer;
  QTimer* m_timerEmulator;
};

#endif // MAINWINDOW_H
