#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    form = new Form;

    initParameters();
    initModel();
    editTableView();


    QObject::connect(ui->addDataButton,SIGNAL(clicked(bool)),this,SLOT(openForm()));
    QObject::connect(ui->toggleContentStackButton,SIGNAL(clicked(bool)),this,SLOT(toggleContent()));

    QObject::connect(ui->dateEditField,SIGNAL(dateChanged(QDate)),this,SLOT(changeCurrentDate(QDate)));
    QObject::connect(ui->dayRadioButton,SIGNAL(clicked(bool)),this,SLOT(setDayInterval()));
    QObject::connect(ui->monthRadioButton,SIGNAL(clicked(bool)),this,SLOT(setMonthInterval()));
    QObject::connect(ui->yearRadioButton,SIGNAL(clicked(bool)),this,SLOT(setYearInterval()));

    QObject::connect(ui->nextDateButton,SIGNAL(clicked(bool)),this,SLOT(incrementCurrentDate()));
    QObject::connect(ui->previousDateButton,SIGNAL(clicked(bool)),this,SLOT(decrementCurrentDate()));

    QObject::connect(form,SIGNAL(closed()),this,SLOT(formClosed()));
    QObject::connect(form,SIGNAL(dataChecked(Finance)),this,SLOT(saveNewData(Finance)));

    QObject::connect(ui->incomesCheckBox,SIGNAL(clicked(bool)),this,SLOT(checkShowingTypes()));
    QObject::connect(ui->expensesCheckBox,SIGNAL(clicked(bool)),this,SLOT(checkShowingTypes()));

    QObject::connect(ui->deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteData()));
}

MainWindow::~MainWindow()
{
    delete form;
    delete ui;
}

void MainWindow::initParameters()
{
    currentDate = QDate::currentDate();
    ui->dateEditField->setDate(currentDate);
    filterInterval = DAY;
    filterType = BOTH;
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

    main_model = new QSqlTableModel(0,sdb);
    main_model->setTable("f_data");
    main_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    main_model->setHeaderData(0, Qt::Horizontal, "id");
    main_model->setHeaderData(1, Qt::Horizontal, "Type");
    main_model->setHeaderData(2, Qt::Horizontal, "Date");
    main_model->setHeaderData(3, Qt::Horizontal, "Category");
    main_model->setHeaderData(4, Qt::Horizontal, "Sum");


    updateModelFilter();
    main_model->select();

    form_model = new QSqlQueryModel(/*0,sdb*/);
//    form_model->setTable("f_categories");
//    form_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QSqlQuery query("select name from f_categories", sdb);
    form_model->setQuery(query);
    form->setModel(form_model);


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
     ui->tableView->setModel(main_model);
     ui->tableView->verticalHeader()->hide();
     ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     ui->tableView->hideColumn(0); // don't show the ID
     ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
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

    switch(filterType){
    case NONE:
        filterString.append("AND f_type = 'none'");
        break;
    case INCOMES:
        filterString.append("AND f_type = 'income'");
        break;
    case EXPENSES:
        filterString.append("AND f_type = 'expense'");
        break;
    case BOTH:
        break;
    }

    qDebug() << filterString;

    main_model->setFilter(filterString);

}

void MainWindow::openForm()
{
    ui->addDataButton->setEnabled(false);
    form->setDate(currentDate);
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

void MainWindow::checkShowingTypes()
{
    bool income = ui->incomesCheckBox->isChecked();
    bool expense = ui->expensesCheckBox->isChecked();
    if (income && expense) filterType = BOTH;
    if (income && !expense) filterType = INCOMES;
    if (!income && expense) filterType = EXPENSES;
    if (!income && !expense) filterType = NONE;

    updateModelFilter();
}

void MainWindow::deleteData()
{

    QModelIndexList indexes =  ui->tableView->selectionModel()->selectedRows();
    int countRow = indexes.count();

    for( int i = countRow; i > 0; i--)
           main_model->removeRow( indexes.at(i-1).row(), QModelIndex());
    main_model->submitAll();
    main_model->select();
}

void MainWindow::changeCurrentDate(QDate date)
{
    currentDate = date;
    updateModelFilter();
}

void MainWindow::formClosed()
{
    ui->addDataButton->setEnabled(true);
}

void MainWindow::saveNewData(Finance finance)
{
    qDebug()<< finance.toString();

    QString queryString = QString("INSERT INTO f_data(f_type, f_date, f_category, f_sum) VALUES('%1', '%2', '%3', %4 )")
            .arg(finance.getType())
            .arg(finance.getDate().toString("yyyy-MM-dd"))
            .arg(finance.getCategory())
            .arg(finance.getSum());

    qDebug()<<queryString;

    QSqlQuery query;

    if(!query.exec(queryString))
        qDebug() << "ERROR: " << query.lastError().text();

    main_model->select();
//    model->insertRows(model->rowCount(),3);
//    QSqlRecord record;
//    record.setValue("f_type", QString(finance.getType()));
//    record.setValue("f_date", QString(finance.getDate().toString("'dd-MM-yyyy'")));
//    record.setValue("f_category", QString(finance.getCategory()));
//    record.setValue("f_sum", QString(finance.getSum()));
//    record.setValue("f_id", QString(finance.getType()));
//    if(model->insertRecord(-1,record))
//        qDebug()<<"inserted";
//    else qDebug() << "didn't insert";
//    model->submitAll();

//    model->setQuery();
}
