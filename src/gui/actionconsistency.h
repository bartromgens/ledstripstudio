#ifndef ACTIONCONSISTENCY_H
#define ACTIONCONSISTENCY_H

#include <QAction>

class ActionConsistency
{
public:

  ActionConsistency();

  void toggleSingleColor(bool isChecked);
  void toggleAnimation(bool isChecked);

public:

  QAction* m_dotsAnimationAct;
  QAction* m_rainbowAnimationAct;
  QAction* m_imageAnimationAct;
  QAction* m_openColorPickerAct;

};

#endif // ACTIONCONSISTENCY_H
