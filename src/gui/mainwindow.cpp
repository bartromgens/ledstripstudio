#include "mainwindow.h"
#include "gui/ui_mainwindow.h"


#include "audioinput/audioinput.h"
#include "gui/ledstripstatuswidget.h"
#include "gui/spectrumsettingswidget.h"
#include "gui/playersettingswidget.h"
#include "spectrum/spectrumanalyser.h"
#include "spectrum/toneanalyser.h"
#include "studio/imagestudio.h"
#include "studio/studio.h"
#include "studio/spectrumstudio.h"

#include "bitcoinclient/bitcoinexchangeclient.h"

#include <QFileDialog>
#include <QPushButton>

const int SPECTRUM_SAMPLES = static_cast<int>(std::pow(2.0, 15));
const int NLEDS = 160;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  SpectrumObserver(),
  ToneObserver(),
  ui(new Ui::MainWindow),
  m_colorDialog(new QColorDialog(this)),
  m_nLedsTotal(NLEDS),
  m_settings(new ControlSettings()),
  m_player(new Player(m_settings)),
  m_studio(new Studio(m_nLedsTotal)),
  m_audioInput(new AudioInput(SPECTRUM_SAMPLES, m_nLedsTotal)),
  m_spectrumAnalyser(new SpectrumAnalyser(SPECTRUM_SAMPLES)),
  m_toneAnalyser(new ToneAnalyser()),
  m_spectrumStudio(new SpectrumStudio()),
  m_toneStudio(new ToneStudio()),
  m_imageStudio(new ImageStudio(m_nLedsTotal)),
  m_mtgoxState(new BitcoinExchangeClient()),
//  m_spectrumSettingsDialog(new QDockWidget(this)),
  m_spectrumSettingsWidget(new SpectrumSettingsWidget(m_settings, this)),
  m_ledStripStatusWidget(new LedStripStatusWidget(this)),
//  m_playerSettingsDialog(new QDockWidget(this)),
//  m_playerSettingsWidget(new PlayerSettingsWidget()),
  m_toneToolbar(m_toneStudio),
  m_fftToolbar(m_audioInput, m_spectrumAnalyser),
  m_timer(),
  m_lastSingleColor(0, 0, 0)
{
  ui->setupUi(this);

  ui->mainWidget->setSettings(m_settings.get());

  setWindowTitle("Light Emitting Strip Studio");
  setWindowIcon(QIcon("./icons/color_wheel2.png"));

  createActions();
  createToolbars();
  createMenus();

  connectAllSlots();

  m_settings->loadSettings();
  m_spectrumSettingsWidget->updateAudioControlGUI();

  m_audioInput->setControlSettings(m_settings);

  createTimers();

  m_spectrumAnalyser->registerObserver(this);
  m_toneAnalyser->registerObserver(this);

//  ui->frame_3->setVisible(false);
  ui->centralwidget->layout()->addWidget(m_spectrumSettingsWidget);
  ui->centralwidget->layout()->addWidget(m_ledStripStatusWidget);

  setActionsDefaults();
}


MainWindow::~MainWindow()
{
  stopToneAnalyser();
  stopSpectrumAnalyser();

  delete ui;

  std::cout << "MainWindow::~MainWindow()" << std::endl;
}


void
MainWindow::createTimers()
{
  m_timer.reset(new QTimer(this));
  m_timer->setInterval(10000);
  connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(update()));
  m_timer->start();

  m_timerEmulator.reset(new QTimer(this));
  m_timerEmulator->setInterval(30);
  connect(m_timerEmulator.get(), SIGNAL(timeout()), this, SLOT(slotPlayerPlayed()));
  m_timerEmulator->start();

//  m_timerEmulator = new QTimer(this);
//  unsigned int nowMsSinceEpoch = QDateTime::toTime_t();
//  QDateTime::currentMSecsSinceEpoch();

//  m_timerEmulator->setInterval(30);
//  connect(m_timerEmulator, SIGNAL(timeout()), this, SLOT(slotPlayerPlayed()));
//  m_timerEmulator->start();
}


void
MainWindow::closeEvent(QCloseEvent* /*event*/)
{
  std::cout << "MainWindow::closeEvent()" << std::endl;

  m_settings->saveSettings();
  stopAudioInput();

  if (m_audioInputThread)
  {
    if(!m_audioInputThread->timed_join(boost::posix_time::seconds(2)))
    {
      m_audioInputThread->interrupt();
    }
  }
}


void
MainWindow::startAudioInputThread()
{
  std::cout << "MainWindow::startAudioInputThread()" << std::endl;
  m_audioInputThread.reset(new boost::thread(&MainWindow::startAudioInput, this));
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
MainWindow::startAnimationThread() const
{
  std::cout << "MainWindow::startAnimation()" << std::endl;
  boost::thread t1(&MainWindow::startAnimation, this);
  t1.detach();
}


void
MainWindow::startAnimation() const
{
  m_player->playAllAnimations();
}


void
MainWindow::stopAnimation()
{
  m_player->stopAnimations();
}


void
MainWindow::startSpectrumAnalyser() const
{
  m_audioInput->registerObserver(m_spectrumAnalyser);
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
  if (isChecked)
  {
    m_toneToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
    m_colorToggleAct->setChecked(false);
    startSpectrumAnalyser();
  }
  else
  {
    m_spectrumSettingsToggleAct->setVisible(false);
    if (!m_toneToggleButton->isChecked())
    {
      stopSpectrumAnalyser();
    }
  }

  m_spectrumSettingsToggleAct->setVisible(isChecked);
  m_fftToolbar.toggleFFTSettings(isChecked);
}


void
MainWindow::slotToggleToneAnalysis(bool isChecked)
{
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

  m_toneToolbar.toggleToneAnalysis(isChecked);
  m_fftToolbar.toggleFFTSettings(isChecked);
}


void
MainWindow::slotToggleAnimation(bool isChecked)
{
  m_dotsAnimationAct->setVisible(isChecked);
  m_rainbowAnimationAct->setVisible(isChecked);
  m_imageAnimationAct->setVisible(isChecked);

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
MainWindow::slotToggleSpectrumSettings(bool isChecked)
{
  m_spectrumSettingsWidget->setVisible(isChecked);
//  m_spectrumSettingsDialog->setFloating(true);
//  m_spectrumSettingsDialog->setWidget(m_spectrumSettingsWidget);
//  m_spectrumSettingsWidget->show();
//  m_spectrumSettingsDialog->setVisible(true);
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
  Color colorNew(color);

  Animation animation = m_studio->createSingleColorSingleFrameAnimation(colorNew);

  m_player->addAnimation(animation);
  m_player->playFrame();
}


void
MainWindow::slotToggleDotAnimation(bool isChecked)
{
  if (isChecked)
  {
    startDotAnimationThread();
  }
  else
  {
    stopAnimation();
  }
}


void
MainWindow::startDotAnimationThread()
{
  boost::thread t1(&MainWindow::playDotAnimation, this);
  t1.detach();
}


void
MainWindow::playDotAnimation()
{
  m_player->addAnimation( m_studio->createSingleColorSingleFrameAnimation( Color() ) );
  m_player->playAllAnimations();

  std::vector<Color> colors;
  colors.push_back(Color(127, 0, 0));
  colors.push_back(Color(0, 127, 0));
  colors.push_back(Color(0, 0, 127));
  colors.push_back(Color(127, 0, 127));
  colors.push_back(Color(127, 127, 0));
  colors.push_back(Color(0, 127, 127));

  while (m_player->isPlaying())
  {
    for (std::size_t i = 0; i < 2; ++i)
    {
      int nFrames = 1000;

      for (std::size_t i = 0; i < colors.size(); ++i)
      {
//        Animation animationA = m_studio->createMovingDot((rand() % m_nLedsTotal), nFrames, colors[i], ((rand() % 400 - 200) / 100.0) ) ;
        Animation animationA = m_studio->createMovingLine((rand() % m_nLedsTotal), nFrames, colors[i], ((rand() % 400 - 200) / 100.0) ) ;
        m_player->addAnimation(animationA);
      }
    }

    m_player->playAllAnimations();
  }
}


void
MainWindow::slotToggleRainbowAnimation(bool isChecked)
{
  if (isChecked)
  {
    double speed = 2;
    m_player->addAnimation(m_studio->createMovingRainbow(speed));
    startAnimationThread();
  }
  else
  {
    stopAnimation();
  }
}


void
MainWindow::slotToggleImageAnimation(bool isChecked)
{
  if (isChecked)
  {
    QString filename = QFileDialog::getOpenFileName(this, "Open Image");
    Animation animation = m_imageStudio->createImageAnimation(filename.toStdString());
    m_player->addAnimation(animation);
    startAnimationThread();
  }
  else
  {
    stopAnimation();
  }
}


void
MainWindow::slotToggleRecording(bool isChecked)
{
  if (isChecked)
  {
    m_player->startRecording();
  }
  else
  {
    m_player->stopRecording();
    Animation animation = m_player->getRecordedAnimation();
    m_player->clearRecordedAnimation();

    std::string filename = "recordedAnimation";
    m_imageStudio->createImageFromAnimation(animation, filename);
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

  m_spectrumSettingsToggleAct = new QAction(this);
  m_spectrumSettingsToggleAct->setIcon(QIcon("./icons/preferences-system.png"));
  m_spectrumSettingsToggleAct->setStatusTip(tr("Open spectrum settings."));
  m_spectrumSettingsToggleAct->setVisible(false);
  m_spectrumSettingsToggleAct->setCheckable(true);
  connect(m_spectrumSettingsToggleAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleSpectrumSettings(bool)));

  m_openColorPickerAct = new QAction(this);
  m_openColorPickerAct->setIcon(QIcon("./icons/color_wheel.png"));
  m_openColorPickerAct->setStatusTip(tr("Open color selector."));
  m_openColorPickerAct->setVisible(false);
  connect(m_openColorPickerAct, SIGNAL(triggered()), this, SLOT(slotOpenColorPicker()));

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

  m_imageAnimationAct = new QAction(this);
  m_imageAnimationAct->setIcon(QIcon("./icons/image-animation.png"));
  m_imageAnimationAct->setStatusTip(tr("Toggles images animation."));
  m_imageAnimationAct->setCheckable(true);
  connect(m_imageAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleImageAnimation(bool)));
  m_imageAnimationAct->setChecked(false);
  m_imageAnimationAct->setVisible(false);

  m_recordAnimationAct = new QAction(this);
  m_recordAnimationAct->setIcon(QIcon("./icons/record-animation.png"));
  m_recordAnimationAct->setStatusTip(tr("Toggles recording the animation."));
  m_recordAnimationAct->setCheckable(true);
  connect(m_recordAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleRecording(bool)));
  m_recordAnimationAct->setChecked(false);
  m_recordAnimationAct->setVisible(true);
}


void
MainWindow::setActionsDefaults()
{
  m_audioToggleButton->setChecked(true);
}


void
MainWindow::createToolbars()
{
  m_mainToolBar = new QToolBar(tr("Main toolbar"), this);
  addToolBar(Qt::TopToolBarArea, m_mainToolBar);
  m_mainToolBar->setIconSize(QSize(32, 32));

  m_mainToolBar->addAction(m_spectrumToggleButton);
  m_mainToolBar->addAction(m_toneToggleButton);
  m_mainToolBar->addAction(m_animationToggleAct);
  m_mainToolBar->addAction(m_colorToggleAct);
  m_mainToolBar->addSeparator();
  m_mainToolBar->addAction(m_audioToggleButton);
  m_mainToolBar->addSeparator();
  m_mainToolBar->addAction(m_recordAnimationAct);
  m_mainToolBar->addSeparator();

  m_detailsToolBar = new QToolBar(tr("Details toolbar"), this);
  addToolBar(Qt::LeftToolBarArea, m_detailsToolBar);
  m_detailsToolBar->setIconSize(QSize(32, 32));
  m_detailsToolBar->setMinimumSize(32, 32);
  m_detailsToolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  m_detailsToolBar->setOrientation(Qt::Vertical);

  m_detailsToolBar->addSeparator();
  m_detailsToolBar->addAction(m_spectrumSettingsToggleAct);

  m_toneToolbar.initialise(m_detailsToolBar);

  m_detailsToolBar->addAction(m_dotsAnimationAct);
  m_detailsToolBar->addAction(m_rainbowAnimationAct);
  m_detailsToolBar->addAction(m_imageAnimationAct);

  m_detailsToolBar->addAction(m_openColorPickerAct);

  m_detailsToolBar->addSeparator();

  m_fftToolbar.initialise(m_detailsToolBar);
}


void
MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  editMenu = menuBar()->addMenu(tr("&Edit"));
  helpMenu = menuBar()->addMenu(tr("&Help"));
}


void
MainWindow::connectAllSlots()
{
  connect( m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));
}


void
MainWindow::update()
{
//  getMtGox();

  int fps = m_player->getFPS();
  m_ledStripStatusWidget->setFPS(fps);
}


void
MainWindow::slotPlayerPlayed()
{
  Frame frame = m_player->getLastFrame();

  m_ledStripStatusWidget->update(frame);
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

  m_player->playFrame();
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


void
MainWindow::getMtGox()
{
  m_mtgoxState->getMtGoxData();

  std::cout << m_mtgoxState->m_last << std::endl;

  int ledNr = (m_mtgoxState->m_last -m_mtgoxState->m_low) / (m_mtgoxState->m_high - m_mtgoxState->m_low) * m_nLedsTotal;

  std::cout << ledNr << std::endl;

  Color color;
  if ( std::fabs(m_mtgoxState->m_lastOld - m_mtgoxState->m_last) < 0.01 )
  {
    color = Color(0, 0, 127);
  }
  else if ( m_mtgoxState->m_lastOld > m_mtgoxState->m_last )
  {
    color = Color(127, 0, 0);
  }
  else
  {
    color = Color(0, 127, 0);
  }

  Animation animation;
  Frame frame(m_nLedsTotal);
  for (int i = 0; i < ledNr; ++i)
  {
    LED led(i, color);
    frame.addLED(led);
  }
  animation.addFrame(frame);

  m_player->addAnimation(animation);
  m_player->addAnimation(animation);
  m_player->playFrame();
}

