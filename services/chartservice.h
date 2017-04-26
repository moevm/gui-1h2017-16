#ifndef CHARTSERVICE_H
#define CHARTSERVICE_H
#include "../util/utilenums.h"
#include <QSqlQuery>
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE

class ChartService
{
public:
    ChartService();
    QChart* updateChart(QSqlQuery query, UtilEnums::ChartType chartType, UtilEnums::Interval interval, QDate currentDate);
private:
    QChart* getIncomePieChart(QSqlQuery query);
    QChart* getExpensePieChart(QSqlQuery query);
    QChart* getBalanceBarChart(QSqlQuery query, UtilEnums::Interval interval, QDate date);

    void countBarSetsByYear(QSqlQuery query, QBarSet *income, QBarSet *expense);
    void countBarSetsByMonth(QSqlQuery query, QBarSet *income, QBarSet *expense, int max_day);
    QChart* setUpBarChart(QBarSet *income, QBarSet *expense, QStringList categories);
};

#endif // CHARTSERVICE_H
