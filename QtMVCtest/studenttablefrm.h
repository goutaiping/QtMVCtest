#ifndef STUDENTTABLEFRM_H
#define STUDENTTABLEFRM_H

#include "mysqltablefrm.h"

class StudentTableFrm : public MySqlTableFrm
{
    Q_OBJECT
public:
    explicit StudentTableFrm(QWidget *parent = 0);
    ~StudentTableFrm();

public:

protected:

};

#endif // STUDENTTABLEFRM_H
