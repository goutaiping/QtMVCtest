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
    pagedsqltablemodel.cpp \
    pagedsqltableframe.cpp \
    dbconfigdialog.cpp \
    iniloader.cpp

HEADERS  += dialog.h \
    pagedsqltablemodel.h \
    pagedsqltableframe.h \
    dbconfigdialog.h \
    iniloader.h

FORMS    += dialog.ui \
    pagedsqltableframe.ui \
    dbconfigdialog.ui
