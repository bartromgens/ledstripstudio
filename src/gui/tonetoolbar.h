#ifndef TONETOOLBAR_H
#define TONETOOLBAR_H

#include "studio/tonestudio.h"

#include <QAction>
#include <QString>
#include <QToolBar>

#include <memory>

class ToneToolbar : public QObject
{
  Q_OBJECT

public:
  ToneToolbar(std::shared_ptr<ToneStudio> toneStudio);

  void initialise(QToolBar* parentToolbar);

  void toggleToneAnalysis(bool isChecked);

  void updateToneSettingsVisibility(ToneStudio::ToneAnimationType type);

private:

private slots:
  void slotToggleStepTone(bool isChecked);
  void slotToggleSmoothTone(bool isChecked);
  void slotToggleToneHistory(bool isChecked);
  void slotToggleIndividualTone(bool isChecked);
  void slotToggleCornerTone(bool isChecked);

private:
  QToolBar* m_parentToolbar;

  std::shared_ptr<ToneStudio> m_toneStudio;

  QAction* m_stepToneAct;
  QAction* m_smoothToneAct;
  QAction* m_historyToneAct;
  QAction* m_individualToneAct;
  QAction* m_cornerToneAct;
};

#endif // TONETOOLBAR_H
