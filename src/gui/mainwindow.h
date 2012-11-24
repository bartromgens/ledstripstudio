#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/basic/animation.h"

#include <QMainWindow>
#include <QToolBar>
#include <QColorDialog>

#include <iostream>
#include <memory>

class Studio;
class Player;
class AudioInput;

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

private slots:
  void slotColorSelected(const QColor &color);
  void slotBrightnessChanged();
  void slotToggleAudioInput();

private:
  void createMenus();
  void createActions();
  void createToolbar();
  void startAudioInput();
  void stopAudioInput();
  void connectAllSlots() const;
  void startAudioInputThread();
  
private:
  Ui::MainWindow *ui;
  QColorDialog m_colorDialog;
  std::size_t m_nLedsTotal;
  Player* m_player;
  Studio* m_studio;
  AudioInput* m_audioInput;

  QToolBar* fileToolBar;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;

  QAction* newAct;

  bool m_isAudioOn;
};

#endif // MAINWINDOW_H
