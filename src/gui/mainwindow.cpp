#include "mainwindow.h"
#include "gui/ui_mainwindow.h"


#include "audioinput/audioinput.h"
#include "gui/spectrumsettingswidget.h"
#include "spectrum/spectrumanalyser.h"
#include "spectrum/toneanalyser.h"
#include "studio/studio.h"
#include "studio/spectrumstudio.h"
#include "studio/tonestudio.h"

#include <QPushButton>

#include <boost/thread.hpp>

const int SPECTRUM_SAMPLES = static_cast<int>(std::pow(2.0, 16));

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  SpectrumObserver(),
  ToneObserver(),
  ui(new Ui::MainWindow),
  m_colorDialog(),
  m_nLedsTotal(160),
  m_player(new Player()),
  m_studio(new Studio(m_nLedsTotal)),
  m_audioInput(new AudioInput(SPECTRUM_SAMPLES)),
  m_audioControlSettings(new ControlSettings()),
  m_spectrumAnalyser(new SpectrumAnalyser(SPECTRUM_SAMPLES)),
  m_toneAnalyser(new ToneAnalyser()),
  m_spectrumStudio(new SpectrumStudio()),
  m_spectrumAnimationSettingsWidget(new SpectrumSettingsWidget(m_audioControlSettings)),
  m_isAudioOn(false),
  m_timer(0),
  m_isSpectrumToLeds(false),
  m_isToneToLeds(false)
{
  ui->setupUi(this);

  setWindowTitle("Light Emitting Strip Studio");
  setWindowIcon(QIcon("./icons/color_wheel2.png"));

  createActions();
  createToolbar();
  createMenus();

  connectAllSlots();

  m_audioControlSettings->loadSettings();
  m_spectrumAnimationSettingsWidget->updateAudioControlGUI();

  m_audioInput->setControlSettings(m_audioControlSettings);

  m_timer = new QTimer(this);
  m_timer->setInterval(200);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->start();

  m_timerEmulator = new QTimer(this);
  m_timerEmulator->setInterval(30);
  connect(m_timerEmulator, SIGNAL(timeout()), this, SLOT(slotPlayerPlayed()));
  m_timerEmulator->start();

  m_spectrumAnalyser->registerObserver(this);
  m_toneAnalyser->registerObserver(this);

//  startAnimationThread();
}


MainWindow::~MainWindow()
{
  delete ui;
}


void
MainWindow::slotToggleAudioInput(bool isChecked)
{
  std::cout << "MainWindow::slotToggleAudioInput()" << std::endl;
  if (isChecked)
  {
    m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-high.svg"));
    startAudioInputThread();
  }
  else
  {
    m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-muted.svg"));
    stopAudioInput();
  }
}


void
MainWindow::startAudioInputThread()
{
  m_isAudioOn = true;
  std::cout << "MainWindow::startAudioInputThread()" << std::endl;
  boost::thread t1(&MainWindow::startAudioInput, this);
  t1.detach();
}


void
MainWindow::startAudioInput()
{
  m_audioControlSettings->setActive(true);

  m_audioInput->openStream();
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
MainWindow::notifySpectrum(std::map<double, double> spectrum)
{
//  std::cout << "MainWindow::notifySpectrum()" << std::endl;
//  int minFreq = 200;
//  int maxFreq = 1000;
//  m_spectrumStudio->drawSpectrumInConsole(spectrum, minFreq, maxFreq);
  if (m_isSpectrumToLeds)
  {
    updateLEDs(spectrum);
  }
}


void
MainWindow::notifyTone(std::map<std::string, double> toneAmplitudes)
{
  ToneStudio toneStudio;
  Animation animation;
//  animation = toneStudio.createToneAnimationLoudest(m_nLedsTotal, toneAmplitudes);
  animation = toneStudio.createToneAnimationSmoothSum(m_nLedsTotal, toneAmplitudes);
  m_player->addAnimation(animation);
  m_player->playFrame();
}


void
MainWindow::slotToggleSpectrumAnalysis(bool isChecked)
{
  m_isSpectrumToLeds = isChecked;
  if (isChecked)
  {
    m_toneToggleButton->setChecked(false);
    m_isToneToLeds = false;
    startSpectrumAnalyser();
  }
  else
  {
    if (!m_isToneToLeds)
    {
      stopSpectrumAnalyser();
    }
  }
}


void
MainWindow::startSpectrumAnalyser() const
{
  m_audioInput->registerObserver(m_spectrumAnalyser);
}


void
MainWindow::stopSpectrumAnalyser() const
{
  m_audioInput->unregisterObserver(m_spectrumAnalyser);
}


void
MainWindow::slotToggleToneAnalysis(bool isChecked)
{
  m_isToneToLeds = isChecked;
  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_isSpectrumToLeds = false;
    startSpectrumAnalyser();
    startToneAnalyser();
  }
  else
  {
    stopSpectrumAnalyser();
    stopToneAnalyser();
  }
}


void
MainWindow::startToneAnalyser() const
{
  m_spectrumAnalyser->registerObserver(m_toneAnalyser.get());
}


void
MainWindow::stopToneAnalyser() const
{
  m_spectrumAnalyser->unregisterObserver(m_toneAnalyser.get());
}


void
MainWindow::createActions()
{
  m_audioToggleButton = new QAction(this);
  m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-muted.svg"));
  m_audioToggleButton->setStatusTip(tr("Start audio input control panel."));
  m_audioToggleButton->setCheckable(true);
  connect(m_audioToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleAudioInput(bool)));

  m_spectrumToggleButton = new QAction(this);
  m_spectrumToggleButton->setIcon(QIcon("./icons/wave_high_frequency.png"));
  m_spectrumToggleButton->setStatusTip(tr("Start spectrum analysis."));
  m_spectrumToggleButton->setCheckable(true);
  connect(m_spectrumToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleSpectrumAnalysis(bool)));

  m_toneToggleButton = new QAction(this);
  m_toneToggleButton->setIcon(QIcon("./icons/audio-x-generic.svg"));
  m_toneToggleButton->setStatusTip(tr("Start tone analysis."));
  m_toneToggleButton->setCheckable(true);
  connect(m_toneToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleToneAnalysis(bool)));

  m_openColorPickerAct = new QAction(this);
  m_openColorPickerAct->setIcon(QIcon("./icons/color_wheel2.png"));
  m_openColorPickerAct->setStatusTip(tr("Open colour picker."));
  connect(m_openColorPickerAct, SIGNAL(triggered()), this, SLOT(slotOpenColorPicker()));

  m_openSpectrumSettingsAct = new QAction(this);
  m_openSpectrumSettingsAct->setIcon(QIcon("./icons/settings.svg"));
  m_openSpectrumSettingsAct->setStatusTip(tr("Open spectrum settings."));
  connect(m_openSpectrumSettingsAct, SIGNAL(triggered()), this, SLOT(slotOpenSpetrumSettings()));
}


void
MainWindow::createToolbar()
{
  fileToolBar = addToolBar(tr("Main toolbar"));
  fileToolBar->setIconSize(QSize(32, 32));

  fileToolBar->addAction(m_audioToggleButton);
  fileToolBar->addSeparator();
  fileToolBar->addAction(m_spectrumToggleButton);
  fileToolBar->addAction(m_toneToggleButton);
  fileToolBar->addSeparator();
  fileToolBar->addAction(m_openColorPickerAct);
  fileToolBar->addSeparator();
  fileToolBar->addAction(m_openSpectrumSettingsAct);
}


void
MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  editMenu = menuBar()->addMenu(tr("&Edit"));
  helpMenu = menuBar()->addMenu(tr("&Help"));
}


void
MainWindow::startAnimationThread() const
{
  std::cout << "MainWindow::startAnimation()" << std::endl;
  boost::thread t1(&MainWindow::startAnimation, this);
  t1.detach();
}


void
MainWindow::startAnimation() const
{
  for (std::size_t i = 0; i < 1; ++i)
  {
    Color colorA(127, 0, 0);
    Color colorB(0, 127, 0);
    Color colorC(0, 0, 127);
    Color colorD(127, 0, 127);

//    int nFrames = 10000;

//    Animation animationA = m_studio->createMovingDot(0, nFrames, colorA, 2.5);
//    Animation animationB = m_studio->createMovingDot(0, nFrames, colorB, 1.5);
//    Animation animationC = m_studio->createMovingDot(0, nFrames, colorC, 1.0);
//    Animation animationD = m_studio->createMovingDot(0, nFrames, colorD, 0.7);
//    Animation animationA = m_studio->createMovingDot(colorB, 1.0);

//    m_player->addAnimation(m_studio->createMovingLine(nFrames, colorA, 1.1));
//    m_player->addAnimation(m_studio->createMovingLine(nFrames, colorB, -0.4));
//    m_player->addAnimation(m_studio->createMovingLine(nFrames, colorC, 0.2));

//    Animation animationA = m_studio->createSingleColorSingleFrameAnimation(colorA);

//    m_player->addAnimation(m_studio->createMovingRainbow());
//    m_player->addAnimation(m_studio->createRandomMovingDots(20, nFrames));
    m_player->addAnimation(m_studio->createCellularAutomata());
    m_player->playAllAnimations();
  }
}


void
MainWindow::connectAllSlots() const
{
  connect( &m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));
}


void
MainWindow::slotOpenSpetrumSettings()
{
  QDialog* dialog = new QDialog();
  QGridLayout* gridLayout = new QGridLayout(dialog);
  gridLayout->addWidget(m_spectrumAnimationSettingsWidget);
  dialog->show();
//  m_spectrumAnimationSettingsWidget->show();
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

  m_player->addAnimation(animation);
  m_player->playFrame();
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
  int fps = m_audioControlSettings->getStatusFPS();
  ui->fpsLcd->setText(QString::number(fps));
}


void
MainWindow::slotPlayerPlayed()
{
  Frame frame = m_player->getLastFrame();

  if (ui->enableEmulator->isChecked())
  {
    ui->ledStripEmulator->setHeight(ui->ledStripEmulator->size().height());
    ui->ledStripEmulator->setWidth(ui->ledStripEmulator->size().width()/m_nLedsTotal);
    ui->ledStripEmulator->setFrame(frame);
    ui->ledStripEmulator->update();
  }

  if (ui->enableEmulatorRGB->isChecked())
  {
    ui->ledStripEmulatorRed->setHeight(ui->ledStripEmulatorRed->size().height());
    ui->ledStripEmulatorRed->setWidth(ui->ledStripEmulatorRed->size().width()/m_nLedsTotal);
    ui->ledStripEmulatorGreen->setHeight(ui->ledStripEmulatorGreen->size().height());
    ui->ledStripEmulatorGreen->setWidth(ui->ledStripEmulatorGreen->size().width()/m_nLedsTotal);
    ui->ledStripEmulatorBlue->setHeight(ui->ledStripEmulatorBlue->size().height());
    ui->ledStripEmulatorBlue->setWidth(ui->ledStripEmulatorBlue->size().width()/m_nLedsTotal);

    Frame frameR = frame;
    Frame frameG = frame;
    Frame frameB = frame;

    frameR.amplifyRGB(1.0, 0.0, 0.0);
    frameG.amplifyRGB(0.0, 1.0, 0.0);
    frameB.amplifyRGB(0.0, 0.0, 1.0);

    ui->ledStripEmulatorRed->setFrame(frameR);
    ui->ledStripEmulatorGreen->setFrame(frameG);
    ui->ledStripEmulatorBlue->setFrame(frameB);

    ui->ledStripEmulatorRed->update();
    ui->ledStripEmulatorGreen->update();
    ui->ledStripEmulatorBlue->update();
  }
}


void
MainWindow::updateLEDs(const std::map<double, double>& spectrum)
{
//  std::cout << "MainWindow::updateLEDs" << std::endl;
  double brightnessRed = 0.0;
  double brightnessGreen = 0.0;
  double brightnessBlue = 0.0;

  m_audioControlSettings->lock();

  double amplifyFactor = m_audioControlSettings->volumeTotal/1000.0;
  double amplifyFactorRed = m_audioControlSettings->volumeRed/25.0;
  double amplifyFactorGreen = m_audioControlSettings->volumeGreen/50.0;
  double amplifyFactorBlue = m_audioControlSettings->volumeBlue/100.0;

  int freqRmin = m_audioControlSettings->freqRedMin;
  int freqRmax = m_audioControlSettings->freqRedMax;
  int freqGmin = m_audioControlSettings->freqGreenMin;
  int freqGmax = m_audioControlSettings->freqGreenMax;
  int freqBmin = m_audioControlSettings->freqBlueMin;
  int freqBmax = m_audioControlSettings->freqBlueMax;

  m_audioControlSettings->unlock();

  m_audioControlSettings->setStatusFPS(m_player->getFPS());

  for (std::map<double, double>::const_iterator iter = spectrum.begin();
       iter != spectrum.end(); ++iter)
  {
    double frequency = iter->first;
    double amplitude = iter->second;

    if (frequency > freqRmin && frequency < freqRmax)
    {
        brightnessRed += amplitude*amplifyFactor*amplifyFactorRed;
    }

    if (frequency > freqGmin && frequency < freqGmax)
    {
      brightnessGreen += amplitude*amplifyFactor*amplifyFactorGreen;
    }

    if (frequency > freqBmin && frequency < freqBmax)
    {
      brightnessBlue += amplitude*amplifyFactor*amplifyFactorBlue;
    }
  }

  Animation animation = m_spectrumStudio->createWaveformAnimationCentral(m_nLedsTotal, brightnessRed, brightnessGreen, brightnessBlue);
  m_player->addAnimation(animation);

  //  Studio studio(nLEDs);
//  if (m_nUpdates++ % (nLEDs/30) == 1)
//  {
//    double speed = 1.0;
//    m_ledPlayer->addAnimation( studio.createMovingLine(nLEDs/speed,
//                                                       Color(std::abs(cos(m_nUpdates/5.0+1)*127), std::abs(sin(m_nUpdates/8.0)*127), std::abs(sin(m_nUpdates/23.0+5)*127)),
//                                                       speed)
//                             );
//  }

  m_player->playFrame();
}
