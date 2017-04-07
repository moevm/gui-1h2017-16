#include "mainwindow.h"
#include "dbservice.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    DBService dbservice;

    MainWindow w;
    w.show();

//    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("D:\IEdb.db3");
////    sdb.setDatabaseName("C:\IEdb.db3");
//    if(!sdb.open()){
//        qDebug()<<"doesn't work";
//    }else{
//        qDebug()<< "db has opened";
//    }


//    QSqlTableModel *model = new QSqlTableModel(0,sdb);
//    model->setTable("f_data");
//    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

//    while (model->canFetchMore())
//        {
//            model->fetchMore();
//        }
//    model->setHeaderData(0, Qt::Horizontal, "id");
//    model->setHeaderData(1, Qt::Horizontal, "type");
//    model->setHeaderData(2, Qt::Horizontal, "date");
//    model->setHeaderData(3, Qt::Horizontal, "cat");
//    model->setHeaderData(4, Qt::Horizontal, "sum");
//    QString filter = QString("f_date BETWEEN %1 AND %2").arg("'2016-12-12'").arg("'2018-12-12'");
//    qDebug() << filter;
//    model->setFilter(filter);
//    model->select();

//    QTableView *view = new QTableView;
//    view->setModel(model);
////    view->hideColumn(0); // don't show the ID
//    view->show();

    return a.exec();
}
