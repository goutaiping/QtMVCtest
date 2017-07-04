#ifndef TABLETYPES_H
#define TABLETYPES_H

#include <QString>
#include <QVariant>

// namespace QtMVCTest start
//namespace QtMVCTest {

enum Sex
{
    Male, Female
};

enum Grade
{
    Grade_1,
    Grade_2,
    Grade_3,
    Grade_4,
    Grade_5,
    Grade_6,
    Grade_7,
    Grade_8,
    Grade_9,
    Grade_10,
    Grade_11,
    Grade_12,
    Grade_13,
    Grade_14,
    Grade_15,
    Grade_16,
    Grade_17,
    Grade_18,
    Grade_19,
    Grade_20
};

enum Subject
{

};

class Field
{
public:
    Field();

    QString fdName;     /* field name */
    QString udfName;   /* user define name */
    QVariant value;
    int         len;
};

///
/// \brief The Table class
///
class Table
{
public:
    Table();

    Field id;
    Field ctTime;
    Field utTime;
};

class TableSchool
{
public:
    TableSchool();

    Field name;
};

///
/// \brief The TableClass class
///
class TableClass : public Table
{
public:
    TableClass();

    Field name;
    Field year;
    Field generation;
    Field grade;
    Field orderNum;
};

///
/// \brief The TableStudent class
///
class TableStudent : public Table
{
public:
    TableStudent();

    Field name;
    Field sex;
    Field age;
    Field interests;

};


//}// namespace QtMVCTest end
#endif // TABLETYPES_H
