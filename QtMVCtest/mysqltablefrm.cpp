#include "mysqltablefrm.h"
#include "ui_mysqltablefrm.h"
#include "mysqlmodel.h"

MySqlTableFrm::MySqlTableFrm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MySqlTableFrm),
    mModel(NULL)
{
    ui->setupUi(this);
}

MySqlTableFrm::~MySqlTableFrm()
{
    delete ui;
}

void MySqlTableFrm::toNextPage()
{
    if (mModel)
        mModel->toNextPage();
}

void MySqlTableFrm::toPrevPage()
{
    if (mModel)
        mModel->toPrevPage();
}

void MySqlTableFrm::toFirstPage()
{
    if (mModel)
        mModel->toFirstPage();
}

void MySqlTableFrm::toLastPage()
{
    if (mModel)
        mModel->toLastPage();
}

void MySqlTableFrm::toPage(const int page)
{
    if (mModel)
        mModel->toPage(page);
}

void MySqlTableFrm::setAdvancedKey(Table *key)
{
    if (mModel)
        mModel->setAdvancedKey(key);
}

void MySqlTableFrm::setModel(MySqlModel *model)
{
    ui->tableView->setModel(model);
}
