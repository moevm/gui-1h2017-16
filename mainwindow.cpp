#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


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
}
