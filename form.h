#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>
#include <QSqlTableModel>
#include "finance.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();


    void setDate(const QDate &value);
    void setModel(QSqlTableModel *value);


signals:
    void closed();
    void dataChecked(Finance);
    void categoryToolButtonPressed();
    void incomesRadioButtonSignal();
    void expensesRadioButtonSignal();

private slots:
    void checkInputData();
    void disableCategoryToolButton();

private:
    Ui::Form *ui;
    void closeEvent(QCloseEvent*);
    void clearFields();
};

#endif // FORM_H
