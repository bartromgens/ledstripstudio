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
class SpectrumAnalyser;
class SpectrumSettingsWidget;
class ToneAnalyser;
class SpectrumStudio;
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

  void slotOpenColorPicker();
  void slotColorSelected(const QColor& color);
  void slotOpenSpetrumSettings();

  void slotToggleAudioInput(bool isChecked);
  void slotToggleSpectrumAnalysis(bool isChecked);
  void slotToggleToneAnalysis(bool isChecked);


private:
  void createMenus();
  void createActions();
  void createToolbar();
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
  std::shared_ptr<ControlSettings> m_audioControlSettings;
  std::shared_ptr<SpectrumAnalyser> m_spectrumAnalyser;
  std::shared_ptr<ToneAnalyser> m_toneAnalyser;
  std::unique_ptr<SpectrumStudio> m_spectrumStudio;

  SpectrumSettingsWidget* m_spectrumAnimationSettingsWidget;

//  LedStripEmulator* m_ledStripEmulator;

  QToolBar* fileToolBar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* m_audioToggleButton;
  QAction* m_spectrumToggleButton;
  QAction* m_toneToggleButton;
  QAction* m_openColorPickerAct;
  QAction* m_openSpectrumSettingsAct;

  bool m_isAudioOn;

  QTimer* m_timer;
  QTimer* m_timerEmulator;

  bool m_isSpectrumToLeds;
  bool m_isToneToLeds;
};

#endif // MAINWINDOW_H
