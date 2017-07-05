#include "dialog.h"
#include "ui_dialog.h"
#include "mysqlmodel.h"
#include "tabletypes.h"
#include "pagedsqltableframe.h"
#include "pagedsqltablemodel.h"
#include <QHBoxLayout>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    mDlgProgress = new QProgressDialog(this);



    //insertRecord();

    QHBoxLayout *hLay = new QHBoxLayout(this);
    mFrmSqlTable = new PagedSqlTableFrame(this);
    hLay->addWidget(mFrmSqlTable);

    mFrmSqlTable->setModelQueryFields("*");
    mFrmSqlTable->setModelQueryFilter("1");
    mFrmSqlTable->setModelQueryOrderByFields("");
    mFrmSqlTable->setModelQueryTable("student");

    mFrmSqlTable->onFirstPage();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::createTable()
{

}

void Dialog::insertRecord()
{
    InsertRecordThread *t = new InsertRecordThread(this);
    connect(t, SIGNAL(sendStatus(QString)), mDlgProgress, SLOT(setLabelText(QString)));
    connect(t, SIGNAL(sendProgressMaximum(int)), mDlgProgress, SLOT(setMaximum(int)));
    connect(t, SIGNAL(sendProgressValue(int)), mDlgProgress, SLOT(setValue(int)));
    connect(t, SIGNAL(started()), this, SLOT(onInsertRecordThreadStarted()));
    connect(t, SIGNAL(finished()), this, SLOT(onInsertRecordThreadFinished()));
    t->start();
}

void Dialog::onInsertRecordThreadStarted()
{
    mDlgProgress->show();
}

void Dialog::onInsertRecordThreadFinished()
{

}

void InsertRecordThread::run()
{
    // connect database
    emit sendStatus("connecting database......");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "qt_insert_mysql_conn");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("1234");
    if (!db.open()) {
        emit sendStatus(db.lastError().text());
        return;
    }
    emit sendStatus("connect database succeed");

    // create database and table
    emit sendStatus("create database and table......");
    QSqlQuery q(db);
    QString dbName = "QtSqlTableFrameworkTest";
    if (!runQuery(q, QString("drop database if exists %1;").arg(dbName)))
        return;
    if (!runQuery(q, QString("create database %1;").arg(dbName)))
        return;
    if (!runQuery(q, QString("use %1;").arg(dbName)))
        return;
    if (!runQuery(q,
                "create table student("
                "id varchar(48) not null primary key, "
                "name varchar(32) not null, "
                "sex enum('male', 'female') default 'male', "
                "age tinyint default 0, "
                "interests varchar(128) default null"
                ");"
                ))
        return;
    emit sendStatus("create database and table succeed");

    // insert data
    TableStudent p;
    QString statement;
    int finished = 0, total = 10000;
    emit sendProgressMaximum(10000);
    emit sendProgressValue(finished);
    for (int i = 0; i < total; i ++) {
        p.name.value = QString("Peter %1").arg(i);
        if (i % 2 == 0)
            p.sex.value = "male";
        else
            p.sex.value = "female";
        p.age.value = i % 30;
        p.interests.value = QString("run %1 meters every morning").arg(i);
        statement = QString("insert into student(id, name, sex, age, interests) values(uuid(), '%1', '%2', %3, '%4');")
                .arg(p.name.value.toString())
                .arg(p.sex.value.toString())
                .arg(p.age.value.toInt())
                .arg(p.interests.value.toString());
        if (!runQuery(q, statement))
            return;

        emit sendProgressValue(++ finished);
    }
}

bool InsertRecordThread::runQuery(QSqlQuery &q, const QString &statement)
{
    if (!q.exec(statement)) {
        emit sendStatus(q.lastError().text());
        return false;
    }
    return true;
}
