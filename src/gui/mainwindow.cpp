#include "mainwindow.h"
#include "src/gui/ui_mainwindow.h"

#include "src/basic/animation.h"
#include "src/player/player.h"
#include "src/studio/studio.h"
#include "src/audioinput/audioinput.h"


#include <QTime>

#include <thread>
#include <memory>
#include <unistd.h> // for usleep on linux


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_colorDialog(),
  m_nLedsTotal(160),
  m_player(new Player()),
  m_studio(new Studio(m_nLedsTotal)),
  m_audioInput(new AudioInput()),
  m_isAudioOn(false)
{
  setWindowTitle("LED Controller");

  ui->setupUi(this);

  createActions();
  createToolbar();
  createMenus();

  connectAllSlots();

//  m_colorDialog.open();

  //  Animation animation;


//  for (std::size_t i = 0; i < 1000; ++i)
//  {
//    Color color(cos(i/M_PI)*177, sin(i/M_PI)*177, cos(2*i/M_PI)*177);
//    Animation animation = m_studio->createMovingLine(color);
//    m_player->play(animation);
//  }

}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_player;
  delete m_studio;
}


void
MainWindow::createActions()
{
  newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Start Audio Input Control Panel"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(slotToggleAudioInput()));
}

void
MainWindow::createToolbar()
{
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->addAction(newAct);
}

void
MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));

  helpMenu = menuBar()->addMenu(tr("&Help"));
}


void
MainWindow::startAudioInputThread()
{
  m_isAudioOn = true;
  std::cout << "MainWindow::startAudioInputThread()" << std::endl;
  std::thread t1(&MainWindow::startAudioInput, this);
  t1.detach();
}

void
MainWindow::startAudioInput()
{
  m_audioInput->openStream();
  m_audioInput->setLedPlayer(m_player);
  m_audioInput->startStream();
}


void
MainWindow::stopAudioInput()
{
  std::cout << "MainWindow::stopAudioInput()" << std::endl;
  m_audioInput->setLedPlayer(m_player);
  m_audioInput->closeStream();
  m_isAudioOn = false;
}

void
MainWindow::connectAllSlots() const
{
  connect( &m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));
  connect( ui->brightnessRedHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
  connect( ui->brightnessGreenHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
  connect( ui->brightnessBlueHorizontalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
}


void
MainWindow::slotColorSelected(const QColor &color)
{
  Color colorNew(color.red()/255.0*127.0, color.green()/255.0*127.0, color.blue()/255.0*127.0);

  Animation animation = m_studio->createSingleColorSingleFrameAnimation(colorNew);

  m_player->play(animation);
}


void
MainWindow::slotBrightnessChanged()
{
  int red = ui->brightnessRedHorizontalSlider->value();
  int green = ui->brightnessGreenHorizontalSlider->value();
  int blue = ui->brightnessBlueHorizontalSlider->value();
  Color color(red, green, blue);

  Animation animation = m_studio->createSingleColorSingleFrameAnimation(color);

  m_player->play(animation);
}


void
MainWindow::slotToggleAudioInput()
{
  if (m_isAudioOn)
  {
    stopAudioInput();
  }
  else
  {
    startAudioInputThread();
  }
}
