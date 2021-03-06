#include "serialportconnectiondialog.h"
#include "ui_serialportconnectiondialog.h"

#include <QtSerialPort/QSerialPortInfo>

#include <iostream>


SerialPortConnectionDialog::SerialPortConnectionDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SerialPortConnectionDialog)
{
  ui->setupUi(this);

  for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
  {
    std::cout << port.systemLocation().toStdString() << std::endl;
    ui->serialPortComboBox->addItem(port.systemLocation());
  }
}


SerialPortConnectionDialog::~SerialPortConnectionDialog()
{
  delete ui;
}


QString
SerialPortConnectionDialog::getSelectedPortName() const
{
  return ui->serialPortComboBox->currentText();
}
