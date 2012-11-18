#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/player/ledcontroller.h"
#include "src/player/player.h"

#include <QMainWindow>

#include <iostream>
#include <memory>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void updateBrightness();

  Animation createAnimationTest1();
  Animation createAnimationTest2();
//  Animation createAnimationTest3();
//  Animation createAnimationTest4();
private slots:
  void updateBrightnessBlue();
  void updateBrightnessRed();
  void updateBrightnessGreen();

private:
//  std::unique_ptr<LEDController> createLedController(QString serialPortName);
  
private:
  Ui::MainWindow *ui;
  std::size_t m_nLedsTotal;
  Player* m_player;
};

#endif // MAINWINDOW_H
