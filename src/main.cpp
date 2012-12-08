#include "gui/mainwindow.h"

//#include "windows.h"

#include <iostream>
#include <QApplication>
#include <random>
#include <ctime>

int main(int argc,char *argv[])
{
  srand(time(NULL));

  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("BartRomgens");
  QCoreApplication::setOrganizationDomain("bartromgens.org");
  QCoreApplication::setApplicationName("LedStrip");

  MainWindow window;
  window.show();

  return app.exec();
}

