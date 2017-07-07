/**************************************************************************
 *
 *  PagedSqlTableModel 类是一个基于Qt Model View Control框架的 Model(模型)类.
 *  由用户自定义数据库查询语句的各部分,组成一个完成的查询语句.
 *  该类运行查询语句,并将结果分页,同时提供跳到某一页的功能.
 *  该类最好配合 PagedSqlTableFrame 使用.
 *
 *
 * **************************************************************************/

#ifndef PAGEDSQLTABLEMODEL_H
#define PAGEDSQLTABLEMODEL_H

#include <QAbstractItemModel>
#include <QtSql>
#include <QTextDocument>

class PagedSqlTableModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PagedSqlTableModel(QObject *parent = 0);
    virtual ~PagedSqlTableModel();

    // 重写
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);

public slots:
    int resultCount() const     { return mRstCount; }
    int pageCount() const       { return mPageCount; }
    int maxRowPerpage() const           { return mMaxRowPerpage; }
    int currentPage() const     { return mCurPage; }
    void setMaxRowPerpage(const int count)  { mMaxRowPerpage = count; }

    void beginSearch();                 /* 开始新的查询 */
    void toFirstPage();                 /* 转到首页 */
    void toPrevPage();                  /* 转到上一页 */
    void toNextPage();                  /* 转到下一页 */
    void toLastPage();                  /* 转到尾页 */
    void toPage(const int page);    /* 转到任意页 */

    void setFields(const QString &fields)                { mQueryFields = fields; }
    void setFilter(const QString &filter)                   { mQueryFilter = filter; }
    void setTable(const QString &table)                  { mQueryTable = table; }
    void setOrderByFields(const QString &fields)    { mQueryOrderByFields = fields; }
    void setDatabaseName(const QString &name)       { mDbName = name; }
    void setDatabaseHost(const QString &host)        { mDbHost = host; }
    void setDatabasePort(const int port)                    { mDbPort = port; }
    void setDatabaseUser(const QString &user)           { mDbUser = user; }
    void setDatabasePasswd(const QString &passwd)   { mDbPasswd = passwd; }

private:
    void setResultCount(const int count);
    void setPageCount(const int count);
    void setCurrentPage(const int page);
    int offset()                      { return mCurPage * mMaxRowPerpage; }

    QSqlDatabase getMySQLConn();

    bool doSelect();


signals:
    void currentPageChanged(int page);
    void resultCountChanged(int count);
    void pageCountChanged(int count);

private:
    mutable QSqlQuery *mQuery;                  /* 模型的数据源,保存查询结果的查询器 */

    // 当前页
    int                             mRowCount;              /* 查询结果的行数 */
    int                             mColumnCount;       /* 查询结果的列数 */

    // 分页参数
    int                             mRstCount;              /* 查询结果总条数 */
    int                             mPageCount;             /* 查询结果总页数 */
    int                             mMaxRowPerpage;     /* 每页最多结果数 */
    int                             mCurPage;                 /* 当前页,最小为 0,最大等于总页数 - 1  */

    // 表头
    QVariantList              mHorHeaderData;       /* 水平表头的数据 */

    // 数据库查询器查询语句参数
    // 组合后的查询语句为:
    // SELECT mQueryFields FROM mQueryTable WHERE mQueryFilter LIMIT mMaxRowPerpage offset offset() order by mQueryOrderByFields.
    // 如果mQueryOrderByFields为空,则不是用order by选项
    QString                     mQueryFilter;           /* 查询语句的 条件过滤 部分,不能为空*/
    QString                     mQueryFields;           /* 查询语句的 查询项 部分,不能为空 */
    QString                     mQueryTable;            /* 查询语句的 表名 部分,不能为空 */
    QString                     mQueryOrderByFields;    /* 查询语句的 排序项 部分,可以为空,为空时不是用排序项排序 */

    // 数据库连接参数
    QString                     mDbName;            /* 数据库名称 */
    QString                     mDbHost;            /* 数据库主机地址(IP) */
    int                             mDbPort;            /* 数据库端口 */
    QString                     mDbUser;            /* 数据库用户名 */
    QString                     mDbPasswd;          /* 数据库用户密码 */
};

#endif // PAGEDSQLTABLEMODEL_H
