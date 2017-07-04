#ifndef STUDENTSQLMODEL_H
#define STUDENTSQLMODEL_H

#include "mysqlmodel.h"

class StudentSqlModel : public MySqlModel
{
    Q_OBJECT
public:
    StudentSqlModel(QObject *parent = 0);
    ~StudentSqlModel();

protected:
    bool selectByAdvacedKey();
};

#endif // STUDENTSQLMODEL_H
