#include "dbservice.h"


DBService::DBService()
{
    initDB();
}

void DBService::saveData(Finance *fin)
{

}

QVector<Finance> DBService::queryToFinanceVector(QSqlQuery *query)
{
    QSqlRecord rec = query->record();
    QVector<Finance> vector;
    while(query->next()){
        Finance fin = Finance();
        fin.setType(query->value(rec.indexOf("f_type")).toString());
        fin.setDate(QDate::fromString(query->value(rec.indexOf("f_date")).toString(),"yyyy-MM-dd"));
        fin.setCategory(query->value(rec.indexOf("f_category")).toString());
        fin.setSum(query->value(rec.indexOf("f_sum")).toInt());
        vector.append(fin);
    }
    return vector;
}


void DBService::initDB()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("D:\IEdb.db3");
    sdb.setDatabaseName("D:\IEdb.db3");
    if(!sdb.open()){
        qDebug()<<"doesn't work";
    }else{
        qDebug()<< "db has opened";
    }

    QSqlTableModel *model = new QSqlTableModel(0,sdb);
    model->setTable("f_data");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

//    QSqlQuery query;

//    if(!query.exec("CREATE TABLE IF NOT EXISTS `f_data` ("
//                   "`f_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
//                   "`f_type`	TEXT NOT NULL, `f_date`	DATE NOT NULL,"
//                   "`f_category`	TEXT NOT NULL, `f_sum`	INTEGER NOT NULL);"))
//        qDebug() << "ERROR: " << query.lastError().text();

//   if(!query.exec("CREATE TABLE IF NOT EXISTS `categories` ("
//                    "`cat_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
//                    "`cat_name`	TEXT NOT NULL);"))
//       qDebug() << "ERROR: " << query.lastError().text();

    //    qDebug()<<QString( QCoreApplication::applicationDirPath() + "/logo.jpg" );
}
