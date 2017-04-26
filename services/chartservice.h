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
    QChart* updateChart(QSqlQuery query, UtilEnums::ChartType chartType);
private:
    QChart* getIncomePieChart(QSqlQuery query);
    QChart* getExpensePieChart(QSqlQuery query);
    QChart* getBalanceBarChart(QSqlQuery query);
};

#endif // CHARTSERVICE_H
