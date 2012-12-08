#ifndef PLAYER_H
#define PLAYER_H

#include "ledcontroller.h"

#include "src/basic/animation.h"

#include <memory>
#include <mutex>

class Player
{
public:
  Player();
  ~Player();

  std::unique_ptr<LEDController> createLedController(QString serialPortName);

  void addAnimation(const Animation &animation);

  void playFrame();

  int getFPS() const;
  Frame getLastFrame() const;

  // experimental
  Frame smoothenFrames(const Frame &firstFrame, const Frame &secondFrame, int nFrames = 1);

  void playAllAnimations();

private:
  std::unique_ptr<LEDController> m_ledController;
  Frame m_lastFrame;
  Animation m_mainAnimation;
  mutable std::mutex m_mutex;
};

#endif // PLAYER_H
