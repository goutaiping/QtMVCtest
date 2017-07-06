#include "dbconfigdialog.h"
#include "ui_dbconfigdialog.h"

DbConfigDialog::DbConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbConfigDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_yes, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

DbConfigDialog::~DbConfigDialog()
{
    delete ui;
}

QString DbConfigDialog::host()
{
    return ui->lineEdit_host->text();
}

int DbConfigDialog::port()
{
    return ui->lineEdit_port->text().toInt();
}

QString DbConfigDialog::user()
{
    return ui->lineEdit_user->text();
}

QString DbConfigDialog::password()
{
    return ui->lineEdit_passwd->text();
}
