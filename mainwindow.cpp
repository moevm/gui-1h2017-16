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

MainWindow::openAddDataForm()
{
//    AddDataForm *form = new AddDataForm;
//    form->show();
//    addForm->show();
    Finance fin = Finance("hellosd", QDate::currentDate(),"cassdat",12333);
    dbservice->saveData(&fin);

    QVector<Finance> vector = dbservice->getFinanceDataByDay(QDate::currentDate());
    for(int i =0; i<vector.length();i++){
        qDebug()<<vector.at(i).toString();
    }
}
