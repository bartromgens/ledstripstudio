#include "gui/mainwindow.h"

//#include "windows.h"

#include <iostream>
#include <QApplication>

int main(int argc,char *argv[])
{
  QCoreApplication::setOrganizationName("BartRomgens");
  QCoreApplication::setOrganizationDomain("bartromgens.org");
  QCoreApplication::setApplicationName("LedStrip");

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}

