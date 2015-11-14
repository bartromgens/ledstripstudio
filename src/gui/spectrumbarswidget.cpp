#include "spectrumbarswidget.h"

#include <QPainter>


SpectrumBarsWidget::SpectrumBarsWidget(QWidget* parent)
: QWidget(parent),
  m_spectrum()
{
}


SpectrumBarsWidget::~SpectrumBarsWidget()
{
}


void
SpectrumBarsWidget::setSpectrum(const std::vector<std::pair<double, double>>& spectrum)
{
  m_spectrum = spectrum;
}


void
SpectrumBarsWidget::paintEvent(QPaintEvent* e)
{
  Q_UNUSED(e);
  if (m_spectrum.empty())
  {
    return;
  }

  int barWidth = 2;
  QPainter qp(this);
  for (std::size_t i = 0; i < m_spectrum.size(); ++i)
  {
    if (i > 600)
    {
      break;
    }
    int barHeight = m_spectrum[i].second;
    QRect rect(i*barWidth, height()-5, barWidth, -barHeight);
    qp.drawRect(rect);
  }
}
