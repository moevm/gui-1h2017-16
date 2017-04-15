#include "categoryform.h"
#include "ui_categoryform.h"
#include <QSqlRecord>
#include <QDebug>
CategoryForm::CategoryForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CategoryForm)
{
    ui->setupUi(this);
}

CategoryForm::~CategoryForm()
{
    delete ui;
}
void CategoryForm::setIncomeCategoriesModel(QSqlTableModel *model)
{

    ui->incomesListView->setModel(model);
    ui->incomesListView->setModelColumn(1);
}

void CategoryForm::setExpenseCategoriesModel(QSqlTableModel *model)
{

    ui->expensesListView->setModel(model);
    ui->expensesListView->setModelColumn(1);
}
