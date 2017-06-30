#ifndef MYSQLTABLEFRM_H
#define MYSQLTABLEFRM_H

#include <QFrame>

namespace Ui {
class MySqlTableFrm;
}

class MySqlTableFrm : public QFrame
{
    Q_OBJECT

public:
    explicit MySqlTableFrm(QWidget *parent = 0);
    ~MySqlTableFrm();

private:
    Ui::MySqlTableFrm *ui;
};

#endif // MYSQLTABLEFRM_H
