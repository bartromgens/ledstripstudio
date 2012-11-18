#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/player/ledcontroller.h"
#include "src/player/player.h"

#include <QMainWindow>
#include <QColorDialog>

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

  Animation createAnimationTest1();
  Animation createAnimationTest2();
//  Animation createAnimationTest3();
  //  Animation createAnimationTest4();
private slots:
  void slotColorSelected(const QColor &color);
  void slotBrightnessChanged();

private:
//  std::unique_ptr<LEDController> createLedController(QString serialPortName);
  
private:
  Ui::MainWindow *ui;
  QColorDialog m_colorDialog;
  std::size_t m_nLedsTotal;
  Player* m_player;
};

#endif // MAINWINDOW_H
