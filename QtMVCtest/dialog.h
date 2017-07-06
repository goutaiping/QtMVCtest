#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUuid>
#include <QtSql>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>
#include <QListWidget>
#include <QTreeWidget>

namespace Ui {
class Dialog;
}

class PagedSqlTableFrame;
class PagedSqlTableModel;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
   void onItemClicked(QTreeWidgetItem *item, int column);
   void showTWMenu(const QPoint &pos);

   void onNewConn();

private:
    Ui::Dialog *ui;

    PagedSqlTableFrame *mFrmSqlTable;

    QString mDbHost;
    int         mDbPort;
    QString mDbUser;
    QString mDbPasswd;
};

#endif // DIALOG_H
