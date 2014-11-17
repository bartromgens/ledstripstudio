#ifndef LEDSTRIPEMULATOR_H
#define LEDSTRIPEMULATOR_H

#include "basic/frame.h"

#include <QWidget>

class LedStripEmulator : public QWidget
{
  Q_OBJECT

public:
  LedStripEmulator(QWidget* parent = 0);
  virtual ~LedStripEmulator();

  void setFrame(const Frame& frame);
  void setHeight(int height);
  void setWidth(int width);

protected:
  virtual void paintEvent(QPaintEvent* event);
  void drawStrip(QPainter* qp);

private:
  Frame m_frame;
  int m_height;
  int m_width;
};

#endif // LEDSTRIPEMULATOR_H
