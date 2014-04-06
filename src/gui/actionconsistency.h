#ifndef ACTIONCONSISTENCY_H
#define ACTIONCONSISTENCY_H

#include <QAction>

class ActionConsistency
{
public:
  ActionConsistency();

  void toggleSpectrumAnalaysis(bool isChecked);
  void toggleToneAnalysis(bool isChecked);
  void toggleSingleColor(bool isChecked);
  void toggleAnimation(bool isChecked);

public:
  QAction* m_stripToggleButton;
  QAction* m_audioToggleButton;
  QAction* m_spectrumToggleButton;
  QAction* m_toneToggleButton;
  QAction* m_animationToggleAct;
  QAction* m_colorToggleAct;

  QAction* m_spectrumSettingsToggleAct;

  QAction* m_dotsAnimationAct;
  QAction* m_rainbowAnimationAct;
  QAction* m_imageAnimationAct;
  QAction* m_recordAnimationAct;
  QAction* m_openColorPickerAct;

};

#endif // ACTIONCONSISTENCY_H
