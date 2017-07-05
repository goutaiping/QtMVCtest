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

    void toFirstPage();                 /* ת����ҳ */
    void toPrevPage();                  /* ת����һҳ */
    void toNextPage();                  /* ת����һҳ */
    void toLastPage();                  /* ת��βҳ */
    void toPage(const int page);    /* ת������ҳ */

    void setFields(const QString &fields)   { mQueryFields = fields; }
    void setFilter(const QString &filter)   { mQueryFilter = filter; }
    void setTable(const QString &table)     { mQueryTable = table; }
    void setOrderByFields(const QString &fields) { mQueryOrderByFields = fields; }

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
    int                             mRowCount;              /* ��ѯ��������� */
    int                             mColumnCount;       /* ��ѯ��������� */

    int                             mRstCount;              /* ��ѯ��������� */
    int                             mPageCount;             /* ��ѯ�����ҳ�� */
    int                             mMaxRowPerpage;     /* ÿҳ������� */
    int                             mCurPage;                 /* ��ǰҳ,��СΪ 0,��������ҳ�� - 1  */

    QVariantList              mHorHeaderData;       /* ˮƽ��ͷ������ */

    QString                     mQueryFilter;
    QString                     mQueryFields;
    QString                     mQueryTable;
    QString                     mQueryOrderByFields;
};

#endif // PAGEDSQLTABLEMODEL_H
