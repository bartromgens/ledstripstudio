#include "tonetoolbar.h"

#include <QSettings>

#include <memory>

ToneToolbar::ToneToolbar(ToneStudio& toneStudio)
: QObject(),
  ConfigurationGroup(),
  m_toneStudio(toneStudio),
  m_actionGroup(this)
{
}

ToneToolbar::~ToneToolbar()
{
}


void
ToneToolbar::initialise(QToolBar* parentToolbar)
{
  m_stepToneAct = new QAction(parentToolbar);
  m_stepToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::Loudest)));
  m_stepToneAct->setIcon(QIcon("./icons/step-tone-setting.png"));
  m_stepToneAct->setStatusTip(tr("Set loudest tone mode."));
  m_stepToneAct->setCheckable(true);
  m_stepToneAct->setVisible(false);
  connect(m_stepToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleStepTone(bool)));

  m_smoothToneAct = new QAction(parentToolbar);
  m_smoothToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::SmoothSum)));
  m_smoothToneAct->setIcon(QIcon("./icons/smooth-tone-setting.png"));
  m_smoothToneAct->setStatusTip(tr("Set smooth tone mode."));
  m_smoothToneAct->setCheckable(true);
  connect(m_smoothToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleSmoothTone(bool)));
  m_smoothToneAct->setVisible(false);

  m_historyToneAct = new QAction(parentToolbar);
  m_historyToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::History)));
  m_historyToneAct->setIcon(QIcon("./icons/tone-animation-history.png"));
  m_historyToneAct->setStatusTip(tr("Set history tone mode."));
  m_historyToneAct->setCheckable(true);
  connect(m_historyToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleToneHistory(bool)));
  m_historyToneAct->setVisible(false);

  m_individualToneAct = new QAction(parentToolbar);
  m_individualToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::Individual)));
  m_individualToneAct->setIcon(QIcon("./icons/tone-animation-individual.png"));
  m_individualToneAct->setStatusTip(tr("Set individual tone mode."));
  m_individualToneAct->setCheckable(true);
  connect(m_individualToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleIndividualTone(bool)));
  m_individualToneAct->setVisible(false);

  m_cornerToneAct = new QAction(parentToolbar);
  m_cornerToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::Corner)));
  m_cornerToneAct->setIcon(QIcon("./icons/tone-animation-corner.png"));
  m_cornerToneAct->setStatusTip(tr("Set corner tone mode."));
  m_cornerToneAct->setCheckable(true);
  connect(m_cornerToneAct, SIGNAL(toggled(bool)), this, SLOT(slotToggleCornerTone(bool)));
  m_cornerToneAct->setVisible(false);

  m_actionGroup.addAction(m_stepToneAct);
  m_actionGroup.addAction(m_smoothToneAct);
  m_actionGroup.addAction(m_historyToneAct);
  m_actionGroup.addAction(m_individualToneAct);
  m_actionGroup.addAction(m_cornerToneAct);

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


ToneStudio::AnimationType
ToneToolbar::getAnimationType() const
{
  if (m_actionGroup.checkedAction())
  {
    return static_cast<ToneStudio::AnimationType>( m_actionGroup.checkedAction()->data().toInt() );
  }
  return ToneStudio::AnimationType::None;
}


void
ToneToolbar::saveConfiguration(QSettings& config) const
{
  config.beginGroup( "Tone" );

  config.setValue("ToneAnimationType", static_cast<int>( getAnimationType() ));

  config.endGroup();
}


void
ToneToolbar::loadConfiguration(QSettings& config)
{
  config.beginGroup( "Tone" );

  ToneStudio::AnimationType toneType =
      static_cast<ToneStudio::AnimationType>(config.value("ToneAnimationType", "").toInt());

  switch (toneType)
  {
    case ToneStudio::AnimationType::None:
    {
      break;
    }
    case ToneStudio::AnimationType::Loudest:
    {
      m_stepToneAct->setChecked(true);
      break;
    }
    case ToneStudio::AnimationType::SmoothSum:
    {
      m_smoothToneAct->setChecked(true);
      break;
    }
    case ToneStudio::AnimationType::History:
    {
      m_historyToneAct->setChecked(true);
      break;
    }
    case ToneStudio::AnimationType::Individual:
    {
      m_individualToneAct->setChecked(true);
      break;
    }
    case ToneStudio::AnimationType::Corner:
    {
      m_cornerToneAct->setChecked(true);
      break;
    }
  }

  config.endGroup();
}
