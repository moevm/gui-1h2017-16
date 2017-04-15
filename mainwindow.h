#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include "form.h"
#include "dbservice.h"
#include "categoryform.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Form *form;

    CategoryForm *categoryForm;

    QSqlTableModel *main_model;
    QSqlQueryModel *form_model;

    QSqlTableModel *income_category_model;
    QSqlTableModel *expense_category_model;

    QDate currentDate;
    QStringList currentCategories;

    enum interval {DAY, MONTH, YEAR};
    enum type {NONE, INCOMES, EXPENSES, BOTH};

    interval filterInterval;
    type filterType;

    void initParameters();
    void initModels();
    void initMainModel(QSqlDatabase sdb);
    void initFormModel(QSqlDatabase sdb);
    void editTableView();
    void updateModelFilter();
    void makeConnects();


private slots:
    //interaction with other forms
    void openForm();
    void openCategoryForm();
    void formClosed();

    //main window UI
    void toggleContent();
    void setDayInterval();
    void setMonthInterval();
    void setYearInterval();
    void incrementCurrentDate();
    void decrementCurrentDate();
    void changeCurrentDate(QDate);
    void checkShowingTypes();

    //crud
    void saveNewData(Finance);
    void deleteData();
    void addIncomeCategory(QString);
    void addExpenseCategory(QString);
    void deleteIncomeCategory(QModelIndexList);
    void deleteExpenseCategory(QModelIndexList);



};

#endif // MAINWINDOW_H
