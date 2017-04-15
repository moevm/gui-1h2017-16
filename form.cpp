#include "form.h"
#include "ui_form.h"


Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->errorLabel->setVisible(false);

     QObject::connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(checkInputData()));
     QObject::connect(ui->categoryToolButton,SIGNAL(clicked(bool)),this,SLOT(disableCategoryToolButton()));
}

Form::~Form()
{
    delete ui;
}


void Form::checkInputData()
{
    if(ui->sumEditField->text()=="" || ui->sumEditField->text().toInt()<=0){
        qDebug()<<"error";
        ui->errorLabel->setVisible(true);
        return;
    }

    Finance finance;
    finance.setDate(ui->dateEditField->date());
    finance.setType((ui->incomesRadioButton->isChecked()) ? "income" : "expense");
    finance.setCategory(ui->categoryComboBox->currentText());
    finance.setSum(ui->sumEditField->text().toInt());

    emit dataChecked(finance);
    close();
}

void Form::disableCategoryToolButton()
{
    emit categoryToolButtonPressed();
}

void Form::setModel(QSqlTableModel *value)
{
    ui->categoryComboBox->setModel(value);
    ui->categoryComboBox->setModelColumn(1);
}

void Form::setDate(const QDate &value)
{
    ui->dateEditField->setDate(value);
}

void Form::closeEvent(QCloseEvent *event)
{
    clearFields();
    emit closed();
    event->accept();
}

void Form::clearFields(){
    ui->errorLabel->setVisible(false);
    ui->sumEditField->setText("");
}
