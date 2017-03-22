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

private:
    Ui::AddDataForm *ui;
};

#endif // ADDDATAFORM_H
