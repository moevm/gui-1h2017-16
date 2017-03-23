#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QSqlDatabase>
#include <QDebug>
#include <QCoreApplication>
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
    QVector<Finance> getFinanceDataByDay(QDate date);
    QVector<Finance> getFinanceDataByMonth(QDate date);
    QVector<Finance> getFinanceDataByYear(QDate date);
    QVector<Finance> getAllFinanceData();
private:
    void initDB();
    QSqlDatabase sdb;
};

#endif // DBSERVICE_H
