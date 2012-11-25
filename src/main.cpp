#include "gui/mainwindow.h"

//#include "windows.h"

#include <iostream>
#include <QApplication>

int main(int argc,char *argv[])
{
  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("BartRomgens");
  QCoreApplication::setOrganizationDomain("bartromgens.org");
  QCoreApplication::setApplicationName("LedStrip");

  MainWindow window;
  window.show();

  return app.exec();
}

