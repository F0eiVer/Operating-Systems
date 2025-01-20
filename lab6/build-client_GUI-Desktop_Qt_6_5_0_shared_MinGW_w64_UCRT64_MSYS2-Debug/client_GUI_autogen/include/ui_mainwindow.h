/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *server_ip;
    QLineEdit *port;
    QLabel *label_2;
    QLabel *label_3;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_4;
    QLabel *connection_status;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *comboBox;
    QPushButton *table_button;
    QPushButton *graphic_button;
    QPushButton *connect_button;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *contentLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 665);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(180, 10, 221, 31));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        server_ip = new QLineEdit(centralwidget);
        server_ip->setObjectName("server_ip");
        server_ip->setGeometry(QRect(40, 70, 171, 28));
        port = new QLineEdit(centralwidget);
        port->setObjectName("port");
        port->setGeometry(QRect(230, 70, 121, 28));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 50, 141, 20));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(230, 50, 41, 16));
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(0, 150, 1021, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(520, -2, 20, 161));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 120, 151, 20));
        connection_status = new QLabel(centralwidget);
        connection_status->setObjectName("connection_status");
        connection_status->setGeometry(QRect(230, 120, 141, 20));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        connection_status->setPalette(palette);
        QFont font1;
        font1.setBold(true);
        connection_status->setFont(font1);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(710, 10, 191, 31));
        label_6->setFont(font);
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(571, 44, 171, 31));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(570, 70, 161, 28));
        table_button = new QPushButton(centralwidget);
        table_button->setObjectName("table_button");
        table_button->setGeometry(QRect(760, 70, 91, 29));
        graphic_button = new QPushButton(centralwidget);
        graphic_button->setObjectName("graphic_button");
        graphic_button->setGeometry(QRect(880, 70, 101, 29));
        connect_button = new QPushButton(centralwidget);
        connect_button->setObjectName("connect_button");
        connect_button->setGeometry(QRect(380, 70, 111, 29));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 160, 1021, 451));
        contentLayout = new QVBoxLayout(verticalLayoutWidget);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1024, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\201\321\217 \320\272 \321\201\320\265\321\200\320\262\320\265\321\200\321\203", nullptr));
        server_ip->setInputMask(QString());
        server_ip->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "IP \321\201\320\265\321\200\320\262\320\265\321\200\320\260", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\200\321\202", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\321\203\321\201 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217:", nullptr));
        connection_status->setText(QCoreApplication::translate("MainWindow", "\320\235\320\265\320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\276", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\321\203\321\207\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\320\265\320\275\320\275\320\276\320\271 \320\270\320\275\321\202\320\265\321\200\320\262\320\260\320\273", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217 \320\267\320\260 24 \321\207\320\260\321\201\320\260", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217 \320\267\320\260 \320\274\320\265\321\201\321\217\321\206", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\320\222\321\200\320\265\320\274\321\217 \320\267\320\260 \320\263\320\276\320\264", nullptr));

        table_button->setText(QCoreApplication::translate("MainWindow", "\320\242\320\260\320\261\320\273\320\270\321\206\320\260", nullptr));
        graphic_button->setText(QCoreApplication::translate("MainWindow", "\320\223\321\200\320\260\321\204\320\270\320\272", nullptr));
        connect_button->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
