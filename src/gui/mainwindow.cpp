#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/player/animation.h"

#include <QTime>

#include <memory>
#include <unistd.h> // for usleep on linux

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_nLedsTotal(160),
  m_player(new Player())
{
  setWindowTitle("LED Controller");

  ui->setupUi(this);
  sleep(2);
//  connect( ui->brightnessYellowHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( updateBrightnessBlue() ) );
//  connect( ui->brightnessRedHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( updateBrightnessRed() ) );
//  connect( ui->brightnessGreenHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( updateBrightnessGreen() ) );
  Animation animation;

  QTime timer;
  timer.start();

  for (std::size_t i = 0; i < 1000; ++i)
  {
    animation = createAnimationTest2();
    m_player->play(animation);
  }
//  std::cout << timer.elapsed() << std::endl;

}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_player;
}

Animation
MainWindow::createAnimationTest1()
{
  Animation animation;
  std::size_t nFrames = 127;

  for (std::size_t i = 0; i < nFrames; ++i)
  {
    Frame frame(m_nLedsTotal);
    for (std::size_t j = 1; j <= m_nLedsTotal; ++j)
    {
      LED led;
      led.setLEDnr(j);
      led.setColor(Color(i, 0, 0));
      frame.addLED(led);
    }
    animation.addFrame(frame);
  }
  for (std::size_t i = 0; i < nFrames; ++i)
  {
    Frame frame(m_nLedsTotal);
    for (std::size_t j = 1; j <= m_nLedsTotal; ++j)
    {
      LED led;
      led.setLEDnr(j);
      led.setColor(Color(0, i, 0));
      frame.addLED(led);
    }
    animation.addFrame(frame);
  }
  for (std::size_t i = 0; i < nFrames; ++i)
  {
    Frame frame(m_nLedsTotal);
    for (std::size_t j = 1; j <= m_nLedsTotal; ++j)
    {
      LED led;
      led.setLEDnr(j);
      led.setColor(Color(i, 0, j));
      frame.addLED(led);
    }
    animation.addFrame(frame);
  }
  return animation;
}

Animation MainWindow::createAnimationTest2()
{
  Animation animation;
  double speed = 0.3;
  int nFrames = m_nLedsTotal/speed;

  for (int i = 1; i < nFrames; ++i)
  {
    double pos = i*speed+1;
    double dx = std::fabs(int(pos)-pos);
    double dx1 = std::fabs(int(pos+1)-pos);
    double dx2 = std::fabs(int(pos-1)-pos);
    double dx3 = std::fabs(int(pos+2)-pos);
    double dx4 = std::fabs(int(pos-2)-pos);

//    std::cout << dx1 << ", " << dx2 << ", " << dx3 << ", " << dx4 << std::endl;

    if ((unsigned int)pos < (m_nLedsTotal-2))
    {
      Frame frame(m_nLedsTotal);
      LED led;
      led.setLEDnr(int(pos));
      led.setColor(Color(100-50*dx, 0, 0));
      frame.addLED(led);
      led = LED();
      led.setLEDnr(int(pos+1));
      led.setColor(Color(100-40*dx1, 0, 0));
      frame.addLED(led);
      led = LED();
      led.setLEDnr(int(pos+2));
      led.setColor(Color(100-40*dx3, 0, 0));
      frame.addLED(led);
      if (pos > 3.0)
      {
        led = LED();
        led.setLEDnr(int(pos-1));
        led.setColor(Color(100-40*dx2, 0, 0));
        frame.addLED(led);
        led = LED();
        led.setLEDnr(int(pos-2));
        led.setColor(Color(100-40*dx4, 0, 0));
        frame.addLED(led);
      }
      animation.addFrame(frame);
    }
  }

  return animation;
}

//Animation MainWindow::createAnimationTest3()
//{
//}

//Animation MainWindow::createAnimationTest4()
//{
//}

//void
//MainWindow::updateBrightness()
//{
//  m_ledController->send();
//}

void
MainWindow::updateBrightnessBlue()
{
//  int brightness = ui->brightnessYellowHorizontalSlider->value();
//  Color color(127, 0, brightness);
//  for (int i = 1; i <= m_nLedsTotal; ++i)
//  {
//    m_ledController->setColor(color);
//    m_ledController->setLEDnr(i);
//  }
//  updateBrightness();
}

void
MainWindow::updateBrightnessGreen()
{
//  int brightness = ui->brightnessGreenHorizontalSlider->value();
//  Color color(0, brightness, 0);
//  m_ledController->setColor(color);
//  m_ledController->setLEDnr(30);
//  updateBrightness();
}

void
MainWindow::updateBrightnessRed()
{
//  int brightness = ui->brightnessRedHorizontalSlider->value();
//  Color color(brightness, 0, 0);
//  m_ledController->setColor(color);
//  m_ledController->setLEDnr(45);
//  updateBrightness();
}
