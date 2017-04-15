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
private:
    Ui::CategoryForm *ui;
};

#endif // CATEGORYFORM_H
