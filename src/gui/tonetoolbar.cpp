#include "tonetoolbar.h"

#include <QSettings>

#include <memory>

ToneToolbar::ToneToolbar(ToneStudio& toneStudio)
  : QObject(),
    ConfigurationGroup(),
    m_toneStudio(toneStudio)
{
}

ToneToolbar::~ToneToolbar()
{

}


void
ToneToolbar::initialise(QToolBar* parentToolbar)
{
  m_stepToneAct = new QAction(parentToolbar);
  m_stepToneAct->setIcon(QIcon("./icons/step-tone-setting.png"));
  m_stepToneAct->setStatusTip(tr("Set loudest tone mode."));
  m_stepToneAct->setCheckable(true);
  m_stepToneAct->setVisible(false);
  connect(m_stepToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleStepTone(bool)));

  m_smoothToneAct = new QAction(parentToolbar);
  m_smoothToneAct->setIcon(QIcon("./icons/smooth-tone-setting.png"));
  m_smoothToneAct->setStatusTip(tr("Set smooth tone mode."));
  m_smoothToneAct->setCheckable(true);
  connect(m_smoothToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleSmoothTone(bool)));
  m_smoothToneAct->setVisible(false);

  m_historyToneAct = new QAction(parentToolbar);
  m_historyToneAct->setIcon(QIcon("./icons/tone-animation-history.png"));
  m_historyToneAct->setStatusTip(tr("Set history tone mode."));
  m_historyToneAct->setCheckable(true);
  connect(m_historyToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleToneHistory(bool)));
  m_historyToneAct->setVisible(false);

  m_individualToneAct = new QAction(parentToolbar);
  m_individualToneAct->setIcon(QIcon("./icons/tone-animation-individual.png"));
  m_individualToneAct->setStatusTip(tr("Set individual tone mode."));
  m_individualToneAct->setCheckable(true);
  connect(m_individualToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleIndividualTone(bool)));
  m_individualToneAct->setVisible(false);

  m_cornerToneAct = new QAction(parentToolbar);
  m_cornerToneAct->setIcon(QIcon("./icons/tone-animation-corner.png"));
  m_cornerToneAct->setStatusTip(tr("Set corner tone mode."));
  m_cornerToneAct->setCheckable(true);
  connect(m_cornerToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleCornerTone(bool)));
  m_cornerToneAct->setVisible(false);

  parentToolbar->addAction(m_historyToneAct);
  parentToolbar->addAction(m_cornerToneAct);
  parentToolbar->addAction(m_individualToneAct);
  parentToolbar->addAction(m_smoothToneAct);
  parentToolbar->addAction(m_stepToneAct);

  m_historyToneAct->setChecked(true);
}


void
ToneToolbar::toggleToneAnalysis(bool isChecked)
{
  m_stepToneAct->setVisible(isChecked);
  m_smoothToneAct->setVisible(isChecked);
  m_historyToneAct->setVisible(isChecked);
  m_individualToneAct->setVisible(isChecked);
  m_cornerToneAct->setVisible(isChecked);
}


void
ToneToolbar::updateToneSettingsVisibility(ToneStudio::ToneAnimationType type)
{
  if (type != ToneStudio::Loudest)
  {
    m_stepToneAct->setChecked(false);
  }
  if (type != ToneStudio::SmoothSum)
  {
    m_smoothToneAct->setChecked(false);
  }
  if (type != ToneStudio::History)
  {
    m_historyToneAct->setChecked(false);
  }
  if (type != ToneStudio::Individual)
  {
    m_individualToneAct->setChecked(false);
  }
  if (type != ToneStudio::Corner)
  {
    m_cornerToneAct->setChecked(false);
  }
}


void
ToneToolbar::slotToggleStepTone(bool isChecked)
{
  if (isChecked)
  {
    updateToneSettingsVisibility(ToneStudio::Loudest);
    m_toneStudio.setAnimationType(ToneStudio::Loudest);
  }
  else
  {
    m_toneStudio.setAnimationType(ToneStudio::None);
  }
}


void
ToneToolbar::slotToggleSmoothTone(bool isChecked)
{
  if (isChecked)
  {
    updateToneSettingsVisibility(ToneStudio::SmoothSum);
    m_toneStudio.setAnimationType(ToneStudio::SmoothSum);
  }
  else
  {
    m_toneStudio.setAnimationType(ToneStudio::None);
  }
}


void
ToneToolbar::slotToggleToneHistory(bool isChecked)
{
  if (isChecked)
  {
    updateToneSettingsVisibility(ToneStudio::History);
    m_toneStudio.setAnimationType(ToneStudio::History);
  }
  else
  {
    m_toneStudio.setAnimationType(ToneStudio::None);
  }
}


void
ToneToolbar::slotToggleIndividualTone(bool isChecked)
{
  if (isChecked)
  {
    updateToneSettingsVisibility(ToneStudio::Individual);
    m_toneStudio.setAnimationType(ToneStudio::Individual);
  }
  else
  {
    m_toneStudio.setAnimationType(ToneStudio::None);
  }
}


void
ToneToolbar::slotToggleCornerTone(bool isChecked)
{
  if (isChecked)
  {
    updateToneSettingsVisibility(ToneStudio::Corner);
    m_toneStudio.setAnimationType(ToneStudio::Corner);
  }
  else
  {
    m_toneStudio.setAnimationType(ToneStudio::None);
  }
}


void
ToneToolbar::saveConfiguration(QSettings& config) const
{
  config.beginGroup( "Tone" );

  config.setValue("ToneAnimationType", m_toneStudio.getAnimationType());

  config.endGroup();
}


void
ToneToolbar::loadConfiguration(QSettings& config)
{
  config.beginGroup( "Tone" );

  ToneStudio::ToneAnimationType toneType = static_cast<ToneStudio::ToneAnimationType>(config.value("ToneAnimationType", "").toInt());

  switch (toneType)
  {
    case ToneStudio::None:
    {
      break;
    }
    case ToneStudio::Loudest:
    {
      m_stepToneAct->setChecked(true);
      break;
    }
    case ToneStudio::SmoothSum:
    {
      m_smoothToneAct->setChecked(true);
      break;
    }
    case ToneStudio::History:
    {
      m_historyToneAct->setChecked(true);
      break;
    }
    case ToneStudio::Individual:
    {
      m_individualToneAct->setChecked(true);
      break;
    }
    case ToneStudio::Corner:
    {
      m_cornerToneAct->setChecked(true);
      break;
    }
  }

  config.endGroup();
}
