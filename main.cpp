#include "mainwindow.h"
#include "dbservice.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
