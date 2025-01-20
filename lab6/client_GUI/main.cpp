#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  Client cl;

  QApplication a(argc, argv);
  MainWindow w(&cl);
  w.show();
  return a.exec();
}
