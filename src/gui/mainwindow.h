#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic/animation.h"

#include <QMainWindow>
#include <QToolBar>
#include <QColorDialog>
#include <QTimer>

#include <iostream>
#include <memory>

class AudioInput;
class AudioInputObserver;
class ControlSettings;
class LedStripEmulator;
class Player;
class Studio;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  Animation createAnimationTest1();
  Animation createAnimationTest2();

public slots:
  void slotPlayerPlayed();

protected:
  virtual void closeEvent(QCloseEvent* event);

private slots:
  void slotOpenColorPicker();
  void slotColorSelected(const QColor& color);
  void slotBrightnessChanged();
  void slotToggleAudioInput();
  void slotVolumeChanged();
  void slotFrequencyChanged();

private:
  void createMenus();
  void createActions();
  void createToolbar();
  void startAudioInput();
  void stopAudioInput();
  void connectAllSlots() const;
  void startAudioInputThread();
  void startAnimationThread() const;
  void startAnimation() const;
  void updateAudioControlGUI();
  
private slots:
  void update();

private:
  std::unique_ptr<Ui::MainWindow> ui;
  QColorDialog m_colorDialog;
  std::size_t m_nLedsTotal;

  std::shared_ptr<Player> m_player;
  std::unique_ptr<Studio> m_studio;
  std::unique_ptr<AudioInput> m_audioInput;
  std::shared_ptr<ControlSettings> m_audioControlSettings;
  std::shared_ptr<AudioInputObserver> m_spectrumAnalyser;

//  LedStripEmulator* m_ledStripEmulator;

  QToolBar* fileToolBar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* newAct;
  QAction* openColorPickerAct;

  bool m_isAudioOn;

  QTimer* m_timer;
  QTimer* m_timerEmulator;
};

#endif // MAINWINDOW_H
