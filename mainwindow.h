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

    void initModel();
    void editTableView();


private slots:
    void openAddDataForm();
    void toggleContent();
};

#endif // MAINWINDOW_H
