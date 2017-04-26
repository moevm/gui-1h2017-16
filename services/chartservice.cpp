#include "chartservice.h"

ChartService::ChartService()
{

}

QChart *ChartService::updateChart(QSqlQuery query, UtilEnums::ChartType chartType)
{
    switch(chartType){
    case UtilEnums::BALANCE:
        return getBalanceBarChart(query);
        break;
    case UtilEnums::INCOME:
        return getIncomePieChart(query);
        break;
    case UtilEnums::EXPENSE:
        return getExpensePieChart(query);
        break;
    }
}

QChart *ChartService::getIncomePieChart(QSqlQuery query)
{
    QPieSeries *series = new QPieSeries();

    while (query.next()) {
        if(query.value(0).toString()=="доходы"){
            series->append(query.value(1).toString(),(double)query.value(2).toInt());
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    return chart;
}

QChart *ChartService::getExpensePieChart(QSqlQuery query)
{
    QPieSeries *series = new QPieSeries();

    while (query.next()) {
        if(query.value(0).toString()=="расходы"){
            series->append(query.value(1).toString(),(double)query.value(2).toInt());
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    return chart;
}

QChart *ChartService::getBalanceBarChart(QSqlQuery query)
{

}

