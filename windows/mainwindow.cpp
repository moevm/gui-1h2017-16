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
    initCategoryCheckBoxesView();
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

void MainWindow::initCategoryCheckBoxesView()
{
   QSqlTableModel* model = dbservice->getIncomeCategoryModel();
   int rowCount = model ->rowCount();

   for(int i = 0; i<rowCount;i++){
       QListWidgetItem *item = new QListWidgetItem;
       item->setData(Qt::DisplayRole, model->index(i,1).data().toString());
       item->setData(Qt::CheckStateRole, Qt::Checked );
       ui->incomeCategoryListWidget->addItem( item );
   }

   model = dbservice->getExpenseCategoryModel();
   rowCount = model ->rowCount();

   for(int i = 0; i<rowCount;i++){
       QListWidgetItem *item = new QListWidgetItem;
       item->setData(Qt::DisplayRole, model->index(i,1).data().toString());
       item->setData(Qt::CheckStateRole, Qt::Checked );
       ui->expenseCategoryListWidget->addItem( item );
   }
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
    QObject::connect(ui->incomeCategoryListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(updateFromIncomeCategoryListWidget(QListWidgetItem*)));
    QObject::connect(ui->expenseCategoryListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(updateFromExpenseCategoryListWidget(QListWidgetItem*)));


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
    qDebug()<<"in update model filter";
    dbservice->updateMainModelFilter(filterInterval,filterType,currentDate);
    updateCategoryListWidgets();
    updateChart();
}

void MainWindow::updateCategoryListWidgets()
{
    fillCategoryListWidget( ui->incomeCategoryListWidget, dbservice->getPossibleIncomeCategories());
    fillCategoryListWidget( ui->expenseCategoryListWidget, dbservice->getPossibleExpenseCategories());
}

void MainWindow::fillCategoryListWidget(QListWidget *widget, QSet<QString> set)
{
    for (int i=0;i<widget->count();i++){
        QListWidgetItem *item = widget->item(i);
        if(set.contains(item->text())){
            qDebug()<<"show "<<item->text();
            widget->item(i)->setHidden(false);
            emit widget->itemChanged(item);
        }else{
            qDebug()<<"hide "<<item->text();
            widget->item(i)->setHidden(true);
        }
    }
}

void MainWindow::updateTableView(QListWidgetItem *item, QString type)
{
    QString category = item->text();

    if(item->checkState()== Qt::Checked){

        qDebug()<<QString("checked: %1").arg(category);
        QSqlTableModel * model = dbservice->getMainModel();
        int rowCount = model ->rowCount();

        QVector<int> rowsToShow;

        for(int i = 0; i<rowCount;i++){
            if(model->index(i,1).data().toString() == type && model->index(i,3).data().toString() == category){
                qDebug()<<i;
                rowsToShow.append(i);
            }
        }

        for(int i=0;i<rowsToShow.size();i++){
            ui->tableView->showRow(rowsToShow.at(i));
        }
    }

    if(item->checkState() == Qt::Unchecked){
        qDebug()<<QString("unchecked: %1").arg(category);

        QSqlTableModel * model = dbservice->getMainModel();
        int rowCount = model ->rowCount();

        QVector<int> rowsToHide;

        for(int i = 0; i<rowCount;i++){

            if(model->index(i,1).data().toString() == type && model->index(i,3).data().toString() == category){
                qDebug()<<i;
                rowsToHide.append(i);
            }
        }

        for(int i=rowsToHide.size()-1;i>=0;i--){
            ui->tableView->hideRow(rowsToHide.at(i));
        }

    }
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
    int curRightPanelIndex = ui->rightPanelStack -> currentIndex();

    if(curContentIndex == 0){
        ui->toggleContentStackButton->setText(QObject::tr("Показать таблицу"));
        ui->addDataButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
        ui->incomeCategoryListWidget->setEnabled(false);
        ui->expenseCategoryListWidget->setEnabled(false);
    }else{
        ui->toggleContentStackButton->setText(QObject::tr("Показать диаграмму"));
        ui->addDataButton->setEnabled(true);
        ui->deleteButton->setEnabled(true);
        ui->incomeCategoryListWidget->setEnabled(true);
        ui->expenseCategoryListWidget->setEnabled(true);
    }

    updateChart();
    ui->contentStack->setCurrentIndex((curContentIndex + 1)%2);
    ui->controlPanelStack->setCurrentIndex((curPanelIndex + 1)%2);
    ui->rightPanelStack->setCurrentIndex((curRightPanelIndex + 1)%2);
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
    if(ui->balanceDiagramRadioButton->isChecked()) {
        currentChart = UtilEnums::BALANCE;
    }
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

void MainWindow::updateFromIncomeCategoryListWidget(QListWidgetItem* item)
{
    updateTableView(item, "доходы");
}

void MainWindow::updateFromExpenseCategoryListWidget(QListWidgetItem* item)
{
    updateTableView(item, "расходы");
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
    QListWidget* widget;
    if(finance.getType()=="доходы"){
        widget = ui->incomeCategoryListWidget;
    }else{
        widget = ui->expenseCategoryListWidget;
    }

    for (int i=0;i<widget->count();i++){
        if(widget->item(i)->text() == finance.getCategory()){
            widget->item(i)->setHidden(false);
            break;
        }
    }

}
void MainWindow::addIncomeCategory(QString s)
{
    dbservice->addIncomeCategory(s);
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::DisplayRole, s);
    item->setData(Qt::CheckStateRole, Qt::Checked );
    ui->incomeCategoryListWidget->addItem( item );
    item->setHidden(true);
}
void MainWindow::addExpenseCategory(QString s)
{
    dbservice->addExpenseCategory(s);
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::DisplayRole, s);
    item->setData(Qt::CheckStateRole, Qt::Checked );
    ui->expenseCategoryListWidget->addItem( item );
     item->setHidden(true);
}
void MainWindow::deleteIncomeCategory(QModelIndexList indexes)
{
    QSet<QString> categoryNamesToDelete = dbservice->deleteIncomeModelData(indexes);
    QListWidget* widget = ui->incomeCategoryListWidget;
    QVector<int> itemsToDelete;
    for (int i=0;i<widget->count();i++){
        if(categoryNamesToDelete.contains(widget->item(i)->text())){
            itemsToDelete.append(i);
        }
    }

    for(int i=itemsToDelete.size()-1;i>=0;i--){
        widget->takeItem(itemsToDelete.at(i));
    }

}
void MainWindow::deleteExpenseCategory(QModelIndexList indexes)
{
    QSet<QString> categoryNamesToDelete = dbservice->deleteExpenseModelData(indexes);
    QListWidget* widget = ui->expenseCategoryListWidget;
    QVector<int> itemsToDelete;
    for (int i=0;i<widget->count();i++){
        if(categoryNamesToDelete.contains(widget->item(i)->text())){
            itemsToDelete.append(i);
        }
    }

    for(int i=itemsToDelete.size()-1;i>=0;i--){
        widget->takeItem(itemsToDelete.at(i));
    }

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

    QSqlTableModel * model = dbservice->getMainModel();
    int rowCount = model ->rowCount();

    QSet<QString> existingIncomeCategories;
    QSet<QString> existingExpenseCategories;

    for(int i = 0; i<rowCount;i++){
        if(model->index(i,1).data().toString() == "доходы"){
            qDebug()<<"added " <<model->index(i,3).data().toString()<<" to existing incomes";
            existingIncomeCategories.insert(model->index(i,3).data().toString());
        }
        else{
            qDebug()<<"added " <<model->index(i,3).data().toString()<<" to existing expenses";
            existingExpenseCategories.insert(model->index(i,3).data().toString());
        }
    }

    QListWidget* widget;

    widget=ui->incomeCategoryListWidget;

    for (int i=0;i<widget->count();i++){
        if(!existingIncomeCategories.contains(widget->item(i)->text())){
            qDebug()<<"need to hide " << widget->item(i)->text();
            widget->item(i)->setHidden(true);
        }
    }

    widget=ui->expenseCategoryListWidget;

    for (int i=0;i<widget->count();i++){
        if(!existingExpenseCategories.contains(widget->item(i)->text())){
            widget->item(i)->setHidden(true);
        }
    }

}
