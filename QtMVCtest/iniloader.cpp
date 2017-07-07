#include "iniloader.h"

#define INI_FILE_NAME "connections.ini"
#define KEY_CONN_HOST "Host"
#define KEY_CONN_PORT   "Port"
#define KEY_CONN_USER   "User"
#define KEY_CONN_PASSWD "Password"

static QSettings gSettings(INI_FILE_NAME, QSettings::IniFormat);

IniLoader::IniLoader()
{

}

void IniLoader::addConn(const ConnMsg &conn)
{
    // 该连接名已经存在
    if (gSettings.childGroups().contains(conn.connName))
        return;

    gSettings.beginGroup(conn.connName);
    gSettings.setValue(KEY_CONN_HOST, conn.connHost);
    gSettings.setValue(KEY_CONN_PORT, conn.connPort);
    gSettings.setValue(KEY_CONN_USER, conn.connUser);
    gSettings.setValue(KEY_CONN_PASSWD, conn.connPasswd);
    gSettings.endGroup();
}

void IniLoader::removeConn(const QString &connName)
{
    gSettings.remove(connName);
}

QList<ConnMsg> IniLoader::getConns()
{
    QList<ConnMsg> conns;
    ConnMsg conn;
    QStringList connNames = gSettings.childGroups();
    for (int i = 0; i < connNames.size(); i ++) {
        gSettings.beginGroup(connNames.at(i));
        conn.connName = connNames.at(i);
        conn.connHost = gSettings.value(KEY_CONN_HOST).toString();
        conn.connPort = gSettings.value(KEY_CONN_PORT).toInt();
        conn.connUser = gSettings.value(KEY_CONN_USER).toString();
        conn.connPasswd = gSettings.value(KEY_CONN_PASSWD).toString();
        gSettings.endGroup();

        conns.append(conn);
    }

    return conns;
}

bool IniLoader::isConnExists(const QString &connName)
{
    return gSettings.childGroups().contains(connName);
}

ConnMsg IniLoader::getConn(const QString &connName)
{
    ConnMsg conn;
    gSettings.beginGroup(connName);
    conn.connName = connName;
    conn.connHost = gSettings.value(KEY_CONN_HOST).toString();
    conn.connPort = gSettings.value(KEY_CONN_PORT).toInt();
    conn.connUser = gSettings.value(KEY_CONN_USER).toString();
    conn.connPasswd = gSettings.value(KEY_CONN_PASSWD).toString();
    gSettings.endGroup();

    return conn;
}

QStringList IniLoader::getConnNames()
{
    return gSettings.childGroups();
}

