#include "dialog.h"
#include "ui_dialog.h"
#include "pagedsqltableframe.h"
#include "pagedsqltablemodel.h"
#include <QHBoxLayout>
#include <QMenu>
#include "dbconfigdialog.h"
#include <QMessageBox>
#include "iniloader.h"
#include <QDebug>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    setWindowTitle(QString::fromLocal8Bit("Qt MySQL 连接测试"));

    loadIni();

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showTWMenu(QPoint)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if (!current)
        return;

    if (TableItem != getItemType(current))
        return;

    for (int i = 0; i < ui->stackedWidget->count(); i ++) {
        QWidget *w = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(w);
        delete w;
    }

    ConnMsg msg = IniLoader::getConn(current->parent()->parent()->text(0));
    PagedSqlTableFrame *frm = new PagedSqlTableFrame(this);
    frm->setModelDbName(current->parent()->text(0));
    frm->setModelDbHost(msg.connHost);
    frm->setModelDbPort(msg.connPort);
    frm->setModelDbUser(msg.connUser);
    frm->setModelDbPasswd(msg.connPasswd);
    frm->setModelQueryFields("*");
    frm->setModelQueryFilter("1");
    frm->setModelQueryOrderByFields("");
    frm->setModelQueryTable(current->text(0));

    ui->stackedWidget->addWidget(frm);
    frm->onBeginSearch();
    ui->stackedWidget->setCurrentWidget(frm);
}

void Dialog::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString connName = "qt_mysql_getNewConn_conn";

    // 如果改item有子item,说明列表已经获取过,不重复获取
    if (item->childCount() > 0)
        return;

    // 连接数据库
    ConnMsg msg;
    if (ConnectionItem == getItemType(item))
        msg = IniLoader::getConn(item->text(column));
    else if (DatabaseItem == getItemType(item))
        msg = IniLoader::getConn(item->parent()->text(column));
    else
        return;

    QSqlDatabase db;
    if (QSqlDatabase::contains(connName))
        db = QSqlDatabase::database(connName);
    else
        db = QSqlDatabase::addDatabase("QMYSQL", connName);
    db.setHostName(msg.connHost);
    db.setPort(msg.connPort);
    db.setUserName(msg.connUser);
    db.setPassword(msg.connPasswd);
    if (!db.open()) {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"),
                                 db.lastError().text(), QMessageBox::Yes);
        return;
    }

    // 获取列表
    if (ConnectionItem == getItemType(item)) {
        // 获取数据库列表
        QSqlQuery q(db);
        q.exec("show databases;");
        while (q.next())
            createDatabaseItem(QStringList() << q.value(0).toString(), item);

    } else if (DatabaseItem == getItemType(item)) {
        // 获取数据库表列表
        QSqlQuery q(db);
        if (!q.exec(QString("use %1;").arg(item->text(column)))) {
            QMessageBox::information(this, QString::fromLocal8Bit("提示"),
                                     q.lastError().text(), QMessageBox::Yes);
            return;
        }
        q.exec("show tables;");
        while (q.next())
            createTableItem(QStringList() << q.value(0).toString(), item);

    }
}

void Dialog::showTWMenu(const QPoint &pos)
{
    QMenu m(ui->treeWidget);

    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (!item) {
        m.addAction(QString::fromLocal8Bit("新建连接"), this, SLOT(onCreateConn()));

    } else if (ConnectionItem == getItemType(item)) {
        m.addAction(QString::fromLocal8Bit("新建连接"), this, SLOT(onCreateConn()));
        m.addAction(QString::fromLocal8Bit("删除连接"), this, SLOT(onDeleteConn()));
        m.addAction(QString::fromLocal8Bit("查看连接参数"), this, SLOT(onCheckDbConfig()));

    } else if (DatabaseItem == getItemType(item)) {
        m.addAction(QString::fromLocal8Bit("这是一个数据库"));

    } else if (TableItem == getItemType(item)) {
        m.addAction(QString::fromLocal8Bit("这是一个表"));

    }
    m.exec(QCursor::pos());
}

void Dialog::onCreateConn()
{
    // 获取连接参数
    DbConfigDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("新建连接"));
    if (QDialog::Rejected == dlg.exec())
        return;
    ConnMsg msg;
    msg.connName = dlg.connName();
    msg.connHost = dlg.host();
    msg.connPort = dlg.port();
    msg.connUser = dlg.user();
    msg.connPasswd = dlg.password();

    // 检查连接名是否已经存在
    if (IniLoader::isConnExists(msg.connName)) {
        QMessageBox::information(this,
                                 QString::fromLocal8Bit("提示"),
                                 QString::fromLocal8Bit("连接%1已经存在").arg(msg.connName),
                                 QMessageBox::Yes);
        return;
    }

    // 连接数据库
    QString connName = "qt_mysql_getNewConn_conn";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connName))
        db = QSqlDatabase::database(connName);
    else
        db = QSqlDatabase::addDatabase("QMYSQL", connName);
    db.setHostName(msg.connHost);
    db.setPort(msg.connPort);
    db.setUserName(msg.connUser);
    db.setPassword(msg.connPasswd);
    if (!db.open()) {
        QMessageBox::information(this,
                                 QString::fromLocal8Bit("提示"),
                                 db.lastError().text(),
                                 QMessageBox::Yes);
        return;
    }

    // 保存连接
    IniLoader::addConn(msg);

    // 创建连接item
    createConnectionItem(QStringList() << msg.connName);
}

void Dialog::onDeleteConn()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();

    if (!item)
        return;

    IniLoader::removeConn(item->text(0));
    ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
    delete item;
    item = NULL;
}

void Dialog::onCheckDbConfig()
{
    if (!ui->treeWidget->currentItem())
        return;

    ConnMsg conn = IniLoader::getConn(ui->treeWidget->currentItem()->text(0));
    DbConfigDialog dlg(this);
    dlg.setWindowTitle(QString::fromLocal8Bit("查看连接参数"));
    dlg.setConnName(conn.connName);
    dlg.setHost(conn.connHost);
    dlg.setPort(conn.connPort);
    dlg.setUser(conn.connUser);
    dlg.setPassword(conn.connPasswd);
    dlg.exec();
}

void Dialog::loadIni()
{
    while (ui->treeWidget->topLevelItemCount() > 0) {
        QTreeWidgetItem *item = ui->treeWidget->takeTopLevelItem(0);
        delete item;
    }

    QStringList connNames = IniLoader::getConnNames();
    for (int i = 0; i < connNames.size(); i ++)
        createConnectionItem(QStringList() << connNames.at(i));
}

QTreeWidgetItem *Dialog::createConnectionItem(const QStringList &strings)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(strings);
    item->setData(0, Qt::UserRole + 1, ConnectionItem);
    ui->treeWidget->addTopLevelItem(item);
    return item;
}

QTreeWidgetItem *Dialog::createDatabaseItem(const QStringList &strings, QTreeWidgetItem *connItem)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(connItem, strings);
    item->setData(0, Qt::UserRole + 1, DatabaseItem);
    return item;
}

QTreeWidgetItem *Dialog::createTableItem(const QStringList &strings, QTreeWidgetItem *databaseItem)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(databaseItem, strings);
    item->setData(0, Qt::UserRole + 1, TableItem);
    return item;
}

int Dialog::getItemType(QTreeWidgetItem *item)
{
    return item->data(0, Qt::UserRole + 1).toInt();
}




