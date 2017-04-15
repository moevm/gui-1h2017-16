#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initParameters();
    initModels();
    editTableView();
    makeConnects();
}

MainWindow::~MainWindow()
{
    delete form;
    delete categoryForm;
    delete ui;
}

void MainWindow::initParameters()
{
    form = new Form;
    categoryForm = new CategoryForm;
    currentDate = QDate::currentDate();
    ui->dateEditField->setDate(currentDate);
    filterInterval = DAY;
    filterType = BOTH;
}

void MainWindow::initModels()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("C:\IEdb.db3");
    sdb.setDatabaseName("D:\IEdb.db3");
    if(!sdb.open()) qDebug()<<"doesn't work";
    else qDebug()<< "db has opened";

    initMainModel(sdb);

    income_category_model = new QSqlTableModel(0,sdb);
    income_category_model->setTable("income_categories");
    income_category_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    income_category_model->select();
    categoryForm->setIncomeCategoriesModel(income_category_model);

    expense_category_model = new QSqlTableModel(0,sdb);
    expense_category_model->setTable("expense_categories");
    expense_category_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    expense_category_model->select();
    categoryForm->setExpenseCategoriesModel(expense_category_model);
}

//init models
void MainWindow::initMainModel(QSqlDatabase sdb)
{
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
}


void MainWindow::editTableView()
{
     ui->tableView->setModel(main_model);
     ui->tableView->verticalHeader()->hide();
     ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     ui->tableView->hideColumn(0); // don't show the ID
     ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
void MainWindow::makeConnects(){
    //main window
    QObject::connect(ui->addDataButton,SIGNAL(clicked(bool)),this,SLOT(openForm()));
    QObject::connect(ui->toggleContentStackButton,SIGNAL(clicked(bool)),this,SLOT(toggleContent()));
    QObject::connect(ui->deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteData()));

    //date UI
    QObject::connect(ui->dateEditField,SIGNAL(dateChanged(QDate)),this,SLOT(changeCurrentDate(QDate)));
    QObject::connect(ui->nextDateButton,SIGNAL(clicked(bool)),this,SLOT(incrementCurrentDate()));
    QObject::connect(ui->previousDateButton,SIGNAL(clicked(bool)),this,SLOT(decrementCurrentDate()));

    //interval group
    QObject::connect(ui->dayRadioButton,SIGNAL(clicked(bool)),this,SLOT(setDayInterval()));
    QObject::connect(ui->monthRadioButton,SIGNAL(clicked(bool)),this,SLOT(setMonthInterval()));
    QObject::connect(ui->yearRadioButton,SIGNAL(clicked(bool)),this,SLOT(setYearInterval()));

    //type group
    QObject::connect(ui->incomesCheckBox,SIGNAL(clicked(bool)),this,SLOT(checkShowingTypes()));
    QObject::connect(ui->expensesCheckBox,SIGNAL(clicked(bool)),this,SLOT(checkShowingTypes()));

    //from form
    QObject::connect(form,SIGNAL(closed()),this,SLOT(formClosed()));
    QObject::connect(form,SIGNAL(dataChecked(Finance)),this,SLOT(saveNewData(Finance)));
    QObject::connect(form,SIGNAL(categoryToolButtonPressed()),this,SLOT(openCategoryForm()));
    QObject::connect(form,SIGNAL(incomesRadioButtonSignal()),this,SLOT(setIncomesComboBoxModel()));
    QObject::connect(form,SIGNAL(expensesRadioButtonSignal()),this,SLOT(setExpensesComboBoxModel()));

    //from category form
    QObject::connect(categoryForm,SIGNAL(addIncomeCategorySignal(QString)),this,SLOT(addIncomeCategory(QString)));
    QObject::connect(categoryForm,SIGNAL(addExpenseCategorySignal(QString)),this,SLOT(addExpenseCategory(QString)));
    QObject::connect(categoryForm,SIGNAL(deleteIncomeCategorySignal(QModelIndexList)),this,SLOT(deleteIncomeCategory(QModelIndexList)));
    QObject::connect(categoryForm,SIGNAL(deleteExpenseCategorySignal(QModelIndexList)),this,SLOT(deleteExpenseCategory(QModelIndexList)));
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

//    qDebug() << filterString;
    main_model->setFilter(filterString);
}

//open other windows
void MainWindow::openForm()
{
    ui->addDataButton->setEnabled(false);
    form->setDate(currentDate);
    form->setModel(income_category_model);
    form->show();
}
void MainWindow::openCategoryForm()
{
    categoryForm->show();
}

void MainWindow::toggleContent(){
    int curContentIndex = ui -> contentStack -> currentIndex();
    int curPanelIndex = ui->controlPanelStack -> currentIndex();

    ui->contentStack->setCurrentIndex((curContentIndex + 1)%2);
    ui->controlPanelStack->setCurrentIndex((curPanelIndex + 1)%2);
}

//filters
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
void MainWindow::changeCurrentDate(QDate date)
{
    currentDate = date;
    updateModelFilter();
}

//date << and >>
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

void MainWindow::formClosed()
{
    ui->addDataButton->setEnabled(true);
}

void MainWindow::setIncomesComboBoxModel()
{
    form->setModel(income_category_model);
}

void MainWindow::setExpensesComboBoxModel()
{
    form->setModel(expense_category_model);
}

//crud
void MainWindow::saveNewData(Finance finance)
{
    qDebug()<< finance.toString();
    QString insertQueryString = QString("INSERT OR IGNORE INTO f_data(f_type, f_date, f_category, f_sum) VALUES('%1', '%2', '%3', %4 )")
            .arg(finance.getType())
            .arg(finance.getDate().toString("yyyy-MM-dd"))
            .arg(finance.getCategory())
            .arg(finance.getSum());

    qDebug()<<insertQueryString;

    QString updateQueryString = QString("UPDATE OR IGNORE f_data SET f_sum = f_sum + %1 where (f_type = '%2' AND f_date = '%3' AND f_category = '%4')")
             .arg(finance.getSum())
            .arg(finance.getType())
            .arg(finance.getDate().toString("yyyy-MM-dd"))
            .arg(finance.getCategory());

    qDebug()<<updateQueryString;

    QSqlQuery query;

    if(!query.exec(updateQueryString))
        qDebug() << "ERROR: " << query.lastError().text();

    if(!query.exec(insertQueryString))
        qDebug() << "ERROR: " << query.lastError().text();

    main_model->select();
}
void MainWindow::addIncomeCategory(QString s)
{
    QString insertQueryString = QString("INSERT OR IGNORE INTO income_categories(c_name) VALUES('%1')")
            .arg(s);
    qDebug()<<insertQueryString;
    QSqlQuery query;
    if(!query.exec(insertQueryString))
        qDebug() << "ERROR: " << query.lastError().text();
    income_category_model->select();
}

void MainWindow::addExpenseCategory(QString s)
{
    QString insertQueryString = QString("INSERT OR IGNORE INTO expense_categories(c_name) VALUES('%1')")
            .arg(s);
    qDebug()<<insertQueryString;
    QSqlQuery query;
    if(!query.exec(insertQueryString))
        qDebug() << "ERROR: " << query.lastError().text();
    expense_category_model->select();
}

void MainWindow::deleteIncomeCategory(QModelIndexList indexes)
{
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
           income_category_model->removeRow( indexes.at(i-1).row(), QModelIndex());
    income_category_model->submitAll();
    income_category_model->select();
}

void MainWindow::deleteExpenseCategory(QModelIndexList indexes)
{
    qDebug() << "yo yo yo";
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
           expense_category_model->removeRow( indexes.at(i-1).row(), QModelIndex());
    expense_category_model->submitAll();
    expense_category_model->select();
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
