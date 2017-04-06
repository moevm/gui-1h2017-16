#include "adddataform.h"
#include "ui_adddataform.h"

AddDataForm::AddDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddDataForm)
{
    ui->setupUi(this);
}

AddDataForm::~AddDataForm()
{
    delete ui;
}

void AddDataForm::on_pushButton_2_clicked()
{

}
