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

private:
  std::vector<std::pair<double, double>> m_spectrum;
};

#endif // SPECTRUMBARSWIDGET_H
