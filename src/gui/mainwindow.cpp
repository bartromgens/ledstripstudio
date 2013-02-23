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
  m_colorDialog(new QColorDialog(this)),
  m_nLedsTotal(160),
  m_player(new Player()),
  m_studio(new Studio(m_nLedsTotal)),
  m_audioInput(new AudioInput(SPECTRUM_SAMPLES, m_nLedsTotal)),
  m_settings(new ControlSettings()),
  m_spectrumAnalyser(new SpectrumAnalyser(SPECTRUM_SAMPLES)),
  m_toneAnalyser(new ToneAnalyser()),
  m_spectrumStudio(new SpectrumStudio()),
  m_toneStudio(new ToneStudio()),
  m_spectrumSettingsDialog(new QDockWidget(this)),
  m_spectrumSettingsWidget(new SpectrumSettingsWidget(m_settings, m_spectrumSettingsDialog)),
  m_isAudioOn(false),
  m_timer(0),
  m_lastSingleColor(0, 0, 0)
{
  ui->setupUi(this);

  setWindowTitle("Light Emitting Strip Studio");
  setWindowIcon(QIcon("./icons/color_wheel2.png"));

  createActions();
  createToolbars();
  createMenus();

  connectAllSlots();

//  QGridLayout* gridLayout = new QGridLayout(m_spectrumSettingsWidget);
//  gridLayout->addWidget(m_spectrumSettingsWidget);

  m_spectrumSettingsDialog->setVisible(false);

  m_settings->loadSettings();
  m_spectrumSettingsWidget->updateAudioControlGUI();

  m_audioInput->setControlSettings(m_settings);

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
  m_settings->setActive(true);

  m_audioInput->openStream();
  m_audioInput->startStream();
  m_audioInput->closeStream();
}


void
MainWindow::stopAudioInput()
{
  std::cout << "MainWindow::stopAudioInput()" << std::endl;
  m_settings->setActive(false);
  m_isAudioOn = false;
}


void
MainWindow::notifySpectrum(std::map<double, double> spectrum)
{
//  std::cout << "MainWindow::notifySpectrum()" << std::endl;
//  int minFreq = 200;
//  int maxFreq = 1000;
//  m_spectrumStudio->drawSpectrumInConsole(spectrum, minFreq, maxFreq);
  if (m_spectrumToggleButton->isChecked())
  {
    updateLEDs(spectrum);
  }
}


void
MainWindow::notifyTone(std::map<std::string, double> toneAmplitudes)
{
  Animation animation;
  animation = m_toneStudio->createToneAnimation(m_nLedsTotal, toneAmplitudes);
  m_player->addAnimation(animation);
  m_player->playFrame();
}


void
MainWindow::slotToggleAudioInput(bool isChecked)
{
  std::cout << "MainWindow::slotToggleAudioInput()" << std::endl;
  if (isChecked)
  {
    m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-high.png"));
    startAudioInputThread();
  }
  else
  {
    m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-muted.png"));
    stopAudioInput();
  }
}


void
MainWindow::slotToggleSpectrumAnalysis(bool isChecked)
{
  m_openSpectrumSettingsAct->setVisible(isChecked);

  if (isChecked)
  {
    m_toneToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
    m_colorToggleAct->setChecked(false);
    startSpectrumAnalyser();
  }
  else
  {
    m_openSpectrumSettingsAct->setVisible(false);
    if (!m_toneToggleButton->isChecked())
    {
      stopSpectrumAnalyser();
    }
  }
}


void
MainWindow::slotToggleToneAnalysis(bool isChecked)
{
  m_stepToneAct->setVisible(isChecked);
  m_smoothToneAct->setVisible(isChecked);

  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
    m_colorToggleAct->setChecked(false);
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
MainWindow::slotToggleAnimation(bool isChecked)
{
  m_dotsAnimationAct->setVisible(isChecked);
  m_rainbowAnimationAct->setVisible(isChecked);

  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_toneToggleButton->setChecked(false);
    m_colorToggleAct->setChecked(false);
  }
}


void
MainWindow::slotToggleSingleColor(bool isChecked)
{
  m_openColorPickerAct->setVisible(isChecked);

  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_toneToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
    slotColorSelected(m_lastSingleColor);
  }
}


void
MainWindow::slotShowSpetrumSettings()
{
  m_spectrumSettingsDialog->setFloating(true);
  m_spectrumSettingsDialog->setWidget(m_spectrumSettingsWidget);
  m_spectrumSettingsWidget->show();
  m_spectrumSettingsDialog->setVisible(true);
}


void
MainWindow::slotToggleStepTone(bool isChecked)
{
  if (isChecked)
  {
    m_smoothToneAct->setChecked(false);
    m_toneStudio->setAnimationType(ToneStudio::Loudest);
  }
  else
  {
    m_toneStudio->setAnimationType(ToneStudio::None);
  }
}


void
MainWindow::slotToggleSmoothTone(bool isChecked)
{
  if (isChecked)
  {
    m_stepToneAct->setChecked(false);
    m_toneStudio->setAnimationType(ToneStudio::SmoothSum);
  }
  else
  {
    m_toneStudio->setAnimationType(ToneStudio::None);
  }
}


void
MainWindow::slotOpenColorPicker()
{
  m_colorDialog->setVisible(true);
}


void
MainWindow::slotColorSelected(QColor color)
{
  m_lastSingleColor = color;
  Color colorNew(color.red()/255.0*127.0, color.green()/255.0*127.0, color.blue()/255.0*127.0);

  Animation animation = m_studio->createSingleColorSingleFrameAnimation(colorNew);

  m_player->addAnimation(animation);
  m_player->playFrame();
}


void
MainWindow::slotToggleDotAnimation(bool isChecked)
{
  if (isChecked)
  {
    startAnimationThread();
  }
}


void
MainWindow::slotToggleRainbowAnimation(bool isChecked)
{
  if (isChecked)
  {
    startAnimationThread();
  }
}



void
MainWindow::createActions()
{
  m_audioToggleButton = new QAction(this);
  m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-muted.png"));
  m_audioToggleButton->setStatusTip(tr("Start audio input control panel."));
  m_audioToggleButton->setCheckable(true);
  connect(m_audioToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleAudioInput(bool)));
//  m_audioToggleButton->setChecked(true);

  m_spectrumToggleButton = new QAction(this);
  m_spectrumToggleButton->setIcon(QIcon("./icons/wave_high_frequency.png"));
  m_spectrumToggleButton->setStatusTip(tr("Start spectrum mode."));
  m_spectrumToggleButton->setCheckable(true);
  connect(m_spectrumToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleSpectrumAnalysis(bool)));

  m_toneToggleButton = new QAction(this);
  m_toneToggleButton->setIcon(QIcon("./icons/audio-x-generic.png"));
  m_toneToggleButton->setStatusTip(tr("Start tone mode."));
  m_toneToggleButton->setCheckable(true);
  connect(m_toneToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleToneAnalysis(bool)));

  m_animationToggleAct = new QAction(this);
  m_animationToggleAct->setIcon(QIcon("./icons/animation-mode.png"));
  m_animationToggleAct->setStatusTip(tr("Start animation mode."));
  m_animationToggleAct->setCheckable(true);
  connect(m_animationToggleAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleAnimation(bool)));

  m_colorToggleAct = new QAction(this);
  m_colorToggleAct->setIcon(QIcon("./icons/color_wheel2.png"));
  m_colorToggleAct->setStatusTip(tr("Start single color mode."));
  m_colorToggleAct->setCheckable(true);
  connect(m_colorToggleAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleSingleColor(bool)));

  m_openSpectrumSettingsAct = new QAction(this);
  m_openSpectrumSettingsAct->setIcon(QIcon("./icons/preferences-system.png"));
  m_openSpectrumSettingsAct->setStatusTip(tr("Open spectrum settings."));
  m_openSpectrumSettingsAct->setVisible(false);
  connect(m_openSpectrumSettingsAct, SIGNAL(triggered()), this, SLOT(slotShowSpetrumSettings()));

  m_openColorPickerAct = new QAction(this);
  m_openColorPickerAct->setIcon(QIcon("./icons/color_wheel.png"));
  m_openColorPickerAct->setStatusTip(tr("Open color selector."));
  m_openColorPickerAct->setVisible(false);
  connect(m_openColorPickerAct, SIGNAL(triggered()), this, SLOT(slotOpenColorPicker()));

  m_stepToneAct = new QAction(this);
  m_stepToneAct->setIcon(QIcon("./icons/step-tone-setting.png"));
  m_stepToneAct->setStatusTip(tr("Set loudest tone mode."));
  m_stepToneAct->setCheckable(true);
  m_stepToneAct->setVisible(false);
  connect(m_stepToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleStepTone(bool)));

  m_smoothToneAct = new QAction(this);
  m_smoothToneAct->setIcon(QIcon("./icons/smooth-tone-setting.png"));
  m_smoothToneAct->setStatusTip(tr("Set smooth tone mode."));
  m_smoothToneAct->setCheckable(true);
  connect(m_smoothToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleSmoothTone(bool)));
  m_smoothToneAct->setChecked(true);
  m_smoothToneAct->setVisible(false);

  m_dotsAnimationAct = new QAction(this);
  m_dotsAnimationAct->setIcon(QIcon("./icons/smooth-tone-setting.png"));
  m_dotsAnimationAct->setStatusTip(tr("Toggles dots animation."));
  m_dotsAnimationAct->setCheckable(true);
  connect(m_dotsAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleDotAnimation(bool)));
  m_dotsAnimationAct->setChecked(false);
  m_dotsAnimationAct->setVisible(false);

  m_rainbowAnimationAct = new QAction(this);
  m_rainbowAnimationAct->setIcon(QIcon("./icons/rainbow-animation.png"));
  m_rainbowAnimationAct->setStatusTip(tr("Toggles rainbow animation."));
  m_rainbowAnimationAct->setCheckable(true);
  connect(m_rainbowAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleRainbowAnimation(bool)));
  m_rainbowAnimationAct->setChecked(false);
  m_rainbowAnimationAct->setVisible(false);
}


void
MainWindow::createToolbars()
{
  m_mainToolBar = new QToolBar(tr("Main toolbar"), this);
  addToolBar(Qt::TopToolBarArea, m_mainToolBar);
  m_mainToolBar->setIconSize(QSize(32, 32));

  m_mainToolBar->addAction(m_audioToggleButton);
  m_mainToolBar->addSeparator();
  m_mainToolBar->addAction(m_spectrumToggleButton);
  m_mainToolBar->addAction(m_toneToggleButton);
  m_mainToolBar->addAction(m_animationToggleAct);
  m_mainToolBar->addAction(m_colorToggleAct);
  m_mainToolBar->addSeparator();

  m_detailsToolBar = new QToolBar(tr("Details toolbar"), this);
  addToolBar(Qt::LeftToolBarArea, m_detailsToolBar);
  m_detailsToolBar->setIconSize(QSize(32, 32));
  m_detailsToolBar->setMinimumSize(32, 32);
  m_detailsToolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  m_detailsToolBar->setOrientation(Qt::Vertical);

  m_detailsToolBar->addSeparator();
  m_detailsToolBar->addAction(m_openSpectrumSettingsAct);
  m_detailsToolBar->addAction(m_stepToneAct);
  m_detailsToolBar->addAction(m_smoothToneAct);
  m_detailsToolBar->addAction(m_dotsAnimationAct);
  m_detailsToolBar->addAction(m_rainbowAnimationAct);
  m_detailsToolBar->addAction(m_openColorPickerAct);
}


void
MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  editMenu = menuBar()->addMenu(tr("&Edit"));
  helpMenu = menuBar()->addMenu(tr("&Help"));
}


void
MainWindow::connectAllSlots() const
{
  connect( m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));
}


void
MainWindow::closeEvent(QCloseEvent* /*event*/)
{
  m_settings->setActive(true);

  m_settings->saveSettings();
}


void
MainWindow::update()
{
  int fps = m_settings->getStatusFPS();
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

  m_settings->lock();

  double amplifyFactor = m_settings->volumeTotal/1000.0;
  double amplifyFactorRed = m_settings->volumeRed/25.0;
  double amplifyFactorGreen = m_settings->volumeGreen/50.0;
  double amplifyFactorBlue = m_settings->volumeBlue/100.0;

  int freqRmin = m_settings->freqRedMin;
  int freqRmax = m_settings->freqRedMax;
  int freqGmin = m_settings->freqGreenMin;
  int freqGmax = m_settings->freqGreenMax;
  int freqBmin = m_settings->freqBlueMin;
  int freqBmax = m_settings->freqBlueMax;

  m_settings->unlock();

  m_settings->setStatusFPS(m_player->getFPS());

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

    int nFrames = 1000;

//    Animation animationA = m_studio->createMovingDot(0, nFrames, colorA, 2.5);
//    Animation animationB = m_studio->createMovingDot(0, nFrames, colorB, 1.5);
//    Animation animationC = m_studio->createMovingDot(0, nFrames, colorC, 1.0);
//    Animation animationD = m_studio->createMovingDot(0, nFrames, colorD, 0.7);
//    Animation animationA = m_studio->createMovingDot(colorB, 1.0);

//    m_player->addAnimation(m_studio->createMovingLine(nFrames, colorA, 0.5));
//    m_player->addAnimation(m_studio->createMovingLine(nFrames, colorB, -0.4));
//    m_player->addAnimation(m_studio->createMovingLine(nFrames, colorC, 0.2));

//    Animation animationA = m_studio->createSingleColorSingleFrameAnimation(colorA);

    m_player->addAnimation(m_studio->createMovingRainbow());
//    m_player->addAnimation(m_studio->createRandomMovingDots(20, nFrames));
//    m_player->addAnimation(m_studio->createCellularAutomata());
    m_player->playAllAnimations();
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
MainWindow::startToneAnalyser() const
{
  m_spectrumAnalyser->registerObserver(m_toneAnalyser.get());
}


void
MainWindow::stopToneAnalyser() const
{
  m_spectrumAnalyser->unregisterObserver(m_toneAnalyser.get());
}
