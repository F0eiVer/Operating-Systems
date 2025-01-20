#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include "client.h"
#include "qcustomplot.h"
#include <QDebug>
#include <QHeaderView>
#include <QLayout>
#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(Client *client, QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_connect_button_clicked();

  void on_table_button_clicked();

  void on_graphic_button_clicked();

private:
  Ui::MainWindow *ui;
  QCustomPlot *customPlot;
  QCPGraph *graphic;
  Client *cl;
  bool tableB;
  bool graphicB;
};
#endif // MAINWINDOW_H
