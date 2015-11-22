#include "tonetoolbar.h"

#include <QSettings>

#include <memory>

ToneToolbar::ToneToolbar(ToneStudio& toneStudio, QWidget* parent)
: QToolBar(parent),
  ConfigurationGroup(),
  m_toneStudio(toneStudio),
  m_actionGroup(this)
{
  initialise();
}


ToneToolbar::~ToneToolbar()
{
}


void
ToneToolbar::initialise()
{
  m_stepToneAct = new QAction(this);
  m_stepToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::Loudest)));
  m_stepToneAct->setIcon(QIcon("./icons/step-tone-setting.png"));
  m_stepToneAct->setToolTip(tr("Loudest tone mode"));
  m_stepToneAct->setCheckable(true);

  m_smoothToneAct = new QAction(this);
  m_smoothToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::SmoothSum)));
  m_smoothToneAct->setIcon(QIcon("./icons/smooth-tone-setting.png"));
  m_smoothToneAct->setToolTip(tr("Smooth tone mode"));
  m_smoothToneAct->setCheckable(true);

  m_historyToneAct = new QAction(this);
  m_historyToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::History)));
  m_historyToneAct->setIcon(QIcon("./icons/tone-animation-history.png"));
  m_historyToneAct->setToolTip(tr("History tone mode"));
  m_historyToneAct->setCheckable(true);

  m_individualToneAct = new QAction(this);
  m_individualToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::Individual)));
  m_individualToneAct->setIcon(QIcon("./icons/tone-animation-individual.png"));
  m_individualToneAct->setToolTip(tr("Individual tone mode"));
  m_individualToneAct->setCheckable(true);

  m_cornerToneAct = new QAction(this);
  m_cornerToneAct->setData(QVariant(static_cast<int>(ToneStudio::AnimationType::Corner)));
  m_cornerToneAct->setIcon(QIcon("./icons/tone-animation-corner.png"));
  m_cornerToneAct->setToolTip(tr("Corner tone mode."));
  m_cornerToneAct->setCheckable(true);

  m_actionGroup.addAction(m_stepToneAct);
  m_actionGroup.addAction(m_smoothToneAct);
  m_actionGroup.addAction(m_historyToneAct);
  m_actionGroup.addAction(m_individualToneAct);
  m_actionGroup.addAction(m_cornerToneAct);

  addAction(m_historyToneAct);
  addAction(m_cornerToneAct);
  addAction(m_individualToneAct);
  addAction(m_smoothToneAct);
  addAction(m_stepToneAct);

  m_historyToneAct->setChecked(true);
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
