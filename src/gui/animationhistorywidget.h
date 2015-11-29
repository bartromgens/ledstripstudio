#ifndef ANIMATIONHISTORYWIDGET_H
#define ANIMATIONHISTORYWIDGET_H

#include <basic/frame.h>

#include <QWidget>


class AnimationHistoryWidget : public QWidget
{
  Q_OBJECT

public:
  explicit AnimationHistoryWidget(QWidget* parent = 0);
  virtual ~AnimationHistoryWidget();

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

#endif // ANIMATIONHISTORYWIDGET_H
