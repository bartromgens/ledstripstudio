#include "mainwindow.h"
#include "gui/ui_mainwindow.h"

#include "audioinput/audioinput.h"
#include "gui/ledstripstatuswidget.h"
#include "gui/spectrumsettingswidget.h"
#include "gui/spectrumwidget.h"
#include "gui/playersettingswidget.h"
#include "gui/serialportconnectiondialog.h"
#include "settings/configurationgroup.h"
#include "settings/controlsettings.h"
#include "spectrum/beatanalyser.h"
#include "spectrum/spectrumanalyser.h"
#include "spectrum/toneanalyser.h"
#include "studio/imagestudio.h"
#include "studio/studio.h"
#include "studio/spectrumstudio.h"

#include <QActionGroup>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>


const int SPECTRUM_SAMPLES = static_cast<int>(std::pow(2.0, 15));
const int NLEDS = 156;
//const int NLEDS = 120;
//const int NLEDS = 160;

QString MainWindow::m_defaultConfigFilename = "./config/default.cfg";
QString MainWindow::m_userConfigFilename = "./config/user.cfg";


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  SpectrumObserver(),
  ToneObserver(),
  ConfigurationGroup(),
  ui(new Ui::MainWindow),
  m_colorDialog(new QColorDialog(this)),
  m_nLedsTotal(NLEDS),
  m_settings(new ControlSettings()),
  m_player(new Player(*m_settings)),
  m_audioInput(new AudioInput(SPECTRUM_SAMPLES, *m_settings)),
  m_studio(new Studio(m_nLedsTotal)),
  m_beatAnalyser(new BeatAnalyser()),
  m_spectrumAnalyser(new SpectrumAnalyser(SPECTRUM_SAMPLES)),
  m_toneAnalyser(new ToneAnalyser()),
  m_spectrumStudio(new SpectrumStudio()),
  m_toneStudio(new ToneStudio()),
  m_imageStudio(new ImageStudio(m_nLedsTotal)),
  m_configurationGroups(),
  m_spectrumSettingsWidget(new SpectrumSettingsWidget(*m_settings, this)),
  m_ledStripStatusWidget(new LedStripStatusWidget(this)),
  m_playerSettingsWidget(new PlayerSettingsWidget(*m_settings, this)),
  m_spectrumWidget(new SpectrumWidget(this)),
  m_toneToolbar(new ToneToolbar(*m_toneStudio, this)),
  m_fftToolbar(new FFTToolbar(*m_audioInput, *m_spectrumAnalyser, this)),
  m_applicationSettingsDialog(new ApplicationSettingsDialog(this)),
  m_actionConsistency(new ActionConsistency()),
  m_timer(),
  m_lastSingleColor(0, 0, 0)
{
  qDebug() << __PRETTY_FUNCTION__;

  ui->setupUi(this);

  setWindowTitle("Light Emitting Strip Studio");
  setWindowIcon(QIcon("./icons/color_wheel2.png"));

  createActions();
  createToolbars();

  connectAllSlots();
  createTimers();

  ui->centralwidget->layout()->addWidget(m_ledStripStatusWidget);
  ui->centralwidget->layout()->addWidget(m_spectrumWidget);
  ui->centralwidget->layout()->addWidget(m_playerSettingsWidget);
  ui->centralwidget->layout()->addWidget(m_spectrumSettingsWidget);

  m_spectrumSettingsWidget->setVisible(false);
  m_fftToolbar->setVisible(false);
  m_toneAnalyser->registerObserver(this);
  m_spectrumAnalyser->registerObserver(this);

  m_configurationGroups.push_back(m_spectrumSettingsWidget);
  m_configurationGroups.push_back(m_playerSettingsWidget);
  m_configurationGroups.push_back(m_toneToolbar);
  m_configurationGroups.push_back(m_fftToolbar);
  m_configurationGroups.push_back(m_applicationSettingsDialog);

  loadUserOrDefaultConfig();

  checkConnection();
}


MainWindow::~MainWindow()
{
  stopBeatAnalyser();
  stopToneAnalyser();
  stopSpectrumAnalyser();
  slotClearStrip();

  delete ui;

  qDebug() << __PRETTY_FUNCTION__;
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
}


void
MainWindow::closeEvent(QCloseEvent* /*event*/)
{
  qDebug() << __PRETTY_FUNCTION__;
  hide();

  slotSaveConfiguration(m_userConfigFilename);
  stopAudioInput();

  if (m_audioInputThread && !m_audioInputThread->timed_join(boost::posix_time::seconds(2)))
  {
    m_audioInputThread->interrupt();
  }
}


void
MainWindow::startAudioInputThread()
{
  qDebug() << __PRETTY_FUNCTION__;
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
  qDebug() << __PRETTY_FUNCTION__;
  m_settings->setActive(false);
}


void
MainWindow::notifySpectrum(const std::vector<std::pair<double, double>>& spectrum)
{
  if (m_spectrumToggleButton->isChecked())
  {
    m_player->addAnimation( m_spectrumStudio->createAnimation(m_nLedsTotal, spectrum, *m_settings) );

    m_player->playFrame();
  }
  m_spectrumWidget->setSpectrum(spectrum);
}


void
MainWindow::notifyTone(const std::map<Tone, double>& toneAmplitudes)
{
  m_player->addAnimation( m_toneStudio->createToneAnimation( m_nLedsTotal, toneAmplitudes ) );
  m_player->playFrame();
}


void
MainWindow::startAnimationThread() const
{
  qDebug() << __PRETTY_FUNCTION__;
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
MainWindow::slotToggleAudioInput(bool isChecked)
{
  qDebug() << __PRETTY_FUNCTION__;
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
MainWindow::slotToggleBeatAnalysis(bool isChecked)
{
  if (isChecked)
  {
    startBeatAnalyser();
  }
  else
  {
    stopBeatAnalyser();
  }
}


void
MainWindow::slotToggleSpectrumAnalysis(bool isChecked)
{
  m_spectrumSettingsWidget->setVisible(isChecked);
  m_fftToolbar->setVisible(isChecked);

  if (isChecked)
  {
    startSpectrumAnalyser();
  }
  else
  {
    stopSpectrumAnalyser();
  }
}


void
MainWindow::slotToggleToneAnalysis(bool isChecked)
{
  m_settings->positionOffset = m_nLedsTotal/2;

  if (isChecked)
  {
    startSpectrumAnalyser();
    startToneAnalyser();
  }
  else
  {
    stopSpectrumAnalyser();
    stopToneAnalyser();
  }
  m_toneToolbar->setVisible(isChecked);
  m_fftToolbar->setVisible(isChecked);
}


void
MainWindow::slotToggleAnimation(bool isChecked)
{
  m_actionConsistency->toggleAnimation(isChecked);
}


void
MainWindow::slotToggleSingleColor(bool isChecked)
{
  m_openColorPickerAct->setVisible(isChecked);

  if (isChecked)
  {
    slotColorSelected(m_lastSingleColor);
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
  colors.push_back(Color(255, 0, 0));
  colors.push_back(Color(0, 255, 0));
  colors.push_back(Color(0, 0, 255));
  colors.push_back(Color(255, 0, 255));
  colors.push_back(Color(255, 255, 0));
  colors.push_back(Color(0, 255, 255));

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
MainWindow::slotShowSettingsDialog()
{
  if (m_applicationSettingsDialog->exec())
  {
    qDebug() << "ApplicationSettingsDialog accepted";
  }
  else
  {
    qDebug() << "ApplicationSettingsDialog rejected";
  }
}


void
MainWindow::createActions()
{
  m_stripToggleButton = new QAction(this);
  m_stripToggleButton->setIcon(QIcon("./icons/clear-strip.png"));
  m_stripToggleButton->setToolTip("Turn all LEDs off");
  connect(m_stripToggleButton, SIGNAL(triggered()), this, SLOT(slotClearStrip()));

  m_audioToggleButton = new QAction(this);
  m_audioToggleButton->setIcon(QIcon("./icons/audio-volume-muted.png"));
  m_audioToggleButton->setToolTip("Toggle audio input control panel");
  m_audioToggleButton->setCheckable(true);
  connect(m_audioToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleAudioInput(bool)));
//  m_audioToggleButton->setChecked(true);

  m_beatToggleButton = new QAction(this);
  m_beatToggleButton->setIcon(QIcon("./icons/beat.png"));
  m_beatToggleButton->setToolTip("Toggle beat detection");
  m_beatToggleButton->setCheckable(true);
  m_beatToggleButton->setVisible(false);  // TODO BR: make visible when beat detection is properly implemented
  connect(m_beatToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleBeatAnalysis(bool)));

  m_spectrumToggleButton = new QAction(this);
  m_spectrumToggleButton->setIcon(QIcon("./icons/wave_high_frequency.png"));
  m_spectrumToggleButton->setToolTip("Toggle spectrum mode");
  m_spectrumToggleButton->setCheckable(true);
  connect(m_spectrumToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleSpectrumAnalysis(bool)));

  m_toneToggleButton = new QAction(this);
  m_toneToggleButton->setIcon(QIcon("./icons/audio-x-generic.png"));
  m_toneToggleButton->setToolTip("Toggle tone mode");
  m_toneToggleButton->setCheckable(true);
  connect(m_toneToggleButton, SIGNAL(toggled(bool)), this, SLOT(slotToggleToneAnalysis(bool)));

  m_animationToggleAct = new QAction(this);
  m_animationToggleAct->setIcon(QIcon("./icons/animation-mode.png"));
  m_animationToggleAct->setToolTip("Toggle animation mode.");
  m_animationToggleAct->setCheckable(true);
  connect(m_animationToggleAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleAnimation(bool)));

  m_colorToggleAct = new QAction(this);
  m_colorToggleAct->setIcon(QIcon("./icons/color_wheel2.png"));
  m_colorToggleAct->setToolTip("Toggle single color mode");
  m_colorToggleAct->setCheckable(true);
  connect(m_colorToggleAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleSingleColor(bool)));

  m_openColorPickerAct = new QAction(this);
  m_openColorPickerAct->setIcon(QIcon("./icons/color_wheel.png"));
  m_openColorPickerAct->setToolTip("Open color selector");
  m_openColorPickerAct->setVisible(false);
  connect(m_openColorPickerAct, SIGNAL(triggered()), this, SLOT(slotOpenColorPicker()));

  m_dotsAnimationAct = new QAction(this);
  m_dotsAnimationAct->setIcon(QIcon("./icons/smooth-tone-setting.png"));
  m_dotsAnimationAct->setToolTip("Toggle dots animation");
  m_dotsAnimationAct->setCheckable(true);
  connect(m_dotsAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleDotAnimation(bool)));
  m_dotsAnimationAct->setChecked(false);
  m_dotsAnimationAct->setVisible(false);

  m_rainbowAnimationAct = new QAction(this);
  m_rainbowAnimationAct->setIcon(QIcon("./icons/rainbow-animation.png"));
  m_rainbowAnimationAct->setToolTip("Toggle rainbow animation");
  m_rainbowAnimationAct->setCheckable(true);
  connect(m_rainbowAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleRainbowAnimation(bool)));
  m_rainbowAnimationAct->setChecked(false);
  m_rainbowAnimationAct->setVisible(false);

  m_imageAnimationAct = new QAction(this);
  m_imageAnimationAct->setIcon(QIcon("./icons/image-animation.png"));
  m_imageAnimationAct->setToolTip("Play image");
  m_imageAnimationAct->setCheckable(true);
  connect(m_imageAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleImageAnimation(bool)));
  m_imageAnimationAct->setChecked(false);
  m_imageAnimationAct->setVisible(false);

  m_recordAnimationAct = new QAction(this);
  m_recordAnimationAct->setIcon(QIcon("./icons/record-animation.png"));
  m_recordAnimationAct->setToolTip("Record animation");
  m_recordAnimationAct->setCheckable(true);
  connect(m_recordAnimationAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleRecording(bool)));
  m_recordAnimationAct->setChecked(false);
  m_recordAnimationAct->setVisible(true);

  m_applicationSettingsAct = new QAction(this);
  m_applicationSettingsAct->setIcon(QIcon("./icons/preferences-system.svg"));
  m_applicationSettingsAct->setToolTip("Application settings");
  connect(m_applicationSettingsAct, SIGNAL(triggered(bool)), this, SLOT(slotShowSettingsDialog()));

  m_animationTypeActionGroup = new QActionGroup(this);
  m_animationTypeActionGroup->addAction(m_toneToggleButton);
  m_animationTypeActionGroup->addAction(m_spectrumToggleButton);
  m_animationTypeActionGroup->addAction(m_animationToggleAct);
  m_animationTypeActionGroup->addAction(m_colorToggleAct);

  m_actionConsistency->m_dotsAnimationAct = m_dotsAnimationAct;
  m_actionConsistency->m_rainbowAnimationAct = m_rainbowAnimationAct;
  m_actionConsistency->m_imageAnimationAct = m_imageAnimationAct;
  m_actionConsistency->m_openColorPickerAct = m_openColorPickerAct;
}


void
MainWindow::createToolbars()
{
  m_mainToolBar = new QToolBar(tr("Main toolbar"), this);
  m_mainToolBar->setIconSize(QSize(32, 32));

  m_mainToolBar->addAction(m_beatToggleButton);
  m_mainToolBar->addAction(m_spectrumToggleButton);
  m_mainToolBar->addAction(m_toneToggleButton);
  m_mainToolBar->addAction(m_animationToggleAct);
  m_mainToolBar->addAction(m_colorToggleAct);
  m_mainToolBar->addSeparator();
  m_mainToolBar->addSeparator();
  m_mainToolBar->addAction(m_audioToggleButton);
  m_mainToolBar->addAction(m_recordAnimationAct);
  m_mainToolBar->addSeparator();
  m_mainToolBar->addAction(m_stripToggleButton);
  m_mainToolBar->addSeparator();

  createConfigurationComboBox();

  QAction* saveConfig = new QAction( QIcon("./icons/document-save-as.svg"), "Save configuration as", this );
  connect( saveConfig, SIGNAL(triggered()), this, SLOT(slotSaveConfigurationAs()) );
  m_mainToolBar->addAction(saveConfig);
  m_mainToolBar->addAction(m_applicationSettingsAct);

  m_detailsToolBar = new QToolBar(tr("Details toolbar"), this);
  m_detailsToolBar->setIconSize(QSize(32, 32));

  m_detailsToolBar->addAction(m_dotsAnimationAct);
  m_detailsToolBar->addAction(m_rainbowAnimationAct);
  m_detailsToolBar->addAction(m_imageAnimationAct);
  m_detailsToolBar->addAction(m_openColorPickerAct);

  addToolBar(Qt::TopToolBarArea, m_mainToolBar);
  addToolBar(Qt::TopToolBarArea, m_fftToolbar);
  addToolBarBreak(Qt::TopToolBarArea);
  addToolBar(Qt::TopToolBarArea, m_toneToolbar);
  addToolBar(Qt::TopToolBarArea, m_detailsToolBar);

  m_fftToolbar->setIconSize(QSize(32, 32));
  m_toneToolbar->setIconSize(QSize(32, 32));
}


void
MainWindow::createConfigurationComboBox()
{
  QComboBox* configCombo = new QComboBox(this);
  configCombo->setFixedHeight(32);

  QStringList nameFilter("*.cfg");
  QDir directory("./config");
  QStringList txtFilesAndDirectories = directory.entryList(nameFilter);

  for (QString& v : txtFilesAndDirectories)
  {
    v.remove(".cfg");
    configCombo->addItem(v);
  }

  m_mainToolBar->addWidget(configCombo);
  connect( configCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotConfigComboChanged(QString)) );
}


void
MainWindow::connectAllSlots()
{
  connect( m_colorDialog, SIGNAL( currentColorChanged(const QColor) ), this, SLOT( slotColorSelected(const QColor) ));
}


void
MainWindow::update()
{
  int fps = m_player->getFPS();
  m_ledStripStatusWidget->setFPS(fps);
}


void
MainWindow::slotClearStrip()
{
  Studio studio(m_nLedsTotal);
  Animation animation = studio.createSingleColorSingleFrameAnimation(Color(0, 0, 0));
  m_player->addAnimation(animation);
  m_player->playFrame();
  m_player->addAnimation(animation);
  m_player->playFrame();
}


void
MainWindow::slotPlayerPlayed()
{
  Frame frame = m_player->getLastFrame();

  m_ledStripStatusWidget->update(frame);
  m_spectrumWidget->update();
}


void
MainWindow::slotConfigComboChanged(QString comboText)
{
  qDebug() << __PRETTY_FUNCTION__;
  QString filename = "./config/" + comboText + ".cfg";
  QSettings settings(filename, QSettings::NativeFormat);
  loadConfigurationAll(settings);
}


void
MainWindow::slotSaveConfiguration(const QString& filename)
{
  QSettings settings(filename, QSettings::NativeFormat);
  qDebug() << __PRETTY_FUNCTION__ << "file: " << filename;
  saveConfigurationAll(settings);
}

void
MainWindow::startBeatAnalyser() const
{
  m_spectrumAnalyser->registerObserver(m_beatAnalyser.get());
}


void
MainWindow::stopBeatAnalyser() const
{
  m_spectrumAnalyser->unregisterObserver(m_beatAnalyser.get());
}


void
MainWindow::startSpectrumAnalyser() const
{
  m_audioInput->registerObserver( m_spectrumAnalyser.get() );
}


void
MainWindow::stopSpectrumAnalyser() const
{
  m_audioInput->unregisterObserver( m_spectrumAnalyser.get() );
}


void
MainWindow::startToneAnalyser() const
{
  m_spectrumAnalyser->registerObserver( m_toneAnalyser.get() );
}


void
MainWindow::stopToneAnalyser() const
{
  m_spectrumAnalyser->unregisterObserver( m_toneAnalyser.get() );
}


void
MainWindow::slotSaveConfigurationAs()
{
  QString filename = QFileDialog::getSaveFileName(this, "Save configuration as", QDir::currentPath() + "/config/", "Configuration files (*.cfg)");
  QSettings config(filename, QSettings::NativeFormat);
  qDebug() << __PRETTY_FUNCTION__ << "file: " << filename;
  saveConfigurationAll(config);
}


void
MainWindow::saveConfiguration(QSettings& config) const
{
  qDebug() << __PRETTY_FUNCTION__;

  config.beginGroup( "Main" );

  config.setValue("audioInput", m_audioToggleButton->isChecked());
  config.setValue("toneAnalysis", m_toneToggleButton->isChecked());
  config.setValue("spectrumAnalysis", m_spectrumToggleButton->isChecked());
  config.setValue("singleColor", m_colorToggleAct->isChecked());
  config.setValue("animation", m_animationToggleAct->isChecked());

  config.endGroup();
}


void
MainWindow::loadConfiguration(QSettings& config)
{
  qDebug() << __PRETTY_FUNCTION__;

  config.beginGroup( "Main" );

  m_audioToggleButton->setChecked(config.value("audioInput").toBool());
  m_toneToggleButton->setChecked(config.value("toneAnalysis").toBool());
  m_spectrumToggleButton->setChecked(config.value("spectrumAnalysis").toBool());
  m_colorToggleAct->setChecked(config.value("singleColor").toBool());
  m_animationToggleAct->setChecked(config.value("animation").toBool());

  m_toneToolbar->setVisible(config.value("toneAnalysis").toBool());

  config.endGroup();
}


void
MainWindow::saveConfigurationAll(QSettings& config) const
{
  qDebug() << __PRETTY_FUNCTION__;

  saveConfiguration(config);

  for (auto iter = m_configurationGroups.begin(); iter != m_configurationGroups.end(); ++iter)
  {
    (*iter)->saveConfiguration(config);
  }
}


void
MainWindow::loadConfigurationAll(QSettings& config)
{
  qDebug() << __PRETTY_FUNCTION__ << " - loading config file: " << config.fileName();

  loadConfiguration(config);

  for (auto iter = m_configurationGroups.begin(); iter != m_configurationGroups.end(); ++iter)
  {
    (*iter)->loadConfiguration(config);
  }
}


void
MainWindow::loadUserOrDefaultConfig()
{
  if (QFile::exists(m_userConfigFilename))
  {
    QSettings config(m_userConfigFilename, QSettings::NativeFormat);
    qDebug() << __PRETTY_FUNCTION__ << " - user config: " << m_userConfigFilename;
    loadConfigurationAll(config);
  }
  else
  {
    QSettings config(m_defaultConfigFilename, QSettings::NativeFormat);
    qDebug() << __PRETTY_FUNCTION__ << " - default config: " << m_defaultConfigFilename;
    loadConfigurationAll(config);
  }
}


void
MainWindow::checkConnection() const
{
  if (m_player->isConnected())
  {
    return;
  }

  SerialPortConnectionDialog dialog;
  if (dialog.exec())
  {
    m_player->connect(dialog.getSelectedPortName().toStdString());
  }
}
