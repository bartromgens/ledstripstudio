#include "mainwindow.h"
#include "src/gui/ui_mainwindow.h"

#include "src/audioinput/audioinput.h"
#include "src/basic/animation.h"
#include "src/gui/ledstripemulator.h"
#include "src/player/player.h"
#include "src/studio/studio.h"
#include "src/settings/controlsettings.h"


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
  m_audioControlSettings(new ControlSettings()),
  m_isAudioOn(false),
  m_timer(0)
{
  setWindowTitle("LED Controller");

  ui->setupUi(this);

  createActions();
  createToolbar();
  createMenus();

  connectAllSlots();

  m_audioControlSettings->loadSettings();
  updateAudioControlGUI();

  m_audioInput->setControlSettings(m_audioControlSettings);

  m_timer = new QTimer(this);
  m_timer->setInterval(200);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->start();

  m_timerEmulator = new QTimer(this);
  m_timerEmulator->setInterval(30);
  connect(m_timerEmulator, SIGNAL(timeout()), this, SLOT(slotPlayerPlayed()));
  m_timerEmulator->start();

  for (std::size_t i = 0; i < 1000; ++i)
  {
    Color colorA(127, 0, 0);
    Color colorB(0, 127, 0);
    Animation animationA = m_studio->createMovingLine(colorA, 2.0);
    Animation animationB = m_studio->createMovingLine(colorB, 1.0);
    Animation animationCombined = animationA.combineTwoAnimations(animationA, animationB);
    m_player->play(animationA);
  }

}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_audioInput;
  delete m_studio;
  delete m_player;
  delete m_audioControlSettings;
}


void
MainWindow::createActions()
{
  newAct = new QAction(tr("&Toggle Audio"), this);
  newAct->setStatusTip(tr("Start Audio Input Control Panel"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(slotToggleAudioInput()));

  openColorPickerAct = new QAction(tr("Pick Colour"), this);
  openColorPickerAct->setStatusTip(tr("Open colour picker"));
  connect(openColorPickerAct, SIGNAL(triggered()), this, SLOT(slotOpenColorPicker()));
}

void
MainWindow::createToolbar()
{
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->addAction(newAct);
  fileToolBar->addAction(openColorPickerAct);
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
  m_audioControlSettings->setActive(true);

  m_audioInput->openStream();
  m_audioInput->setLedPlayer(m_player);
  m_audioInput->startStream();
  m_audioInput->closeStream();
}


void
MainWindow::stopAudioInput()
{
  std::cout << "MainWindow::stopAudioInput()" << std::endl;
  m_audioControlSettings->setActive(false);
  m_isAudioOn = false;
}

void
MainWindow::connectAllSlots() const
{
  connect( &m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));
  connect( ui->brightnessRedSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
  connect( ui->brightnessGreenSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );
  connect( ui->brightnessBlueSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotBrightnessChanged() ) );

  connect( ui->volumeTotalSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );
  connect( ui->volumeRedSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );
  connect( ui->volumeGreenSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );
  connect( ui->volumeBlueSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotVolumeChanged() ) );

  connect( ui->freqRmaxSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqRminSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqGmaxSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqGminSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqBmaxSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );
  connect( ui->freqBminSlider, SIGNAL( valueChanged(int) ), this, SLOT( slotFrequencyChanged() ) );

  connect( ui->freqRminSlider, SIGNAL( valueChanged(int) ), ui->freqRminSpin, SLOT(setValue(int)) );
  connect( ui->freqRminSpin, SIGNAL( valueChanged(int) ), ui->freqRminSlider, SLOT(setValue(int)) );
  connect( ui->freqRmaxSlider, SIGNAL( valueChanged(int) ), ui->freqRmaxSpin, SLOT(setValue(int)) );
  connect( ui->freqRmaxSpin, SIGNAL( valueChanged(int) ), ui->freqRmaxSlider, SLOT(setValue(int)) );

  connect( ui->freqGminSlider, SIGNAL( valueChanged(int) ), ui->freqGminSpin, SLOT(setValue(int)) );
  connect( ui->freqGminSpin, SIGNAL( valueChanged(int) ), ui->freqGminSlider, SLOT(setValue(int)) );
  connect( ui->freqGmaxSlider, SIGNAL( valueChanged(int) ), ui->freqGmaxSpin, SLOT(setValue(int)) );
  connect( ui->freqGmaxSpin, SIGNAL( valueChanged(int) ), ui->freqGmaxSlider, SLOT(setValue(int)) );

  connect( ui->freqBminSlider, SIGNAL( valueChanged(int) ), ui->freqBminSpin, SLOT(setValue(int)) );
  connect( ui->freqBminSpin, SIGNAL( valueChanged(int) ), ui->freqBminSlider, SLOT(setValue(int)) );
  connect( ui->freqBmaxSlider, SIGNAL( valueChanged(int) ), ui->freqBmaxSpin, SLOT(setValue(int)) );
  connect( ui->freqBmaxSpin, SIGNAL( valueChanged(int) ), ui->freqBmaxSlider, SLOT(setValue(int)) );
}


void
MainWindow::slotOpenColorPicker()
{
  m_colorDialog.open();
}


void
MainWindow::slotColorSelected(const QColor &color)
{
  Color colorNew(color.red()/255.0*127.0, color.green()/255.0*127.0, color.blue()/255.0*127.0);

  Animation animation = m_studio->createSingleColorSingleFrameAnimation(colorNew);

  m_player->play(animation);
}


void
MainWindow::updateAudioControlGUI()
{
  m_audioControlSettings->lock();
  ui->volumeTotalSlider->setValue(m_audioControlSettings->volumeTotal);
  ui->volumeRedSlider->setValue(m_audioControlSettings->volumeRed);
  ui->volumeGreenSlider->setValue(m_audioControlSettings->volumeGreen);
  ui->volumeBlueSlider->setValue(m_audioControlSettings->volumeBlue);

  ui->freqRminSlider->setValue(m_audioControlSettings->freqRedMin);
  ui->freqRmaxSlider->setValue(m_audioControlSettings->freqRedMax);
  ui->freqGminSlider->setValue(m_audioControlSettings->freqGreenMin);
  ui->freqGmaxSlider->setValue(m_audioControlSettings->freqGreenMax);
  ui->freqBminSlider->setValue(m_audioControlSettings->freqBlueMin);
  ui->freqBmaxSlider->setValue(m_audioControlSettings->freqBlueMax);
  m_audioControlSettings->unlock();
}


void
MainWindow::slotVolumeChanged()
{
  if (m_audioControlSettings->try_lock())
  {
    m_audioControlSettings->volumeTotal = ui->volumeTotalSlider->value();
    m_audioControlSettings->volumeRed = ui->volumeRedSlider->value();
    m_audioControlSettings->volumeGreen = ui->volumeGreenSlider->value();
    m_audioControlSettings->volumeBlue = ui->volumeBlueSlider->value();

    m_audioControlSettings->unlock();
  }
}


void
MainWindow::slotFrequencyChanged()
{
  if (m_audioControlSettings->try_lock())
  {
    m_audioControlSettings->freqRedMin = ui->freqRminSpin->value();
    m_audioControlSettings->freqRedMax = ui->freqRmaxSpin->value();
    m_audioControlSettings->freqGreenMin = ui->freqGminSpin->value();
    m_audioControlSettings->freqGreenMax = ui->freqGmaxSpin->value();
    m_audioControlSettings->freqBlueMin = ui->freqBminSpin->value();
    m_audioControlSettings->freqBlueMax = ui->freqBmaxSpin->value();

    m_audioControlSettings->unlock();
  }
}


void
MainWindow::slotBrightnessChanged()
{
  int red = ui->brightnessRedSlider->value();
  int green = ui->brightnessGreenSlider->value();
  int blue = ui->brightnessBlueSlider->value();
  m_audioControlSettings->setValue("brightnessRed", red);
  m_audioControlSettings->setValue("brightnessGreen", green);
  m_audioControlSettings->setValue("brightnessBlue", blue);

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


void
MainWindow::closeEvent(QCloseEvent* /*event*/)
{
  m_audioControlSettings->setActive(true);

  m_audioControlSettings->saveSettings();
}


void
MainWindow::update()
{
//  std::cout << "update" << std::endl;
  int fps = m_audioControlSettings->getStatusFPS();
  ui->fpsLcd->setText(QString::number(fps));
  ui->ledStripEmulator->update();
}


void
MainWindow::slotPlayerPlayed()
{
  ui->ledStripEmulator->setHeight(ui->ledStripEmulator->size().height());
  ui->ledStripEmulator->setWidth(ui->ledStripEmulator->size().width()/m_nLedsTotal);
  ui->ledStripEmulatorRed->setHeight(ui->ledStripEmulatorRed->size().height());
  ui->ledStripEmulatorRed->setWidth(ui->ledStripEmulatorRed->size().width()/m_nLedsTotal);
  ui->ledStripEmulatorGreen->setHeight(ui->ledStripEmulatorGreen->size().height());
  ui->ledStripEmulatorGreen->setWidth(ui->ledStripEmulatorGreen->size().width()/m_nLedsTotal);
  ui->ledStripEmulatorBlue->setHeight(ui->ledStripEmulatorBlue->size().height());
  ui->ledStripEmulatorBlue->setWidth(ui->ledStripEmulatorBlue->size().width()/m_nLedsTotal);

  Frame frame = m_player->getLastFrame();
  ui->ledStripEmulator->setFrame(frame);
  ui->ledStripEmulator->update();

  frame = m_player->getLastFrame();
  frame.amplifyRGB(1.0, 0.0, 0.0);
  ui->ledStripEmulatorRed->setFrame(frame);

  frame = m_player->getLastFrame();
  frame.amplifyRGB(0.0, 1.0, 0.0);
  ui->ledStripEmulatorGreen->setFrame(frame);

  frame = m_player->getLastFrame();
  frame.amplifyRGB(0.0, 0.0, 1.0);
  ui->ledStripEmulatorBlue->setFrame(frame);

  ui->ledStripEmulatorRed->update();
  ui->ledStripEmulatorGreen->update();
  ui->ledStripEmulatorBlue->update();
}
