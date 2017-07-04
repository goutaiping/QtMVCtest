#include "studentsqlmodel.h"

StudentSqlModel::StudentSqlModel(QObject *parent) :
    MySqlModel(parent)
{

}

StudentSqlModel::~StudentSqlModel()
{

}

bool StudentSqlModel::selectByAdvacedKey()
{
    QSqlDatabase db = getDb();
    if (!db.isOpen())
        return false;

    beginResetModel();
    if (mQuery)
        delete mQuery;
    mQuery = new QSqlQuery(db);
    mQuery->exec("use QtSqlTableFrameworkTest;");
    mQuery->exec("select * from student;");
    endResetModel();


}

