/**************************************************************************
 *
 *  PagedSqlTableModel ����һ������Qt Model View Control��ܵ� Model(ģ��)��.
 *  ���û��Զ������ݿ��ѯ���ĸ�����,���һ����ɵĲ�ѯ���.
 *  �������в�ѯ���,���������ҳ,ͬʱ�ṩ����ĳһҳ�Ĺ���.
 *  ���������� PagedSqlTableFrame ʹ��.
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

    // ��д
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

    void beginSearch();                 /* ��ʼ�µĲ�ѯ */
    void toFirstPage();                 /* ת����ҳ */
    void toPrevPage();                  /* ת����һҳ */
    void toNextPage();                  /* ת����һҳ */
    void toLastPage();                  /* ת��βҳ */
    void toPage(const int page);    /* ת������ҳ */

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
    mutable QSqlQuery *mQuery;                  /* ģ�͵�����Դ,�����ѯ����Ĳ�ѯ�� */

    // ��ǰҳ
    int                             mRowCount;              /* ��ѯ��������� */
    int                             mColumnCount;       /* ��ѯ��������� */

    // ��ҳ����
    int                             mRstCount;              /* ��ѯ��������� */
    int                             mPageCount;             /* ��ѯ�����ҳ�� */
    int                             mMaxRowPerpage;     /* ÿҳ������� */
    int                             mCurPage;                 /* ��ǰҳ,��СΪ 0,��������ҳ�� - 1  */

    // ��ͷ
    QVariantList              mHorHeaderData;       /* ˮƽ��ͷ������ */

    // ���ݿ��ѯ����ѯ������
    // ��Ϻ�Ĳ�ѯ���Ϊ:
    // SELECT mQueryFields FROM mQueryTable WHERE mQueryFilter LIMIT mMaxRowPerpage offset offset() order by mQueryOrderByFields.
    // ���mQueryOrderByFieldsΪ��,������order byѡ��
    QString                     mQueryFilter;           /* ��ѯ���� �������� ����,����Ϊ��*/
    QString                     mQueryFields;           /* ��ѯ���� ��ѯ�� ����,����Ϊ�� */
    QString                     mQueryTable;            /* ��ѯ���� ���� ����,����Ϊ�� */
    QString                     mQueryOrderByFields;    /* ��ѯ���� ������ ����,����Ϊ��,Ϊ��ʱ���������������� */

    // ���ݿ����Ӳ���
    QString                     mDbName;            /* ���ݿ����� */
    QString                     mDbHost;            /* ���ݿ�������ַ(IP) */
    int                             mDbPort;            /* ���ݿ�˿� */
    QString                     mDbUser;            /* ���ݿ��û��� */
    QString                     mDbPasswd;          /* ���ݿ��û����� */
};

#endif // PAGEDSQLTABLEMODEL_H
