#include "player.h"
#include "basic/universalsleep.h"

#include <QTime>
#include <QtSerialPort/QSerialPortInfo>


Player::Player(ControlSettings& controlSettings)
: m_ledController(),
  m_settings(controlSettings),
  m_lastFrame(0),
  m_mainAnimation(),
  m_recordedAnimation(),
  m_isRecording(false),
  m_animationFPS(40),
  m_isPlaying(false),
  m_mutex()
{
  QStringList ports;
  for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
  {
    std::cout << port.portName().toStdString() << std::endl;
    ports += port.portName();
  }
  const QString serialPortName = "/dev/ttyACM0";
  //  const QString serialPortName = "COM7"; // windows
  m_ledController = createLedController(serialPortName);

  universalsleep::sleep_ms(2000); // let the LED strip initialise
}


Player::~Player()
{
  std::cout << "Player::~Player()" << std::endl;
}


std::unique_ptr<LEDController>
Player::createLedController(QString serialPortName)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  std::unique_ptr<LEDController> ledController;
  ledController->connect(serialPortName.toStdString());
  return ledController;
}


int
Player::getFPS() const
{
  return m_ledController->getFPS();
}


Frame
Player::getLastFrame() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_lastFrame;
}


void
Player::addAnimation(const Animation& animation)
{
  std::lock_guard<std::mutex> lock(m_mutex);

//  QElapsedTimer time;
  if (!m_mainAnimation.getFrames().empty())
  {
    m_mainAnimation = animation.combineTwoAnimations(animation, m_mainAnimation);
  }
  else
  {
    m_mainAnimation = animation;
  }
//  std::cout << "Player::addAnimation() - time :" << time.elapsed() << std::endl;
}


void
Player::playAllAnimations()
{
  m_isPlaying = true;

  QElapsedTimer timer;
  timer.start();
  int msPerFrame = 1000.0/m_animationFPS;

  while (!m_mainAnimation.getFrames().empty())
  {
    int elapsed_ms = timer.nsecsElapsed()/1000000.0;
    unsigned int toSleep = std::max( 0, msPerFrame - elapsed_ms );
    universalsleep::sleep_ms(toSleep);
    timer.restart();
    playFrame();
  }
}


void
Player::stopAnimations()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_isPlaying = false;
  m_mainAnimation.clearFrames();
}


bool
Player::isPlaying() const
{
  return m_isPlaying;
}


void
Player::playFrame()
{
  Frame frame(0);
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    const std::deque<Frame>& frames = m_mainAnimation.getFrames();
    if (!frames.empty())
    {
      m_lastFrame = frames.front();
      m_mainAnimation.pop_frontFrame();
      frame = m_lastFrame;
      frame.setOffset(m_settings.positionOffset);
      frame.setBrightness(m_settings.brightness);
    }

    if (m_isRecording)
    {
      m_recordedAnimation.addFrame(frame);
    }
  }

  m_ledController->send(frame); // this takes about 4-10 ms
}


void
Player::startRecording()
{
  m_isRecording = true;
}


void
Player::stopRecording()
{
  m_isRecording = false;
}


Animation
Player::getRecordedAnimation() const
{
  return m_recordedAnimation;
}


void
Player::clearRecordedAnimation()
{
  m_recordedAnimation.clearFrames();
}


Frame
Player::smoothenFrames(const Frame& firstFrame, const Frame& secondFrame, int /*nFrames*/)
{
  // NOTE: Not tested very well...
  std::size_t nLEDs = firstFrame.getLEDs().size();

  Frame frameSmooth(nLEDs);

  for (std::size_t i = 1; i <= nLEDs; ++i)
  {
    int r;
    int g;
    int b;

    std::vector<LED> ledsFirst = firstFrame.getLEDs();
    std::vector<LED> ledsSecond = secondFrame.getLEDs();
    Color colorFirst = ledsFirst[i].getColor();
    Color colorSecond = ledsSecond[i].getColor();
    r = (colorFirst.r + colorSecond.r)/2;
    g = (colorFirst.g + colorSecond.g)/2;
    b = (colorFirst.b + colorSecond.b)/2;
    Color colorNew(r, g, b);

    LED ledNew(i, colorNew);
    ledNew.setColor(colorNew);
    frameSmooth.addLED(ledNew);
  }

  return frameSmooth;
}
