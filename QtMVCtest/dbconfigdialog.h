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

    QString connName();
    QString host();
    int port();
    QString user();
    QString password();

    void setConnName(const QString &name);
    void setHost(const QString &host);
    void setPort(const int port);
    void setUser(const QString &user);
    void setPassword(const QString &passwd);

private:
    Ui::DbConfigDialog *ui;
};

#endif // DBCONFIGDIALOG_H
