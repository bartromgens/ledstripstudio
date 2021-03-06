#ifndef PLAYER_H
#define PLAYER_H

#include "ledcontroller.h"

#include "basic/animation.h"
#include "settings/controlsettings.h"

#include <string>
#include <memory>


class Player
{

public:

  Player(ControlSettings& controlSettings);
  ~Player();

  bool connect(const std::string& serialPortName);
  bool isConnected() const;

  void addAnimation(const Animation& animation);

  void playFrame();

  bool isPlaying() const;

  void playAllAnimations();
  void stopAnimations();

  int getFPS() const;
  Frame getLastFrame() const;

  void startRecording();
  void stopRecording();

  Animation getRecordedAnimation() const;
  void clearRecordedAnimation();

private:

  static Frame smoothenFrames(const Frame& firstFrame, const Frame& secondFrame, int nFrames = 1);

private:

  std::unique_ptr<LEDController> m_ledController;
  ControlSettings& m_settings;
  Frame m_lastFrame;
  Animation m_mainAnimation;

  Animation m_recordedAnimation;
  std::atomic<bool> m_isRecording;
  unsigned int m_animationFPS;

  std::atomic<bool> m_isPlaying;

  mutable std::mutex m_mutex;

};

#endif // PLAYER_H
