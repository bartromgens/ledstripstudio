#include "player.h"
#include "src/basic/animation.h"
#include "src/basic/color.h"

#include <unistd.h>  // for (u)sleep on Linux

Player::Player()
  : m_ledController(),
    m_mutex()
{
  const QString serialPortName = "/dev/ttyACM0";
  //  const QString serialPortName = "COM11"; // windows
  m_ledController = createLedController(serialPortName);

  sleep(3); // let the LED strip initialise
}


Player::~Player()
{
}


std::unique_ptr<LEDController> Player::createLedController(QString serialPortName)
{
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


void
Player::play(const Animation& animation)
{
  m_mutex.lock();
  std::list<Frame> frames = animation.getFrames();
  while (!frames.empty())
  {
    Frame frame = frames.front();
    m_ledController->send(frame);
    frames.pop_front();
//    std::cout << "Player::play() - frames.size(): " << frames.size() << std::endl;
  }
  m_mutex.unlock();
}


Frame
Player::smoothenFrames(const Frame& firstFrame, const Frame& secondFrame, int /*nFrames*/)
{
  // NOTE: Not teste very well...
  std::size_t nLEDs = firstFrame.getLEDs().size();

  Frame frameSmooth(nLEDs);

  for (std::size_t i = 1; i <= nLEDs; ++i)
  {
    LED ledNew;
    ledNew.setLEDnr(i);
    int r;
    int g;
    int b;

    std::map<int, LED> ledsFirst = firstFrame.getLEDs();
    std::map<int, LED> ledsSecond = secondFrame.getLEDs();
    Color colorFirst = ledsFirst[i].getColor();
    Color colorSecond = ledsSecond[i].getColor();
    r = (colorFirst.r + colorSecond.r)/2;
    g = (colorFirst.g + colorSecond.g)/2;
    b = (colorFirst.b + colorSecond.b)/2;
    Color colorNew(r, g, b);

    ledNew.setColor(colorNew);
    frameSmooth.addLED(ledNew);
  }

//  while (!frames.empty())
//  {
//    Frame frame = frames.front();
//    animationNew.addFrame(frame);

//    std::vector<Frame> framesSmooth(nFrames, frame.getLEDs().size());

//    std::map<int, LED> leds = frame.getLEDs();
//    for (std::map<int, LED>::iterator iter = leds.begin();
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
