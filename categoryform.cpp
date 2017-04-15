#include "categoryform.h"
#include "ui_categoryform.h"
#include <QSqlRecord>
#include <QDebug>
CategoryForm::CategoryForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CategoryForm)
{
    ui->setupUi(this);
    QObject::connect(ui->addIncomeCategoryButton,SIGNAL(clicked(bool)),this,SLOT(checkIncomeLineEditSlot()));
    QObject::connect(ui->addExpenseCategoryButton,SIGNAL(clicked(bool)),this,SLOT(checkExpenseLineEditSlot()));

    QObject::connect(ui->deleteIncomeCategoryButton,SIGNAL(clicked(bool)),this,SLOT(deleteIncomeButtonSlot()));
    QObject::connect(ui->deleteExpenseCategoryButton,SIGNAL(clicked(bool)),this,SLOT(deleteExpenseButtonSlot()));
}

CategoryForm::~CategoryForm()
{
    delete ui;
}

//setters
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

//buttons listeners
void CategoryForm::checkIncomeLineEditSlot()
{
    QString s = ui->incomeCategoryLineEdit->text();
    if(s!=""){
        ui->incomeCategoryLineEdit->clear();
        emit addIncomeCategorySignal(s);
    }
}
void CategoryForm::checkExpenseLineEditSlot()
{
    QString s = ui->expenseCategoryLineEdit->text();
    if(s!=""){
        ui->expenseCategoryLineEdit->clear();
        emit addExpenseCategorySignal(s);
    }
}
void CategoryForm::deleteIncomeButtonSlot()
{
    QModelIndexList indexes =  ui->incomesListView->selectionModel()->selectedIndexes();
    emit deleteIncomeCategorySignal(indexes);
}
void CategoryForm::deleteExpenseButtonSlot()
{
    QModelIndexList indexes =  ui->expensesListView->selectionModel()->selectedIndexes();
    emit deleteExpenseCategorySignal(indexes);
}
