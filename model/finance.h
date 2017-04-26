#ifndef FINANCE_H
#define FINANCE_H
#include <QString>
#include <QDate>

class Finance
{
public:
    Finance();
    Finance(QString type, QDate date, QString category, int sum);

    int getId() const;
    void setId(int value);

    QString getType() const;
    void setType(const QString &value);

    QDate getDate() const;
    void setDate(const QDate &value);

    QString getCategory() const;
    void setCategory(const QString &value);

    int getSum() const;
    void setSum(int value);

    QString toString() const;

private:
    int id;
    QString type;
    QDate date;
    QString category;
    int sum;
};

#endif // FINANCE_H
