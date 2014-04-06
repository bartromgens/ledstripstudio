#include "actionconsistency.h"

ActionConsistency::ActionConsistency()
{
}


void
ActionConsistency::toggleSpectrumAnalaysis(bool isChecked)
{
  if (isChecked)
  {
    m_toneToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
    m_colorToggleAct->setChecked(false);
  }
  else
  {
    m_spectrumSettingsToggleAct->setVisible(false);
  }
  m_spectrumSettingsToggleAct->setVisible(isChecked);
}


void
ActionConsistency::toggleToneAnalysis(bool isChecked)
{
  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
    m_colorToggleAct->setChecked(false);
  }
}


void
ActionConsistency::toggleAnimation(bool isChecked)
{
  m_dotsAnimationAct->setVisible(isChecked);
  m_rainbowAnimationAct->setVisible(isChecked);
  m_imageAnimationAct->setVisible(isChecked);

  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_toneToggleButton->setChecked(false);
    m_colorToggleAct->setChecked(false);
  }
  else
  {
    m_dotsAnimationAct->setChecked(false);
    m_rainbowAnimationAct->setChecked(false);
    m_imageAnimationAct->setChecked(false);
  }
}


void
ActionConsistency::toggleSingleColor(bool isChecked)
{
  m_openColorPickerAct->setVisible(isChecked);

  if (isChecked)
  {
    m_spectrumToggleButton->setChecked(false);
    m_toneToggleButton->setChecked(false);
    m_animationToggleAct->setChecked(false);
  }
}
