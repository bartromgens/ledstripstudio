#include "animationhistorywidget.h"

#include <QPainter>


AnimationHistoryWidget::AnimationHistoryWidget(QWidget* parent)
: QWidget(parent),
    m_frame(0),
    m_height(10),
    m_width(10)
{
}


AnimationHistoryWidget::~AnimationHistoryWidget()
{
}


void
AnimationHistoryWidget::setFrame(const Frame& frame)
{
  m_frame = frame;
}


void
AnimationHistoryWidget::setHeight(int height)
{
  m_height = height;
}


void
AnimationHistoryWidget::setWidth(int width)
{
  m_width = width;
}


void
AnimationHistoryWidget::paintEvent(QPaintEvent* e)
{
  Q_UNUSED(e);
  QPainter qp(this);
  drawStrip(&qp);
}


void
AnimationHistoryWidget::drawStrip(QPainter* qp)
{
  QPen pen(Qt::black, 2, Qt::SolidLine);
  qp->setPen(pen);

  const std::vector<LED>& leds = m_frame.getLEDs();

  int widgetWith = size().width();
  int border = (widgetWith - leds.size()*m_width)/2;

  for (std::size_t i = 0; i < leds.size(); ++i)
  {
    LED led = leds.at(i);
    int x = (i*m_width)+border;
    int y = 2;
    QRect rect(x, y, m_width, m_height-2);
    double magicFactor = 254/127.0*1.0;
    QColor color(std::max(int(led.getColor().r*magicFactor), 0),
                 std::max(int(led.getColor().g*magicFactor), 0),
                 std::max(int(led.getColor().b*magicFactor), 0));
    qp->drawRect(rect);
    qp->fillRect(rect, color);
  }
}
