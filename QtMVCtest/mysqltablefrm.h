#ifndef MYSQLTABLEFRM_H
#define MYSQLTABLEFRM_H

#include <QFrame>

namespace Ui {
class MySqlTableFrm;
}

class MySqlModel;
class Table;

class MySqlTableFrm : public QFrame
{
    Q_OBJECT

public:
    explicit MySqlTableFrm(QWidget *parent = 0);
    ~MySqlTableFrm();

protected slots:
   void toNextPage();
   void toPrevPage();
   void toFirstPage();
   void toLastPage();
   void toPage(const int page);

   void setAdvancedKey(Table *key);
   void setModel(MySqlModel *model);

private:
    Ui::MySqlTableFrm *ui;
protected:
    MySqlModel *mModel;
};

#endif // MYSQLTABLEFRM_H
