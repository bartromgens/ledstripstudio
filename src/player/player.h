#ifndef PLAYER_H
#define PLAYER_H

#include "ledcontroller.h"

#include <memory>
#include <mutex>

class Animation;

class Player
{
public:
  Player();
  ~Player();

  std::unique_ptr<LEDController> createLedController(QString serialPortName);

  void play(const Animation& animation);
  Frame smoothenFrames(const Frame &firstFrame, const Frame &secondFrame, int nFrames = 1);
  int getFPS() const;
private:
  std::unique_ptr<LEDController> m_ledController;
  std::mutex m_mutex;
};

#endif // PLAYER_H
