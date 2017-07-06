#ifndef DBCONFIGDIALOG_H
#define DBCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class DbConfigDialog;
}

class DbConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DbConfigDialog(QWidget *parent = 0);
    ~DbConfigDialog();

    QString host();
    int port();
    QString user();
    QString password();

private:
    Ui::DbConfigDialog *ui;
};

#endif // DBCONFIGDIALOG_H
