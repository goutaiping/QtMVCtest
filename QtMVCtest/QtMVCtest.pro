#-------------------------------------------------
#
# Project created by QtCreator 2017-06-30T16:45:10
#
#-------------------------------------------------

QT       += core gui
QT      += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtMVCtest
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    mysqlmodel.cpp \
    mysqltablefrm.cpp \
    tabletypes.cpp \
    studentsqlmodel.cpp \
    studenttablefrm.cpp

HEADERS  += dialog.h \
    mysqlmodel.h \
    mysqltablefrm.h \
    tabletypes.h \
    studentsqlmodel.h \
    studenttablefrm.h

FORMS    += dialog.ui \
    mysqltablefrm.ui
