#ifndef TONETOOLBAR_H
#define TONETOOLBAR_H

#include "settings/configurationgroup.h"
#include "studio/tonestudio.h"

#include <QAction>
#include <QString>
#include <QToolBar>

#include <memory>

class ToneToolbar : public QToolBar, public ConfigurationGroup
{
  Q_OBJECT

public:

  ToneToolbar(ToneStudio& toneStudio, QWidget* parent);
  virtual ~ToneToolbar();

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

private slots:

  void slotToneModeTriggered() const;

private:

  void initialise();
  ToneStudio::AnimationType getAnimationType() const;

private:

  ToneStudio& m_toneStudio;

  QAction* m_stepToneAct;
  QAction* m_smoothToneAct;
  QAction* m_historyToneAct;
  QAction* m_rangeToneAct;
  QAction* m_individualToneAct;

  QActionGroup m_actionGroup;
};

#endif // TONETOOLBAR_H
