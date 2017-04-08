#include "form.h"
#include "ui_form.h"


Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

     QObject::connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(checkInputData()));
}

Form::~Form()
{
    delete ui;
}


void Form::checkInputData()
{
    if(ui->sumEditField->text()==""){
        qDebug()<<"error";
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

void Form::setModel(QSqlQueryModel *value)
{
    ui->categoryComboBox->setModel(value);
}

void Form::setDate(const QDate &value)
{
    ui->dateEditField->setDate(value);
}



void Form::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}
