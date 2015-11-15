#include "spectrumbarswidget.h"

#include "spectrum/powerspectrum.h"

#include <QPainter>


SpectrumBarsWidget::SpectrumBarsWidget(QWidget* parent)
: QWidget(parent),
  m_spectrum(),
  m_barWidth(1),
  m_nBars(500)
{
}


SpectrumBarsWidget::~SpectrumBarsWidget()
{
}


void
SpectrumBarsWidget::resizeEvent(QResizeEvent* e)
{
  Q_UNUSED(e);
  m_nBars = width()/m_barWidth;
}


void
SpectrumBarsWidget::setSpectrum(const std::vector<std::pair<double, double>>& spectrum)
{
  m_spectrum = PowerSpectrum::resizeSpectrum(spectrum, m_nBars, 2000.0);
//  m_spectrum = spectrum;
}


void
SpectrumBarsWidget::paintEvent(QPaintEvent* e)
{
  Q_UNUSED(e);
  if (m_spectrum.size() < 2)
  {
    return;
  }

  double binWidth = m_spectrum[1].first - m_spectrum[0].first;
  double plotWidth = m_barWidth*m_spectrum.size();

  QPainter qp(this);
  for (std::size_t i = 0; i < m_spectrum.size(); ++i)
  {
    int barHeight = m_spectrum[i].second;
    QRect rect(i*m_barWidth, height()-20, m_barWidth, -barHeight);
    qp.drawRect(rect);
  }
  qp.drawText(QPoint(0, height()), QString("0 Hz"));
  QString maxFrequencyLabel = QString::number(static_cast<int>(m_spectrum.back().first+binWidth)) + QString(" Hz");
  qp.drawText(QPoint(plotWidth-75, height()), maxFrequencyLabel);
}
