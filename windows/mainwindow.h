#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include "form.h"
#include "../services/dbservice.h"
#include "../services/chartservice.h"
#include "categoryform.h"
#include "../util/utilenums.h"
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE

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

    DBService *dbservice;
    ChartService *chartService;

    QDate currentDate;
    QStringList currentCategories;

    UtilEnums::Interval filterInterval;
    UtilEnums::Type filterType;
    UtilEnums::ChartType currentChart;

    QSet<QString> uncheckedIncomeCategories;
    QSet<QString> uncheckedExpenseCategories;

    void initParameters();
    void initModels();
    void initMainModel(QSqlDatabase sdb);
    void editTableView();
    void initCategoryCheckBoxesView();

    void updateModelFilter();
    void updateCategoryListWidgets();
    void fillCategoryListWidget(QListWidget* widget,QSet<QString> set);
    void updateTableView(QListWidgetItem* item,QString type);
    void makeConnects();
    void closeEvent(QCloseEvent*);
private slots:
    //interaction with other forms
    void openForm();
    void openCategoryForm();
    void formClosed();
    void setIncomesComboBoxModel();
    void setExpensesComboBoxModel();

    //main window UI
    void toggleContent();
    void setDayInterval();
    void setMonthInterval();
    void setYearInterval();
    void checkChartType();
    void incrementCurrentDate();
    void decrementCurrentDate();
    void changeCurrentDate(QDate);
    void checkShowingTypes();
    void updateFromIncomeCategoryListWidget(QListWidgetItem*);
    void updateFromExpenseCategoryListWidget(QListWidgetItem*);

    //crud
    void saveNewData(Finance);
    void deleteData();
    void addIncomeCategory(QString);
    void addExpenseCategory(QString);
    void deleteIncomeCategory(QModelIndexList);
    void deleteExpenseCategory(QModelIndexList);

    void updateChart();

};

#endif // MAINWINDOW_H
