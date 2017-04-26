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
    delete dbservice;
}

void MainWindow::initParameters()
{
    form = new Form;
    categoryForm = new CategoryForm;
    dbservice = new DBService;
    currentDate = QDate::currentDate();
    currentChart = UtilEnums::INCOME;
    ui->dateEditField->setDate(currentDate);
    filterInterval = UtilEnums::DAY;
    filterType = UtilEnums::BOTH;
}

void MainWindow::initModels()
{
    dbservice->init();
    updateModelFilter();
    categoryForm->setIncomeCategoriesModel(dbservice->getIncomeCategoryModel());
    categoryForm->setExpenseCategoriesModel(dbservice->getExpenseCategoryModel());

}

void MainWindow::editTableView()
{
     ui->tableView->setModel(dbservice->getMainModel());
     ui->tableView->verticalHeader()->hide();
     ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

    //chart type group
    QObject::connect(ui->incomeDiagramRadioButton,SIGNAL(clicked(bool)),this,SLOT(checkChartType()));
    QObject::connect(ui->expenseDiagramRadioButton,SIGNAL(clicked(bool)),this,SLOT(checkChartType()));
    QObject::connect(ui->balanceDiagramRadioButton,SIGNAL(clicked(bool)),this,SLOT(checkChartType()));

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    form->close();
    categoryForm->close();
    event->accept();
}



void MainWindow::updateModelFilter()
{
    dbservice->updateMainModelFilter(filterInterval,filterType,currentDate);
    updateChart();
}

//open other windows
void MainWindow::openForm()
{
    ui->addDataButton->setEnabled(false);
    form->setDate(currentDate);
    form->setModel(dbservice->getIncomeCategoryModel());
    form->show();
}
void MainWindow::openCategoryForm()
{
    categoryForm->show();
}

void MainWindow::toggleContent(){
    int curContentIndex = ui -> contentStack -> currentIndex();
    int curPanelIndex = ui->controlPanelStack -> currentIndex();

    if(curContentIndex == 0){
        ui->toggleContentStackButton->setText(QObject::tr("Показать таблицу"));
        ui->addDataButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
    }else{
        ui->toggleContentStackButton->setText(QObject::tr("Показать диаграмму"));
        ui->addDataButton->setEnabled(true);
        ui->deleteButton->setEnabled(true);
    }

    updateChart();
    ui->contentStack->setCurrentIndex((curContentIndex + 1)%2);
    ui->controlPanelStack->setCurrentIndex((curPanelIndex + 1)%2);
}

//filters
void MainWindow::setDayInterval()
{
    filterInterval = UtilEnums::DAY;
    updateModelFilter();
}
void MainWindow::setMonthInterval()
{
    filterInterval = UtilEnums::MONTH;
    updateModelFilter();
}
void MainWindow::setYearInterval()
{
    filterInterval = UtilEnums::YEAR;
    updateModelFilter();
}

void MainWindow::checkChartType()
{
    if(ui->incomeDiagramRadioButton->isChecked()) currentChart = UtilEnums::INCOME;
    if(ui->expenseDiagramRadioButton->isChecked()) currentChart = UtilEnums::EXPENSE;
    if(ui->balanceDiagramRadioButton->isChecked()) currentChart = UtilEnums::BALANCE;
    updateChart();
}
void MainWindow::checkShowingTypes()
{
    bool income = ui->incomesCheckBox->isChecked();
    bool expense = ui->expensesCheckBox->isChecked();
    if (income && expense) filterType = UtilEnums::BOTH;
    if (income && !expense) filterType = UtilEnums::INCOMES;
    if (!income && expense) filterType = UtilEnums::EXPENSES;
    if (!income && !expense) filterType = UtilEnums::NONE;
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
    case UtilEnums::DAY:
        currentDate = currentDate.addDays(1);
        break;
    case UtilEnums::MONTH:
        currentDate = currentDate.addMonths(1);
        break;
    case UtilEnums::YEAR:
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
    case UtilEnums::DAY:
        currentDate = currentDate.addDays(-1);
        break;
    case UtilEnums::MONTH:
        currentDate = currentDate.addMonths(-1);
        break;
    case UtilEnums::YEAR:
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
    form->setModel(dbservice->getIncomeCategoryModel());
}
void MainWindow::setExpensesComboBoxModel()
{
    form->setModel(dbservice->getExpenseCategoryModel());
}

//crud
void MainWindow::saveNewData(Finance finance)
{
    dbservice->addFinanceData(finance);
}
void MainWindow::addIncomeCategory(QString s)
{
    dbservice->addIncomeCategory(s);
}
void MainWindow::addExpenseCategory(QString s)
{
    dbservice->addExpenseCategory(s);
}
void MainWindow::deleteIncomeCategory(QModelIndexList indexes)
{
    dbservice->deleteIncomeModelData(indexes);
}
void MainWindow::deleteExpenseCategory(QModelIndexList indexes)
{
    dbservice->deleteExpenseModelData(indexes);
}

void MainWindow::updateChart()
{

    QSqlQuery query = dbservice->getChartQuery(filterInterval,currentChart,currentDate);
    QChart *chart = chartService->updateChart(query, currentChart, filterInterval, currentDate);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::deleteData()
{
    dbservice->deleteMainModelData(ui->tableView->selectionModel()->selectedRows());
}
