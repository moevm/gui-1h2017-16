#include "dbservice.h"


DBService::DBService()
{
    initDB();
}

void DBService::saveData(Finance *fin)
{
    QString queryString = QString("INSERT INTO f_data(f_type, f_date, f_category, f_sum) VALUES('%1', %2, '%3', %4 )")
            .arg(fin->getType())
            .arg("date('now')")
//            .arg(fin->getDate().toString("dd-MM-yyyy"))
            .arg(fin->getCategory())
            .arg(fin->getSum());

//    qDebug()<<queryString;

    QSqlQuery query;

    if(!query.exec(queryString))
        qDebug() << "ERROR: " << query.lastError().text();
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

QVector<Finance> DBService::getFinanceDataByDay(QDate date)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM f_data WHERE f_date = ?");
    query.addBindValue(date.toString("yyyy-MM-dd"));

    if(!query.exec())
            qDebug() << "ERROR: " << query.lastError().text();


    return queryToFinanceVector(&query);
}

QVector<Finance> DBService::getFinanceDataByMonth(QDate date)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM f_data WHERE f_date BETWEEN ? AND ?");
    qDebug()<< date.toString("yyyy-MM")  + "-01";
    qDebug()<< date.toString("yyyy-MM")  + "-31";
    query.addBindValue(date.toString("yyyy-MM")  + "-01");
    query.addBindValue(date.toString("yyyy-MM") + "-31");

    if(!query.exec())
            qDebug() << "ERROR: " << query.lastError().text();

    return queryToFinanceVector(&query);
}

QVector<Finance> DBService::getFinanceDataByYear(QDate date)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM f_data WHERE f_date BETWEEN ? AND ?");
    qDebug()<< date.toString("yyyy")  + "-01-01";
    qDebug()<< date.toString("yyyy")  + "-12-31";
    query.addBindValue(date.toString("yyyy")  + "-01-01");
    query.addBindValue(date.toString("yyyy") + "-12-31");

    if(!query.exec())
            qDebug() << "ERROR: " << query.lastError().text();

    return queryToFinanceVector(&query);
}

QVector<Finance> DBService::getAllFinanceData()
{
    QSqlQuery query;
    if(!query.exec("SELECT * FROM f_data"))
        qDebug() << "ERROR: " << query.lastError().text();

    return queryToFinanceVector(&query);
}

void DBService::initDB()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("D:\IEdb.db3");
    sdb.setDatabaseName("C:\IEdb.db3");
    if(!sdb.open()){
        qDebug()<<"doesn't work";
    }else{
        qDebug()<< "db has opened";
    }

    QSqlQuery query;

    if(!query.exec("CREATE TABLE IF NOT EXISTS `f_data` ("
                   "`f_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                   "`f_type`	TEXT NOT NULL, `f_date`	DATE NOT NULL,"
                   "`f_category`	TEXT NOT NULL, `f_sum`	INTEGER NOT NULL);"))
        qDebug() << "ERROR: " << query.lastError().text();

   if(!query.exec("CREATE TABLE IF NOT EXISTS `categories` ("
                    "`cat_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                    "`cat_name`	TEXT NOT NULL);"))
       qDebug() << "ERROR: " << query.lastError().text();

    //    qDebug()<<QString( QCoreApplication::applicationDirPath() + "/logo.jpg" );
}
