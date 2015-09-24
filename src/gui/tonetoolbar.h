#ifndef TONETOOLBAR_H
#define TONETOOLBAR_H

#include "settings/configurationgroup.h"
#include "studio/tonestudio.h"

#include <QAction>
#include <QString>
#include <QToolBar>

#include <memory>

class ToneToolbar : public QObject, public ConfigurationGroup
{
  Q_OBJECT

public:

  ToneToolbar(ToneStudio& toneStudio);
  virtual ~ToneToolbar();

  void initialise(QToolBar* parentToolbar);

  void toggleToneAnalysis(bool isChecked);

  ToneStudio::AnimationType getAnimationType() const;

  virtual void saveConfiguration(QSettings& config) const;
  virtual void loadConfiguration(QSettings& config);

private:

  ToneStudio& m_toneStudio;

  QAction* m_stepToneAct;
  QAction* m_smoothToneAct;
  QAction* m_historyToneAct;
  QAction* m_individualToneAct;
  QAction* m_cornerToneAct;

  QActionGroup m_actionGroup;
};

#endif // TONETOOLBAR_H
