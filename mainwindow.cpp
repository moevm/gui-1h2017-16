#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addForm = new AddDataForm;


    initModel();
    editTableView();


    QObject::connect(ui->addDataButton,SIGNAL(clicked(bool)),this,SLOT(openAddDataForm()));
    QObject::connect(ui->toggleContentStackButton,SIGNAL(clicked(bool)),this,SLOT(toggleContent()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initModel()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("C:\IEdb.db3");
    sdb.setDatabaseName("D:\IEdb.db3");
    if(!sdb.open()){
        qDebug()<<"doesn't work";
    }else{
        qDebug()<< "db has opened";
    }

    model = new QSqlTableModel(0,sdb);
    model->setTable("f_data");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setHeaderData(0, Qt::Horizontal, "id");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Category");
    model->setHeaderData(4, Qt::Horizontal, "Sum");

    model->select();

//    QSqlQuery query;

//    if(!query.exec("CREATE TABLE IF NOT EXISTS `f_data` ("
//                   "`f_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
//                   "`f_type`	TEXT NOT NULL, `f_date`	DATE NOT NULL,"
//                   "`f_category`	TEXT NOT NULL, `f_sum`	INTEGER NOT NULL);"))
//        qDebug() << "ERROR: " << query.lastError().text();

//   if(!query.exec("CREATE TABLE IF NOT EXISTS `categories` ("
//                    "`cat_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
//                    "`cat_name`	TEXT NOT NULL);"))
//       qDebug() << "ERROR: " << query.lastError().text();

    //    qDebug()<<QString( QCoreApplication::applicationDirPath() + "/logo.jpg" );
}

void MainWindow::editTableView()
{
     ui->tableView->setModel(model);
     ui->tableView->verticalHeader()->hide();
     ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     ui->tableView->hideColumn(0); // don't show the ID
}

void MainWindow::openAddDataForm()
{
    AddDataForm *form = new AddDataForm;
    form->show();
//    addForm->show();
//    Finance fin = Finance("hellosd", QDate::currentDate(),"cassdat",12333);
//    dbservice->saveData(&fin);

//    QVector<Finance> vector = dbservice->getFinanceDataByMonth(QDate::currentDate());
//    ui->tableWidget->clearContents();
//    for(int i =0; i<vector.size();i++){
//        qDebug()<<vector.at(i).toString();
//        if (ui->tableWidget->rowCount() < vector.size()) ui->tableWidget->insertRow(i);
//        ui->tableWidget->setItem(i,0, new QTableWidgetItem(vector.at(i).getCategory()));
//        ui->tableWidget->setItem(i,1, new QTableWidgetItem(vector.at(i).getDate().toString()));
//        ui->tableWidget->setItem(i,2, new QTableWidgetItem(vector.at(i).getType()));
//        ui->tableWidget->setItem(i,3, new QTableWidgetItem(QString::number(vector.at(i).getSum())));
//    }


}

void MainWindow::toggleContent(){
    int curContentIndex = ui -> contentStack -> currentIndex();
    int curPanelIndex = ui->controlPanelStack -> currentIndex();

    ui->contentStack->setCurrentIndex((curContentIndex + 1)%2);
    ui->controlPanelStack->setCurrentIndex((curPanelIndex + 1)%2);
}
