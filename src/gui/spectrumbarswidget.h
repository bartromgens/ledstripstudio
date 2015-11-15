#ifndef SPECTRUMBARSWIDGET_H
#define SPECTRUMBARSWIDGET_H

#include <QWidget>


class SpectrumBarsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SpectrumBarsWidget(QWidget* parent);
  virtual ~SpectrumBarsWidget();

  void setSpectrum(const std::vector<std::pair<double, double>>& spectrum);

  virtual void paintEvent(QPaintEvent* e);

protected:
  virtual void resizeEvent(QResizeEvent* e);

private:
  std::vector<std::pair<double, double>> m_spectrum;
  int m_barWidth;
  int m_nBars;
};

#endif // SPECTRUMBARSWIDGET_H
