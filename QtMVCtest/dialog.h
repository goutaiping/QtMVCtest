#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUuid>
#include <QtSql>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>

namespace Ui {
class Dialog;
}


class InsertRecordThread : public QThread
{
    Q_OBJECT

public:
    InsertRecordThread(QObject *parent = 0)
    {}
    ~InsertRecordThread()
    {}
protected:
    void run();

    bool runQuery(QSqlQuery &q, const QString &statement);
signals:
    void sendStatus(const QString &status);
    void sendProgressMaximum(int max);
    void sendProgressValue(int value);
};


class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void createTable();
    void insertRecord();
    void onInsertRecordThreadStarted();
    void onInsertRecordThreadFinished();

private:
    Ui::Dialog *ui;

    QProgressDialog *mDlgProgress;
};

#endif // DIALOG_H
