#include "spectrumwidget.h"
#include "ui_spectrumwidget.h"


SpectrumWidget::SpectrumWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SpectrumWidget)
{
  ui->setupUi(this);
}


SpectrumWidget::~SpectrumWidget()
{
  delete ui;
}


void
SpectrumWidget::setSpectrum(const std::vector<std::pair<double, double>>& spectrum)
{
  ui->spectrumBarsWidget->setSpectrum(spectrum);
}
