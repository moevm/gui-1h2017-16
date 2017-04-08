#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include "form.h"
#include "dbservice.h"


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
    QSqlTableModel *model;

    QDate currentDate;

    enum interval {DAY, MONTH, YEAR};
    enum type {NONE, INCOMES, EXPENSES, BOTH};

    interval filterInterval;
    type filterType;

//    QString filterIntervalString;
//    QString filterIntervalString;

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
