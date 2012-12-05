#ifndef LEDSTRIPEMULATOR_H
#define LEDSTRIPEMULATOR_H

#include "src/basic/frame.h"

#include <QWidget>

class LedStripEmulator : public QWidget
{
  Q_OBJECT

public:
  LedStripEmulator(QWidget* parent = 0);
  ~LedStripEmulator();

  void setFrame(const Frame& frame);
  void setHeight(int height);
  void setWidth(int width);

protected:
  void paintEvent(QPaintEvent* event);
  void drawStrip(QPainter* qp);

private:
  Frame m_frame;
  int m_height;
  int m_width;
};

#endif // LEDSTRIPEMULATOR_H
