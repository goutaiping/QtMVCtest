#ifndef INILOADER_H
#define INILOADER_H

#include <QSettings>

///
/// \brief The ConnMsg class
///
class ConnMsg
{
public:
    ConnMsg() : connPort(3306)
    {}

    QString connName;
    QString connHost;
    int connPort;
    QString connUser;
    QString connPasswd;
};

///
/// \brief The IniLoader class
///
class IniLoader
{
public:
    IniLoader();

    static void addConn(const ConnMsg &conn);
    static void removeConn(const QString &connName);
    static QList<ConnMsg> getConns();
    static bool isConnExists(const QString &connName);
    static ConnMsg getConn(const QString &connName);
    static QStringList getConnNames();
};

#endif // INILOADER_H
