#ifndef SERIALPORTCONNECTIONDIALOG_H
#define SERIALPORTCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class SerialPortConnectionDialog;
}

class SerialPortConnectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SerialPortConnectionDialog(QWidget *parent = 0);
  ~SerialPortConnectionDialog();

  QString getSelectedPortName() const;

private:
  Ui::SerialPortConnectionDialog *ui;
};

#endif // SERIALPORTCONNECTIONDIALOG_H
