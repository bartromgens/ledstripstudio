#include "mainwindow.h"
#include "src/gui/ui_mainwindow.h"

#include "src/player/animation.h"

#include <QTime>


#include <memory>
#include <unistd.h> // for usleep on linux

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_colorDialog(),
  m_nLedsTotal(160),
  m_player(new Player())
{
  setWindowTitle("LED Controller");

  ui->setupUi(this);

  m_colorDialog.open();

  connect( &m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));

  connect( ui->brightnessRedHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
  connect( ui->brightnessGreenHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
  connect( ui->brightnessBlueHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );

  //  Animation animation;

//  for (std::size_t i = 0; i < 1000; ++i)
//  {
//    animation = createAnimationTest1();
//    m_player->play(animation);
//  }

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
  double speed = 1.0;
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
MainWindow::slotColorSelected(const QColor &color)
{
  Frame frame(m_nLedsTotal);
  for (std::size_t i = 0; i < m_nLedsTotal; ++i)
  {
    LED led;
    Color colorNew(color.red()/255.0*127.0, color.green()/255.0*127.0, color.blue()/255.0*127.0);
    led.setColor(colorNew);
    led.setLEDnr(i);
    frame.addLED(led);
  }
  Animation animation;
  animation.addFrame(frame);

  m_player->play(animation);
}

void
MainWindow::slotBrightnessChanged()
{
  int brightnessRed = ui->brightnessRedHorizontalSlider->value();
  int brightnessGreen = ui->brightnessGreenHorizontalSlider->value();
  int brightnessBlue = ui->brightnessBlueHorizontalSlider->value();

  Frame frame(m_nLedsTotal);

  for (std::size_t i = 0; i < m_nLedsTotal; ++i)
  {
    LED led;
    Color color(brightnessRed, brightnessGreen, brightnessBlue);
    led.setColor(color);
    led.setLEDnr(i);
    frame.addLED(led);
  }
  Animation animation;
  animation.addFrame(frame);

  m_player->play(animation);

//  updateBrightness();
}
