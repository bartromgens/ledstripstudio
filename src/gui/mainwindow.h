#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/basic/animation.h"

#include <QMainWindow>
#include <QColorDialog>

#include <iostream>
#include <memory>

class Studio;
class Player;

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

private:
  void connectAllSlots() const;
  
private:
  Ui::MainWindow *ui;
  QColorDialog m_colorDialog;
  std::size_t m_nLedsTotal;
  Player* m_player;
  Studio* m_studio;
};

#endif // MAINWINDOW_H
