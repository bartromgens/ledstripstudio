#ifndef LEDSTRIPSTATUSWIDGET_H
#define LEDSTRIPSTATUSWIDGET_H

#include "basic/frame.h"

#include <QWidget>

namespace Ui {
class LedStripStatusWidget;
}

class LedStripStatusWidget : public QWidget
{
  Q_OBJECT
  
public:
  explicit LedStripStatusWidget(QWidget *parent = 0);
  ~LedStripStatusWidget();

  void update(const Frame& frame);
  
  void setFPS(int fps);

private:
  Ui::LedStripStatusWidget *ui;

  int m_fps;
};

#endif // LEDSTRIPSTATUSWIDGET_H
