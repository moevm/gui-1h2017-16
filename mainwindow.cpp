#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addForm = new AddDataForm;
    dbservice = new DBService;
    QObject::connect(ui->addDataButton,SIGNAL(clicked(bool)),this,SLOT(openAddDataForm()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAddDataForm()
{
    AddDataForm *form = new AddDataForm;
    form->show();
//    addForm->show();
    Finance fin = Finance("hellosd", QDate::currentDate(),"cassdat",12333);
    dbservice->saveData(&fin);

    QVector<Finance> vector = dbservice->getFinanceDataByMonth(QDate::currentDate());
    ui->tableWidget->clearContents();
    for(int i =0; i<vector.length();i++){
        qDebug()<<vector.at(i).toString();
        if (ui->tableWidget->rowCount() < vector.length()) ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(vector.at(i).getCategory()));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(vector.at(i).getDate().toString()));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(vector.at(i).getType()));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(QString::number(vector.at(i).getSum())));
    }


}
