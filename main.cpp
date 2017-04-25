#include "mainwindow.h"
#include "dbservice.h"
#include <QApplication>
#include <QTextCodec>
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.show();

    return a.exec();
}
