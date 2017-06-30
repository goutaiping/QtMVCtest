#include "mysqltablefrm.h"
#include "ui_mysqltablefrm.h"

MySqlTableFrm::MySqlTableFrm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MySqlTableFrm)
{
    ui->setupUi(this);
}

MySqlTableFrm::~MySqlTableFrm()
{
    delete ui;
}
