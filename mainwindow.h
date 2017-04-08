#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include "form.h"
#include "dbservice.h"
#include "categorysettings.h"


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
    CategorySettings *settingsWindow;
    QSqlTableModel *main_model;
    QSqlQueryModel *form_model;

    QDate currentDate;
    QStringList currentCategories;

    enum interval {DAY, MONTH, YEAR};
    enum type {NONE, INCOMES, EXPENSES, BOTH};

    interval filterInterval;
    type filterType;

    void initParameters();
    void initModel();
    void editTableView();
    void updateModelFilter();


private slots:
    void openForm();
    void toggleContent();
    void setDayInterval();
    void setMonthInterval();
    void setYearInterval();
    void incrementCurrentDate();
    void decrementCurrentDate();
    void checkShowingTypes();

    void changeCurrentDate(QDate);
    void formClosed();
    void saveNewData(Finance);
};

#endif // MAINWINDOW_H
