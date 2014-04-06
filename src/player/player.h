#ifndef PLAYER_H
#define PLAYER_H

#include "ledcontroller.h"

#include "basic/animation.h"
#include "settings/controlsettings.h"

#include <memory>
//#include <mutex>

class Player
{
public:
  Player(std::shared_ptr<ControlSettings> controlSettings);
  ~Player();

  void addAnimation(const Animation &animation);

  void playFrame();
//  void playFrameThread();

  void playAllAnimations();
  void stopAnimations();

  int getNAnimations() const;

  int getFPS() const;
  Frame getLastFrame() const;

  void startRecording();
  void stopRecording();

  bool isPlaying() const;

  void setAnimationFPS(unsigned int fps);

  Animation getRecordedAnimation() const;

  void clearRecordedAnimation();

private:
  std::unique_ptr<LEDController> createLedController(QString serialPortName);

  Frame smoothenFrames(const Frame &firstFrame, const Frame &secondFrame, int nFrames = 1);

private:
  std::unique_ptr<LEDController> m_ledController;
  std::shared_ptr<ControlSettings> m_settings;
  Frame m_lastFrame;
  Animation m_mainAnimation;

  Animation m_recordedAnimation;
  bool m_isRecording;
  unsigned int m_animationFPS;
//  boost::thread* m_animationThread;

  bool m_isPlaying;

  mutable std::mutex m_mutex;
};

#endif // PLAYER_H
