#include "actionconsistency.h"


ActionConsistency::ActionConsistency()
{
}


void
ActionConsistency::toggleSpectrumAnalaysis(bool isChecked)
{
  m_spectrumSettingsToggleAct->setVisible(isChecked);
}


void
ActionConsistency::toggleAnimation(bool isChecked)
{
  m_dotsAnimationAct->setVisible(isChecked);
  m_rainbowAnimationAct->setVisible(isChecked);
  m_imageAnimationAct->setVisible(isChecked);
}


void
ActionConsistency::toggleSingleColor(bool isChecked)
{
  m_openColorPickerAct->setVisible(isChecked);
}
