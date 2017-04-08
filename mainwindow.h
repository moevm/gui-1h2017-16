#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include "adddataform.h"
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
    AddDataForm *addForm;
    QSqlTableModel *model;

    QDate currentDate;

    enum interval{DAY, MONTH, YEAR};

    interval filterInterval;
    QString filterTypeString;

    void initParameters();
    void initModel();
    void editTableView();
    void updateModelFilter();


private slots:
    void openAddDataForm();
    void toggleContent();
    void setDayInterval();
    void setMonthInterval();
    void setYearInterval();
    void incrementCurrentDate();
    void decrementCurrentDate();

    void changeCurrentDate(QDate);
};

#endif // MAINWINDOW_H
