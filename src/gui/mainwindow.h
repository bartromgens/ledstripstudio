#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/basic/animation.h"

#include <QMainWindow>
#include <QToolBar>
#include <QColorDialog>

#include <iostream>
#include <memory>

class AudioInput;
class ControlSettings;
class Player;
class Studio;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  Animation createAnimationTest1();
  Animation createAnimationTest2();

protected:
  virtual void closeEvent(QCloseEvent *event);

private slots:
  void slotOpenColorPicker();
  void slotColorSelected(const QColor &color);
  void slotBrightnessChanged();
  void slotToggleAudioInput();
  void slotVolumeChanged();

private:
  void createMenus();
  void createActions();
  void createToolbar();
  void startAudioInput();
  void stopAudioInput();
  void connectAllSlots() const;
  void startAudioInputThread();
  void updateAudioControlGUI();
  
private:
  Ui::MainWindow *ui;
  QColorDialog m_colorDialog;
  std::size_t m_nLedsTotal;
  Player* m_player;
  Studio* m_studio;
  AudioInput* m_audioInput;
  ControlSettings* m_audioControlSettings;

  QToolBar* fileToolBar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* newAct;
  QAction* openColorPickerAct;

  bool m_isAudioOn;
};

#endif // MAINWINDOW_H
