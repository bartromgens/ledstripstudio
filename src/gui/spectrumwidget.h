#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <QWidget>

namespace Ui {
class SpectrumWidget;
}

class SpectrumWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SpectrumWidget(QWidget *parent = 0);
  virtual ~SpectrumWidget();

  void setSpectrum(const std::vector<std::pair<double, double>>& spectrum);

private:
  Ui::SpectrumWidget *ui;
};

#endif // SPECTRUMWIDGET_H
