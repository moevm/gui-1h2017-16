#include "finance.h"


Finance::Finance()
{

}

Finance::Finance(QString type, QDate date, QString category, int sum)
{
    this->type = type;
    this->date = date;
    this->category = category;
    this->sum=sum;
}

int Finance::getId() const
{
    return id;
}

void Finance::setId(int value)
{
    id = value;
}

QString Finance::getType() const
{
    return type;
}

void Finance::setType(const QString &value)
{
    type = value;
}

QDate Finance::getDate() const
{
    return date;
}

void Finance::setDate(const QDate &value)
{
    date = value;
}

QString Finance::getCategory() const
{
    return category;
}

void Finance::setCategory(const QString &value)
{
    category = value;
}

int Finance::getSum() const
{
    return sum;
}

void Finance::setSum(int value)
{
    sum = value;
}

QString Finance::toString() const
{
    return QString("Finance [type = %1, date = %2, category = %3, sum = %4]")
            .arg(type).arg(date.toString("yyyy-MM-dd")).arg(category).arg(sum);
}
