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

    void toFirstPage();                 /* 转到首页 */
    void toPrevPage();                  /* 转到上一页 */
    void toNextPage();                  /* 转到下一页 */
    void toLastPage();                  /* 转到尾页 */
    void toPage(const int page);    /* 转到任意页 */

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
    mutable QSqlQuery *mQuery;                  /* 模型的数据源,保存查询结果的查询器 */
    int                             mRowCount;              /* 查询结果的行数 */
    int                             mColumnCount;       /* 查询结果的列数 */

    int                             mRstCount;              /* 查询结果总条数 */
    int                             mPageCount;             /* 查询结果总页数 */
    int                             mMaxRowPerpage;     /* 每页最多结果数 */
    int                             mCurPage;                 /* 当前页,最小为 0,最大等于总页数 - 1  */

    QVariantList              mHorHeaderData;       /* 水平表头的数据 */

    QString                     mQueryFilter;
    QString                     mQueryFields;
    QString                     mQueryTable;
    QString                     mQueryOrderByFields;
};

#endif // PAGEDSQLTABLEMODEL_H
