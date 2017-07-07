#include "pagedsqltablemodel.h"
#include <QDebug>

PagedSqlTableModel::PagedSqlTableModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    mQuery = NULL;

    mRowCount = 0;
    mColumnCount = 0;

    mRstCount = 0;
    mPageCount = 0;
    mMaxRowPerpage = 1000;
    mCurPage = 0;
}

PagedSqlTableModel::~PagedSqlTableModel()
{
    if (mQuery)
        delete mQuery;
}

QModelIndex PagedSqlTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex PagedSqlTableModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int PagedSqlTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return mRowCount;
}

int PagedSqlTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return mColumnCount;
}

QVariant PagedSqlTableModel::data(const QModelIndex &index, int role) const
{
    if (!mQuery)
        return QVariant();

    if ((!mQuery->isSelect()) || (!index.isValid()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
        mQuery->seek(index.row());
        return mQuery->value(index.column());
        break;

    case Qt::SizeHintRole:
        return QSize(100, 25);

    default:
        return QVariant();
        break;
    }

}

QVariant PagedSqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (Qt::Vertical == orientation) {
            return section + 1;
        } else {
            if (section < mHorHeaderData.size())
                return mHorHeaderData.at(section);
            else
                return QVariant();
        }
        break;

    default:
        return QVariant();
        break;
    }
}

bool PagedSqlTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (Qt::Horizontal == orientation) {
            if (section < mHorHeaderData.size()) {
                mHorHeaderData.replace(section, value);
                return true;
            } else
                return false;
        } else
            return false;
        break;

    default:
        return false;
        break;
    }
}

void PagedSqlTableModel::beginSearch()
{
    mCurPage = 0;
    emit currentPageChanged(mCurPage);
    doSelect();
}

void PagedSqlTableModel::toFirstPage()
{
    toPage(0);
}

void PagedSqlTableModel::toPrevPage()
{
    toPage(currentPage() - 1);
}

void PagedSqlTableModel::toNextPage()
{
    toPage(currentPage() + 1);
}

void PagedSqlTableModel::toLastPage()
{
    toPage(pageCount() - 1);
}

void PagedSqlTableModel::toPage(const int page)
{
    if ((page >= 0) && (page < pageCount())) {
        setCurrentPage(page);
        doSelect();
    }
}

void PagedSqlTableModel::setResultCount(const int count)
{
    mRstCount = count;
    emit resultCountChanged(count);
}

void PagedSqlTableModel::setPageCount(const int count)
{
    mPageCount = count;
    emit pageCountChanged(count);
}

void PagedSqlTableModel::setCurrentPage(const int page)
{
    mCurPage = page;
    emit currentPageChanged(page);
}

QSqlDatabase PagedSqlTableModel::getMySQLConn()
{
    QSqlDatabase db;
    QString connName = "qt_mysql_pagedsqltablemodel_conn";
    if (QSqlDatabase::contains(connName))
        db = QSqlDatabase::database(connName);
    else
        db = QSqlDatabase::addDatabase("QMYSQL", connName);
    db.setHostName(mDbHost);
    db.setPort(mDbPort);
    db.setUserName(mDbUser);
    db.setPassword(mDbPasswd);
    if (!db.open())
        qDebug() << db.lastError().text();
    return db;
}

bool PagedSqlTableModel::doSelect()
{
    // 重置参数
    mRowCount = 0;
    mColumnCount = 0;
    mRstCount = 0;
    mPageCount = 0;

    // 连接数据库
    QSqlDatabase db = getMySQLConn();
    if (!db.isOpen())
        return false;

    QString statement;

    // 重新生成查询器
    if (mQuery)
        delete mQuery;
    mQuery = new QSqlQuery(db);

    // 运行查询器, 获取记录总行数
    if (!mQuery->exec(QString("use %1;").arg(mDbName))) {
        qDebug() << mQuery->lastError().text();
        return false;
    }
    statement = QString("select count(1) from %1 where %2;")
            .arg(mQueryTable)
            .arg(mQueryFilter);
    if (!mQuery->exec(statement)) {
        qDebug() << mQuery->lastError().text();
        return false;
    }
    if (mQuery->next())
        setResultCount(mQuery->value(0).toInt());

    // 运行查询器, 获取当前页的记录
    if (mQueryOrderByFields.isEmpty()) {
        statement = QString("select %1 from %2 where %3 limit %4 offset %5;")
                .arg(mQueryFields)
                .arg(mQueryTable)
                .arg(mQueryFilter)
                .arg(mMaxRowPerpage)
                .arg(offset());
    } else {
        statement = QString("select %1 from %2 where %3 order by %4 limit %5 offset %6;")
                .arg(mQueryFields)
                .arg(mQueryTable)
                .arg(mQueryFilter)
                .arg(mQueryOrderByFields)
                .arg(mMaxRowPerpage)
                .arg(offset());
    }

    beginResetModel();
    if (!mQuery->exec(statement)) {
        qDebug() << mQuery->lastError().text();
        return false;
    }
    endResetModel();

    // 更新参数
    mRowCount = mQuery->size();
    mColumnCount = mQuery->record().count();
    setPageCount(mRstCount % mMaxRowPerpage == 0 ?
                     mRstCount / mMaxRowPerpage :
                     mRstCount / mMaxRowPerpage + 1);

    // 获取水平表头数据
    mHorHeaderData.clear();
    for (int i = 0; i < mQuery->record().count(); i ++)
        mHorHeaderData.append(mQuery->record().fieldName(i));

    return true;
}

