#include "player.h"
#include "basic/universalsleep.h"

Player::Player()
  : m_ledController(),
    m_lastFrame(0),
    m_mainAnimation(),
    m_mutex()
{
  const QString serialPortName = "/dev/ttyACM0";
  //  const QString serialPortName = "COM11"; // windows
  m_ledController = createLedController(serialPortName);

  universalsleep::sleep_ms(2000); // let the LED strip initialise
}


Player::~Player()
{
}


std::unique_ptr<LEDController> Player::createLedController(QString serialPortName)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  std::unique_ptr<LEDController> ledController( new LEDController() );
  ledController->setSerialPortName(serialPortName);
  ledController->connect();
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
  boost::lock_guard<boost::mutex> lock(m_mutex);
  return m_lastFrame;
}


void
Player::addAnimation(const Animation& animation)
{
  boost::lock_guard<boost::mutex> lock(m_mutex);

  QTime time;
  time.start();
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
  while (!m_mainAnimation.getFrames().empty())
  {
    playFrameThread();
  }
}


void
Player::playFrame()
{
//  std::cout << "Player::playFrame()" << std::endl;

//  QTime time;
//  time.start();
  boost::thread t1(&Player::playFrameThread, this);
  t1.detach();

  //  std::cout << "Player::playFrame() - time :" << time.elapsed() << std::endl;
}


void
Player::playFrameThread()
{
  {
    boost::lock_guard<boost::mutex> lock(m_mutex);

    const std::deque<Frame>& frames = m_mainAnimation.getFrames();
    if (!frames.empty())
    {
      m_lastFrame = frames.front();
      m_mainAnimation.pop_frontFrame();
    }
  }

  m_ledController->send(m_lastFrame);
}

Frame
Player::smoothenFrames(const Frame& firstFrame, const Frame& secondFrame, int /*nFrames*/)
{
  // NOTE: Not teste very well...
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

//  while (!frames.empty())
//  {
//    Frame frame = frames.front();
//    animationNew.addFrame(frame);

//    std::vector<Frame> framesSmooth(nFrames, frame.getLEDs().size());

//    std::vector<LED> leds = frame.getLEDs();
//    for (std::vector<LED>::iterator iter = leds.begin();
//         iter != leds.end(); ++iter)
//    {
//      LED led = iter->second;
//      for (std::size_t i = 0; i < framesSmooth.size(); i++)
//      {
//        LED ledNew;
//        ledNew.setLEDnr(led.getLEDnr());
//        ledNew.setColor( Color(led.getColor().r/(i+1), led.getColor().g/(i+1), led.getColor().b/(i+1)));
//        framesSmooth[i].addLED(ledNew);
//      }
//    }

//    for (std::size_t i = 0; i < framesSmooth.size(); ++i)
//    {
//      animationNew.addFrame(framesSmooth[i]);
//    }

//    frames.pop_front();
//  }

  return frameSmooth;
}
