#include "ledstripstatuswidget.h"
#include "ui_ledstripstatuswidget.h"

LedStripStatusWidget::LedStripStatusWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LedStripStatusWidget),
  m_fps(0)
{
  ui->setupUi(this);

  connect(ui->enableEmulator, SIGNAL(toggled(bool)), this, SLOT(slotEmulatorToggled(bool)));
  connect(ui->enableEmulatorRGB, SIGNAL(toggled(bool)), this, SLOT(slotEmulatorRGBToggled(bool)));
  ui->enableEmulatorRGB->setChecked(false);
}


LedStripStatusWidget::~LedStripStatusWidget()
{
  delete ui;
}


void
LedStripStatusWidget::slotEmulatorRGBToggled(bool isChecked)
{
  ui->frameRGB->setVisible(isChecked);
}


void
LedStripStatusWidget::slotEmulatorToggled(bool isChecked)
{
  ui->frame->setVisible(isChecked);
}


void
LedStripStatusWidget::update(const Frame& frame)
{
  if (frame.getLEDs().empty())
  {
    return;
  }

  int nLeds = frame.getLEDs().size();

  if (ui->enableEmulator->isChecked())
  {
    ui->ledStripEmulator->setHeight(ui->ledStripEmulator->size().height());
    ui->ledStripEmulator->setWidth(ui->ledStripEmulator->size().width()/nLeds);
    ui->ledStripEmulator->setFrame(frame);
    ui->ledStripEmulator->update();
  }

  if (ui->enableEmulatorRGB->isChecked())
  {
    ui->ledStripEmulatorRed->setHeight(ui->ledStripEmulatorRed->size().height());
    ui->ledStripEmulatorRed->setWidth(ui->ledStripEmulatorRed->size().width()/nLeds);
    ui->ledStripEmulatorGreen->setHeight(ui->ledStripEmulatorGreen->size().height());
    ui->ledStripEmulatorGreen->setWidth(ui->ledStripEmulatorGreen->size().width()/nLeds);
    ui->ledStripEmulatorBlue->setHeight(ui->ledStripEmulatorBlue->size().height());
    ui->ledStripEmulatorBlue->setWidth(ui->ledStripEmulatorBlue->size().width()/nLeds);

    Frame frameR = frame;
    Frame frameG = frame;
    Frame frameB = frame;

    frameR.amplifyRGB(1.0, 0.0, 0.0);
    frameG.amplifyRGB(0.0, 1.0, 0.0);
    frameB.amplifyRGB(0.0, 0.0, 1.0);

    ui->ledStripEmulatorRed->setFrame(frameR);
    ui->ledStripEmulatorGreen->setFrame(frameG);
    ui->ledStripEmulatorBlue->setFrame(frameB);

    ui->ledStripEmulatorRed->update();
    ui->ledStripEmulatorGreen->update();
    ui->ledStripEmulatorBlue->update();
  }
}


void
LedStripStatusWidget::setFPS(int fps)
{
  m_fps = fps;
  ui->fpsLcd->setText(QString::number(fps));
}
