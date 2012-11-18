#include "gui/mainwindow.h"

//#include "windows.h"

#include <iostream>
#include <QApplication>

int main(int argc,char *argv[])
{
  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}

