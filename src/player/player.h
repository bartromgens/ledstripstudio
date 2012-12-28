#ifndef PLAYER_H
#define PLAYER_H

#include "ledcontroller.h"

#include "basic/animation.h"

#include <memory>
//#include <mutex>

class Player
{
public:
  Player();
  ~Player();

  void addAnimation(const Animation &animation);

  void playFrame();

  int getFPS() const;
  Frame getLastFrame() const;

  void playAllAnimations();

private:
  std::unique_ptr<LEDController> createLedController(QString serialPortName);

  void playFrameThread();

  Frame smoothenFrames(const Frame &firstFrame, const Frame &secondFrame, int nFrames = 1);

private:
  std::unique_ptr<LEDController> m_ledController;
  Frame m_lastFrame;
  Animation m_mainAnimation;
  mutable boost::mutex m_mutex;
};

#endif // PLAYER_H
