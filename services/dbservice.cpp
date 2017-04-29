#include "dbservice.h"

DBService::DBService()
{

}

void DBService::init()
{
    initDB();
    createTables();
    initModels();
}

QSqlDatabase DBService::getDB()
{
    return sdb;
}

QSqlTableModel *DBService::getIncomeCategoryModel() const
{
    return income_category_model;
}

QSqlTableModel *DBService::getExpenseCategoryModel() const
{
    return expense_category_model;
}

QSqlTableModel *DBService::getMainModel() const
{
    return main_model;
}

void DBService::addFinanceData(Finance finance)
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

void DBService::addIncomeCategory(QString s)
{
    QString insertQueryString = QString("INSERT OR IGNORE INTO income_categories(c_name) VALUES('%1')")
            .arg(s);
//    qDebug()<<insertQueryString;
    QSqlQuery query;
    if(!query.exec(insertQueryString))
        qDebug() << "ERROR: " << query.lastError().text();
    income_category_model->select();
}

void DBService::addExpenseCategory(QString s)
{
    QString insertQueryString = QString("INSERT OR IGNORE INTO expense_categories(c_name) VALUES('%1')")
            .arg(s);
//    qDebug()<<insertQueryString;
    QSqlQuery query;
    if(!query.exec(insertQueryString))
        qDebug() << "ERROR: " << query.lastError().text();
    expense_category_model->select();
}

void DBService::updateMainModelFilter(UtilEnums::Interval interval, UtilEnums::Type type,QDate currentDate)
{
    QString filterString;
    switch(interval){
    case UtilEnums::DAY:
        filterString = QString("f_date = '%1'").arg(currentDate.toString("yyyy-MM-dd"));
        break;
    case UtilEnums::MONTH:
        filterString = QString("f_date BETWEEN '%1-01' AND '%1-31'").arg(currentDate.toString("yyyy-MM"));
        break;
    case UtilEnums::YEAR:
        filterString = QString("f_date BETWEEN '%1-01-01' AND '%1-12-31'").arg(currentDate.toString("yyyy"));
        break;
    default:
        qDebug() << "error in choose interval section";
    }

    switch(type){
    case UtilEnums::NONE:
        filterString.append("AND f_type = 'none'");
        break;
    case UtilEnums::INCOMES:
        filterString.append("AND f_type = 'доходы'");
        break;
    case UtilEnums::EXPENSES:
        filterString.append("AND f_type = 'расходы'");
        break;
    case UtilEnums::BOTH:
        break;
    }

    filterString.append(" order by f_date");

    main_model->setFilter(filterString);
    definePossibleCategories();
}

void DBService::deleteMainModelData(QModelIndexList indexes)
{
//    QModelIndexList indexes =  ui->tableView->selectionModel()->selectedRows();
    int countRow = indexes.count();

    for( int i = countRow; i > 0; i--)
           main_model->removeRow( indexes.at(i-1).row(), QModelIndex());
    main_model->submitAll();
    main_model->select();
}

void DBService::definePossibleCategories()
{
    possibleIncomeCategories.clear();
    possibleExpenseCategories.clear();

    for(int i=0; i < main_model->rowCount();i++){
        if(main_model->index(i,1).data().toString() == "доходы"){
            possibleIncomeCategories.insert(main_model->index(i,3).data().toString());
        }else{
            possibleExpenseCategories.insert(main_model->index(i,3).data().toString());
        }
    }
}

QSqlQuery DBService::getChartQuery(UtilEnums::Interval interval, UtilEnums::ChartType chartType, QDate currentDate)
{
        QString filterString;
        switch(interval){
        case UtilEnums::DAY:
            filterString = QString("f_date = '%1'").arg(currentDate.toString("yyyy-MM-dd"));
            break;
        case UtilEnums::MONTH:
            filterString = QString("f_date BETWEEN '%1-01' AND '%1-31'").arg(currentDate.toString("yyyy-MM"));
            break;
        case UtilEnums::YEAR:
            filterString = QString("f_date BETWEEN '%1-01-01' AND '%1-12-31'").arg(currentDate.toString("yyyy"));
            break;
        default:
            qDebug() << "error in choose interval section";
        }

        QString queryString;
        switch(chartType){
        case UtilEnums::BALANCE:
            queryString = QString("SELECT f_type, f_date, SUM(f_sum) FROM f_data where " + filterString + " group by f_type, f_date order by f_date");
            break;
        case UtilEnums::INCOME:
        case UtilEnums::EXPENSE:
            queryString = QString("SELECT f_type, f_category, SUM(f_sum) FROM f_data where " + filterString + " group by f_type, f_category");
            break;
        }

        QSqlQuery query;
        if(!query.exec(queryString)) qDebug() << "ERROR: " << query.lastError().text();

        return query;

}

QSet<QString> DBService::getPossibleIncomeCategories() const
{
    return possibleIncomeCategories;
}

QSet<QString> DBService::getPossibleExpenseCategories() const
{
    return possibleExpenseCategories;
}

void DBService::deleteIncomeModelData(QModelIndexList indexes)
{
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
           income_category_model->removeRow( indexes.at(i-1).row(), QModelIndex());
    income_category_model->submitAll();
    income_category_model->select();
}

void DBService::deleteExpenseModelData(QModelIndexList indexes)
{
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
           expense_category_model->removeRow( indexes.at(i-1).row(), QModelIndex());
    expense_category_model->submitAll();
    expense_category_model->select();
}

void DBService::initDB()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    //    sdb.setDatabaseName("C:\IEdb.db3");
    sdb.setDatabaseName("D:\IEdb.db3");
    if(!sdb.open()) qDebug()<<"doesn't work";
//    else qDebug()<< "db has opened";
}

void DBService::createTables()
{
    //create if not exist
}

void DBService::initModels()
{
    //MAIN MODEL
    main_model = new QSqlTableModel(0,sdb);
    main_model->setTable("f_data");
    main_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    main_model->setHeaderData(0, Qt::Horizontal, "ID");
    main_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Тип"));
    main_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Дата"));
    main_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Категория"));
    main_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Сумма"));
    main_model->select();

    //INCOME CATEGORY MODEL
    income_category_model = new QSqlTableModel(0,sdb);
    income_category_model->setTable("income_categories");
    income_category_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    income_category_model->select();

    //EXPENSE CATEGORY MODEL
    expense_category_model = new QSqlTableModel(0,sdb);
    expense_category_model->setTable("expense_categories");
    expense_category_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    expense_category_model->select();
}
