#ifndef DBSERVICE_H
#define DBSERVICE_H
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QCoreApplication>
#include "../model/finance.h"
#include "../util/utilenums.h"

class DBService
{
public:
    DBService();
    void init();
    QSqlDatabase getDB();

    QSqlTableModel *getIncomeCategoryModel() const;
    QSqlTableModel *getExpenseCategoryModel() const;
    QSqlTableModel *getMainModel() const;

    void addFinanceData(Finance finance);
    void addIncomeCategory(QString s);
    void addExpenseCategory(QString s);
    QSet<QString> deleteIncomeModelData(QModelIndexList indexes);
    QSet<QString> deleteExpenseModelData(QModelIndexList indexes);

    void updateMainModelFilter(UtilEnums::Interval interval, UtilEnums::Type type,QDate currentDate);
    void deleteMainModelData(QModelIndexList indexes);

    void definePossibleCategories();

    QSqlQuery getChartQuery(UtilEnums::Interval interval, UtilEnums::ChartType chartType, QDate currentDate);

    QSet<QString> getPossibleIncomeCategories() const;
    QSet<QString> getPossibleExpenseCategories() const;

private:
    QSqlDatabase sdb;
    QSqlTableModel *main_model;
    QSqlTableModel *income_category_model;
    QSqlTableModel *expense_category_model;

    QSet<QString> possibleIncomeCategories;
    QSet<QString> possibleExpenseCategories;

    void initDB();
    void createTables();
    void initModels();
};

#endif // DBSERVICE_H
