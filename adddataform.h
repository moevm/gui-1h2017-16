#ifndef ADDDATAFORM_H
#define ADDDATAFORM_H

#include <QWidget>

namespace Ui {
class AddDataForm;
}

class AddDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddDataForm(QWidget *parent = 0);
    ~AddDataForm();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::AddDataForm *ui;
};

#endif // ADDDATAFORM_H
