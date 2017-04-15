#ifndef CATEGORYFORM_H
#define CATEGORYFORM_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class CategoryForm;
}

class CategoryForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit CategoryForm(QWidget *parent = 0);
    ~CategoryForm();
    void setIncomeCategoriesModel(QSqlTableModel *model);
    void setExpenseCategoriesModel(QSqlTableModel *model);

signals:
    void addIncomeCategorySignal(QString);
    void addExpenseCategorySignal(QString);
    void deleteIncomeCategorySignal();
    void deleteExpenseCategorySignal();

private:
    Ui::CategoryForm *ui;

private slots:
    void checkIncomeLineEditSlot();
    void checkExpenseLineEditSlot();
    void deleteIncomeButtonSlot();
    void deleteExpenseButtonSlot();
};

#endif // CATEGORYFORM_H
