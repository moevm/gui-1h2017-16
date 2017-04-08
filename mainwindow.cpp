#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addForm = new AddDataForm;

    initParameters();
    initModel();
    editTableView();


    QObject::connect(ui->addDataButton,SIGNAL(clicked(bool)),this,SLOT(openAddDataForm()));
    QObject::connect(ui->toggleContentStackButton,SIGNAL(clicked(bool)),this,SLOT(toggleContent()));

    QObject::connect(ui->dateEditField,SIGNAL(dateChanged(QDate)),this,SLOT(changeCurrentDate(QDate)));
    QObject::connect(ui->dayRadioButton,SIGNAL(clicked(bool)),this,SLOT(setDayInterval()));
    QObject::connect(ui->monthRadioButton,SIGNAL(clicked(bool)),this,SLOT(setMonthInterval()));
    QObject::connect(ui->yearRadioButton,SIGNAL(clicked(bool)),this,SLOT(setYearInterval()));

    QObject::connect(ui->nextDateButton,SIGNAL(clicked(bool)),this,SLOT(incrementCurrentDate()));
    QObject::connect(ui->previousDateButton,SIGNAL(clicked(bool)),this,SLOT(decrementCurrentDate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initParameters()
{
    currentDate = QDate::currentDate();
    ui->dateEditField->setDate(currentDate);
    filterInterval = DAY;
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

    updateModelFilter();
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

void MainWindow::updateModelFilter()
{
    QString filterString;
    switch(filterInterval){
    case DAY:
        filterString = QString("f_date = '%1'").arg(currentDate.toString("yyyy-MM-dd"));
        break;
    case MONTH:
        filterString = QString("f_date BETWEEN '%1-01' AND '%1-31'").arg(currentDate.toString("yyyy-MM"));
        break;
    case YEAR:
        filterString = QString("f_date BETWEEN '%1-01-01' AND '%1-12-31'").arg(currentDate.toString("yyyy"));
        break;
    default:
        qDebug() << "error in choose interval section";
    }

    qDebug() << filterString;

    model->setFilter(filterString);

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

void MainWindow::setDayInterval()
{
    filterInterval = DAY;
    updateModelFilter();
}

void MainWindow::setMonthInterval()
{
    filterInterval = MONTH;
    updateModelFilter();
}

void MainWindow::setYearInterval()
{
    filterInterval = YEAR;
    updateModelFilter();
}

void MainWindow::incrementCurrentDate()
{
    switch(filterInterval){
    case DAY:
        currentDate = currentDate.addDays(1);
        break;
    case MONTH:
        currentDate = currentDate.addMonths(1);
        break;
    case YEAR:
        currentDate = currentDate.addYears(1);
        break;
    default:
        qDebug() << "error in setting next date section";
    }

    ui->dateEditField->setDate(currentDate);
    updateModelFilter();
}

void MainWindow::decrementCurrentDate()
{
    switch(filterInterval){
    case DAY:
        currentDate = currentDate.addDays(-1);
        break;
    case MONTH:
        currentDate = currentDate.addMonths(-1);
        break;
    case YEAR:
        currentDate = currentDate.addYears(-1);
        break;
    default:
        qDebug() << "error in setting prev date section";
    }

    ui->dateEditField->setDate(currentDate);
    updateModelFilter();
}

void MainWindow::changeCurrentDate(QDate date)
{
    currentDate = date;
    updateModelFilter();
}
