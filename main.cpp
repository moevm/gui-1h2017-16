#include "windows/mainwindow.h"
#include "services/dbservice.h"
#include <QApplication>
#include <QTextCodec>
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
