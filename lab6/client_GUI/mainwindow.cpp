#include "mainwindow.h"
#include "./ui_mainwindow.h"

std::string get_ans(Client *cl, std::string message) {
  std::string input = "";
  char server_buf[255];
  memset(server_buf, 0, sizeof(server_buf));
  cl->Write(message.c_str(), message.size() + 1);
  while (cl->Read(server_buf, sizeof(server_buf) - 1) > 0) {
    input += server_buf;
  }
  input = input.substr(0, input.find("</html>") + 7);
  return input;
}

MainWindow::MainWindow(Client *client, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), cl(client), tableB(false),
      graphicB(false), customPlot(new QCustomPlot) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  cl->Disconnect();
  // Деинициализируем библиотеку сокетов (под Windows)
#if defined(WIN32)
  WSACleanup();
#endif
  delete ui;
}

void MainWindow::on_connect_button_clicked() {
  // Инициализируем библиотеку сокетов (под Windows)
#if defined(WIN32)
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
  // Игнорируем SIGPIPE сигнал
  // Чтобы программа не терминировалась при поптыке записи в закрытый сокет
  signal(SIGPIPE, SIG_IGN);
#endif

  if (cl->Connect(ui->server_ip->text().toStdString(),
                  ui->port->text().toInt())) {

    ui->connection_status->setText("Подключено");
    QPalette palette = ui->connection_status->palette();
    palette.setColor(QPalette::WindowText, Qt::green);
    ui->connection_status->setPalette(palette);

    QMessageBox::information(this, "Подключение установлено",
                             "Подключение к серверу прошло успешно");

  } else {
    QMessageBox::warning(
        this, "Подключение не установлено",
        "Проблемы с подключением к серверу, проверьте корректность данных");
  }
}

void MainWindow::on_table_button_clicked() {
  if (ui->connection_status->text() != "Подключено") {
    QMessageBox::warning(this, "Подключение не установлено",
                         "Перед получением данных, подключитесь к серверу");
    return;
  }
  if (tableB) {
    return;
  }
  tableB = true;

  QTableWidget *tableWidget = new QTableWidget(0, 2);
  QStringList lst;

  lst << "Температура"
      << "Время";
  tableWidget->setHorizontalHeaderLabels(lst);

  std::string user_string = "GET /";

  switch (ui->comboBox->currentIndex()) {
  case 0:
    user_string += "sec";
    break;
  case 1:
    user_string += "hour";
    break;
  case 2:
    user_string += "day";
    break;
  }
  //  std::string input = "";
  //  char server_buf[255];
  //  memset(server_buf, 0, sizeof(server_buf));
  //  cl->Write(user_string.c_str(), user_string.size() + 1);
  //  while (cl->Read(server_buf, sizeof(server_buf) - 1)) {
  //    input += server_buf;
  //  }
  //  input = input.substr(0, input.find("</html>") + 7);

  std::string input = get_ans(cl, user_string);

  size_t index1 = 0;
  size_t index2 = 0;
  while (true) {
    index1 = input.find("<td>", index2);
    index2 = input.find("</td>", index1);
    if (index1 == std::string::npos) {
      break;
    }
    tableWidget->insertRow(tableWidget->rowCount());
    tableWidget->setItem(
        tableWidget->rowCount() - 1, 0,
        new QTableWidgetItem(
            input.substr(index1 + 4, index2 - index1 - 4).c_str()));

    index1 = input.find("<td>", index2);
    index2 = input.find("</td>", index1);
    tableWidget->setItem(
        tableWidget->rowCount() - 1, 1,
        new QTableWidgetItem(
            input.substr(index1 + 4, index2 - index1 - 4).c_str()));
  }

  tableWidget->resizeColumnsToContents();
  tableWidget->resizeRowsToContents();
  ui->contentLayout->addWidget(tableWidget);
  cl->Disconnect();
  ui->connection_status->setText("Неподключено");
  QPalette palette = ui->connection_status->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->connection_status->setPalette(palette);
}

void MainWindow::on_graphic_button_clicked() {
  if (ui->connection_status->text() != "Подключено") {
    QMessageBox::warning(this, "Подключение не установлено",
                         "Перед получением данных, подключитесь к серверу");
    return;
  }
  if (graphicB) {
    return;
  }
  graphicB = true;
  ui->contentLayout->addWidget(customPlot);
  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);

  customPlot->setInteraction(
      QCP::iRangeZoom, true); // Включаем взаимодействие удаления/приближения
  customPlot->setInteraction(
      QCP::iRangeDrag, true); // Включаем взаимодействие перетаскивания графика
  customPlot->axisRect()->setRangeDrag(
      Qt::Horizontal); // Включаем перетаскивание только по горизонтальной оси
  customPlot->axisRect()->setRangeZoom(
      Qt::Horizontal); // Включаем удаление/приближение только по горизонтальной
                       // оси

  //  customPlot->xAxis->setTickLabelType(
  //      QCPAxis::ltDateTime); // Подпись координат по Оси X в качестве Даты и
  //                            // Времени
  //  customPlot->xAxis->setDateTimeFormat(
  //      "hh:mm"); // Устанавливаем формат даты и времени

  // Настраиваем шрифт по осям координат
  customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

  // Автоматическое масштабирование тиков по Оси X
  //  customPlot->xAxis->setAutoTickStep(true);

  /* Делаем видимыми оси X и Y по верхней и правой границам графика,
   * но отключаем на них тики и подписи координат
   * */
  customPlot->xAxis2->setVisible(true);
  customPlot->yAxis2->setVisible(true);
  customPlot->xAxis2->setTicks(false);
  customPlot->yAxis2->setTicks(false);
  customPlot->xAxis2->setTickLabels(false);
  customPlot->yAxis2->setTickLabels(false);

  customPlot->yAxis->setTickLabelColor(
      QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y
  customPlot->legend->setVisible(true); // Включаем Легенду графика
  // Устанавливаем Легенду в левый верхний угол графика
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft |
                                                                  Qt::AlignTop);

  QVector<double> timeV = {};
  QVector<double> tempV = {};
  std::string user_string = "GET /";

  switch (ui->comboBox->currentIndex()) {
  case 0:
    user_string += "secClear";
    break;
  case 1:
    user_string += "hourClear";
    break;
  case 2:
    user_string += "dayClear";
    break;
  }
  std::string input = get_ans(cl, user_string);

  size_t index1 = 0;
  size_t index2 = 0;
  while (true) {
    index1 = input.find("<td>", index2);
    index2 = input.find("</td>", index1);
    if (index1 == std::string::npos) {
      break;
    }
    tempV.push_back(std::stod(input.substr(index1 + 4, index2 - index1 - 4)));

    index1 = input.find("<td>", index2);
    index2 = input.find("</td>", index1);

    timeV.push_back(std::stod(input.substr(index1 + 4, index2 - index1 - 4)));
  }
  if (timeV[timeV.size() - 1] - timeV[0] > 86400) {
    dateTicker->setDateTimeFormat("dd MMM yy");
  } else {
    dateTicker->setDateTimeFormat("hh:mm");
  }

  customPlot->xAxis->setTicker(dateTicker);
  //  graphic->setData(timeV, tempV); // Устанавливаем данные

  customPlot->addGraph();
  customPlot->graph(0)->addData(timeV, tempV);
  customPlot->rescaleAxes(); // Масштабируем график по данным
  customPlot->replot(); // Отрисовываем график
  cl->Disconnect();
  ui->connection_status->setText("Неподключено");
  QPalette palette = ui->connection_status->palette();
  palette.setColor(QPalette::WindowText, Qt::red);
  ui->connection_status->setPalette(palette);
}
