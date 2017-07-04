#include "studenttablefrm.h"
#include "studentsqlmodel.h"

StudentTableFrm::StudentTableFrm(QWidget *parent) :
    MySqlTableFrm(parent)
{
    mModel = new StudentSqlModel(this);
    setModel(mModel);
}

StudentTableFrm::~StudentTableFrm()
{

}

