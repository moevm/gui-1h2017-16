#include "chartservice.h"

ChartService::ChartService()
{

}

QChart *ChartService::updateChart(QSqlQuery query, UtilEnums::ChartType chartType, UtilEnums::Interval interval, QDate currentDate)
{
    switch(chartType){
    case UtilEnums::BALANCE:
        return getBalanceBarChart(query, interval, currentDate);
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

QChart *ChartService::getBalanceBarChart(QSqlQuery query, UtilEnums::Interval interval, QDate date)
{
//    while(query.next()){
//        qDebug()<< "type " << query.value(0).toString() << " date " << query.value(1).toDate().toString()<< " value " << query.value(2).toInt();
//    }
//    query.first();

    QBarSet *income = new QBarSet("Доходы");
    QBarSet *expense = new QBarSet("Расходы");
    QStringList categories;

    switch(interval){
    case UtilEnums::DAY:
        break;
    case UtilEnums::MONTH:{
        int max_day = date.daysInMonth();
        for(int i = 1; i <= max_day; i++){
            categories << QString("%1").arg(i);
        }
        countBarSetsByMonth(query,income,expense,max_day);
        break;
    }
    case UtilEnums::YEAR:
        countBarSetsByYear(query,income,expense);
        categories << "Янв" << "Фев" << "Мар" << "Апр"
                   << "Май" << "Июн" << "Июл" << "Авг"
                   << "Сен" << "Окт" << "Ноя" << "Дек";

        break;
    }
for(int i=0;i<categories.size();i++){
    qDebug()<<categories.at(i);
}
    return setUpBarChart(income,expense,categories);
}

void ChartService::countBarSetsByYear(QSqlQuery query, QBarSet *income, QBarSet *expense){
    int current_month = 1;
    int current_income_sum = 0;
    int current_expense_sum = 0;

    while(query.next()){
        if(query.value(1).toDate().month() == current_month){
            if(query.value(0).toString()=="доходы") current_income_sum+=query.value(2).toInt();
            else current_expense_sum+=query.value(2).toInt();
        }else{
            income->append(current_income_sum);
            current_income_sum = 0;
            expense->append(current_expense_sum);
            current_expense_sum = 0;

            current_month++;

            while(query.value(1).toDate().month() > current_month){
                income->append(0);
                expense->append(0);
                current_month++;
            }
            if(query.value(0).toString()=="доходы") current_income_sum+=query.value(2).toInt();
            else current_expense_sum+=query.value(2).toInt();
        }
    }

    income->append(current_income_sum);
    expense->append(current_expense_sum);
    current_month++;

    while(current_month < 12){
        income->append(0);
        expense->append(0);
        current_month++;
    }
}

void ChartService::countBarSetsByMonth(QSqlQuery query, QBarSet *income, QBarSet *expense, int max_day){
    int current_day = 1;

    int current_income_sum = 0;
    int current_expense_sum = 0;

    while(query.next()){
        if(query.value(1).toDate().day() == current_day){
            if(query.value(0).toString()=="доходы") current_income_sum+=query.value(2).toInt();
            else current_expense_sum+=query.value(2).toInt();
        }else{
            income->append(current_income_sum);
            current_income_sum = 0;
            expense->append(current_expense_sum);
            current_expense_sum = 0;

            current_day++;

            while(query.value(1).toDate().day() > current_day){
                income->append(0);
                expense->append(0);
                current_day++;
            }
            if(query.value(0).toString()=="доходы") current_income_sum+=query.value(2).toInt();
            else current_expense_sum+=query.value(2).toInt();
        }
    }

    income->append(current_income_sum);
    expense->append(current_expense_sum);
    current_day++;

    while(current_day < max_day){
        income->append(0);
        expense->append(0);
        current_day++;
    }
}

QChart *ChartService::setUpBarChart(QBarSet *income, QBarSet *expense, QStringList categories)
{
    QBarSeries *series = new QBarSeries();
    series->append(income);
    series->append(expense);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);

    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);
    return chart;
}

