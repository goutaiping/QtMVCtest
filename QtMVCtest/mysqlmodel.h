/**
 ** MySqlModel ��
 **
 **
 **
 **
 **
 **
 **
 **/

#ifndef MYSQLMODEL_H
#define MYSQLMODEL_H

#include <QAbstractItemModel>
#include <QtSql>
#include <QTextDocument>

class DBTable {
public:
    DBTable() {}
    virtual bool isValid() { return true; }
    qint64 id;
};

class Person : public DBTable {
    Person() : DBTable() {}
};

class MySqlModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MySqlModel(QObject *parent = 0);
    virtual ~MySqlModel();

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

    //
public slots:
    int resultCount() const
    { return mResultCount; }

    int pageCount() const
    { return mPageCount; }

    int maxRowPerpage() const
    { return mMaxRowPerpage; }

    int currentPage() const
    { return mCurrentPage; }

protected:
    void setResultCount(const int count);
    void setPageCount(const int count);
    void setMaxRowPerpage(const int count);
    void setCurrentPage(const int num);

    void toNextPage();          /* ��һҳ */
    void toPrevPage();          /* ��һҳ */
    void toFirstPage();         /* ��ҳ */
    void toLastPage();          /* βҳ */
    void toPage(const int page); /* ת����pageҳ */

    bool doSelect();

    // 1.��ȡ��ѯ�ܽ����,��ֵ��mResultCount
    // 2.��ҳ��ѯ,��ȡ��ǰҳ�Ľ������
    virtual bool selectByAdvacedKey() = 0;

    virtual qint64 getRowInfoId(const int row);
    virtual void print(QTextDocument *doc);

signals:
    void currentPageChanged(int page);
    void resultCountChanged(int count);
    void pageCountChanged(int count);

protected:
    mutable QSqlQuery   mQuery;                         /* �����ѯ����Ĳ�ѯ�� */
    int                             mRowCount;                      /* ��ѯ���е�ǰ������ */
    int                             mColumnCount;               /* ��ѯ���е�ǰ������ */

    QVariantList              mHorHeaderData;

    int                             mResultCount;                      /* ��ѯ����������� */
    int                             mPageCount;                 /* ��ѯ�������ҳ�� */
    int                             mMaxRowPerpage;         /* ÿҳ�����ʾ�Ĳ�ѯ����� */
    int                             mCurrentPage;                       /* ��ǰ��ʾ��ҳ�� */

};

#endif // MYSQLMODEL_H
