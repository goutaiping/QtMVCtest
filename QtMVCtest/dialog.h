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
   void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
   void onItemDoubleClicked(QTreeWidgetItem *item, int column);
   void showTWMenu(const QPoint &pos);

   void onCreateConn();
   void onDeleteConn();
   void onCheckDbConfig();

   void loadIni();

protected:
   enum ItemType {
       ConnectionItem,
       DatabaseItem,
       TableItem
   };
   QTreeWidgetItem *createConnectionItem(const QStringList &strings);
   QTreeWidgetItem *createDatabaseItem(const QStringList &strings, QTreeWidgetItem *connItem);
   QTreeWidgetItem *createTableItem(const QStringList &strings, QTreeWidgetItem *databaseItem);
   int getItemType(QTreeWidgetItem *item);

private:
    Ui::Dialog *ui;

    PagedSqlTableFrame *mFrmSqlTable;
};

#endif // DIALOG_H
