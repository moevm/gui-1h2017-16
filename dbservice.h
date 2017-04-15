#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QSqlDatabase>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "finance.h"


class DBService
{
public:
    DBService();

    void saveData(Finance* fin);
    void changeData();
    void deleteData();

    QVector<Finance> queryToFinanceVector(QSqlQuery *query);

private:
    void initDB();

};

#endif // DBSERVICE_H
