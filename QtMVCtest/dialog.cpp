#include "dialog.h"
#include "ui_dialog.h"
#include "pagedsqltableframe.h"
#include "pagedsqltablemodel.h"
#include <QHBoxLayout>
#include <QMenu>
#include "dbconfigdialog.h"
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    mDbPort(3306)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(onTableChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showTWMenu(QPoint)));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onTableChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if (!current->parent())
        return;

    for (int i = 0; i < ui->stackedWidget->count(); i ++) {
        QWidget *w = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(w);
        delete w;
    }

    PagedSqlTableFrame *frm = new PagedSqlTableFrame(this);
    frm->setModelDbName(current->parent()->text(0));
    frm->setModelDbHost(mDbHost);
    frm->setModelDbPort(mDbPort);
    frm->setModelDbUser(mDbUser);
    frm->setModelDbPasswd(mDbPasswd);
    frm->setModelQueryFields("*");
    frm->setModelQueryFilter("1");
    frm->setModelQueryOrderByFields("");
    frm->setModelQueryTable(current->text(0));

    ui->stackedWidget->addWidget(frm);
    frm->onBeginSearch();
    ui->stackedWidget->setCurrentWidget(frm);
}

void Dialog::showTWMenu(const QPoint &pos)
{
    Q_UNUSED(pos);

    QMenu m(ui->treeWidget);
    m.addAction(QString::fromLocal8Bit("新建连接"), this, SLOT(onNewConn()));
    m.addAction(QString::fromLocal8Bit("查看当前连接参数"), this, SLOT(onCheckDbConfig()));
    m.exec(QCursor::pos());
}

void Dialog::onNewConn()
{
    // 获取连接参数
    DbConfigDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("新建连接"));
    if (QDialog::Rejected == dlg.exec())
        return;
    mDbHost = dlg.host();
    mDbPort = dlg.port();
    mDbUser = dlg.user();
    mDbPasswd = dlg.password();

    ui->treeWidget->clear();

    // 连接数据库
    QString connName = "qt_mysql_getNewConn_conn";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connName))
        db = QSqlDatabase::database(connName);
    else
        db = QSqlDatabase::addDatabase("QMYSQL", connName);
    db.setHostName(mDbHost);
    db.setPort(mDbPort);
    db.setUserName(mDbUser);
    db.setPassword(mDbPasswd);
    if (!db.open()) {
        QMessageBox::information(this,
                                 QString::fromLocal8Bit("提示"),
                                 db.lastError().text(),
                                 QMessageBox::Yes);
        return;
    }

    // 获取数据库列表
    QSqlQuery q(db);
    q.exec("show databases;");
    while (q.next()) {
        ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList() << q.value(0).toString()));
    }

    // 获取每个数据库的表
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i ++) {
        if (!q.exec(QString("use %1;").arg(ui->treeWidget->topLevelItem(i)->text(0))))
            continue;
        q.exec("show tables;");
        while (q.next()) {
            ui->treeWidget->topLevelItem(i)->addChild(new QTreeWidgetItem(QStringList() << q.value(0).toString()));
        }
    }

}

void Dialog::onCheckDbConfig()
{
    DbConfigDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("当前连接参数"));
    dlg.setHost(mDbHost);
    dlg.setPort(mDbPort);
    dlg.setUser(mDbUser);
    dlg.setPassword(mDbPasswd);
    dlg.exec();
}


