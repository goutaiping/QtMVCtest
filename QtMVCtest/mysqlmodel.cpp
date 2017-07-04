#include "mysqlmodel.h"
#include "tabletypes.h"

MySqlModel::MySqlModel(QObject *parent) :
     QAbstractItemModel(parent)
{
    mRowCount = 0;
    mColumnCount = 0;

    mResultCount = 0;
    mPageCount = 0;
    mMaxRowPerpage = 100;
    mCurrentPage = 0;

    mAdvancedKey = NULL;
    mQuery = NULL;
}

MySqlModel::~MySqlModel()
{

}

QModelIndex MySqlModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex MySqlModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int MySqlModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return mRowCount;
}

int MySqlModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return mColumnCount;
}

QVariant MySqlModel::data(const QModelIndex &index, int role) const
{
    if (!mQuery.isSelect() || !index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        mQuery.seek(index.row());
        return mQuery.value(index.column());
        break;

    default:
        return QVariant();
        break;
    }
}

QVariant MySqlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (Qt::Vertical == orientation) {
            return section;
        } else if (Qt::Horizontal == orientation) {
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

bool MySqlModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (Qt::Horizontal == orientation) {
            if (section < mHorHeaderData.size()) {
                mHorHeaderData.replace(section, value);
                return true;
            }
        }
        break;

    default:
        return false;
        break;
    }
}

void MySqlModel::setResultCount(const int count)
{
    mResultCount = count;
    emit resultCountChanged(count);
}

void MySqlModel::setPageCount(const int count)
{
    mPageCount = count;
    emit pageCountChanged(count);
}

void MySqlModel::setMaxRowPerpage(const int count)
{
    mMaxRowPerpage = count;
}

void MySqlModel::setCurrentPage(const int num)
{
    mCurrentPage = num;
    emit currentPageChanged(num);
}

QSqlDatabase MySqlModel::getDb()
{
    QString connName = "qt_table_mysql_conn";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connName))
        db = QSqlDatabase::database(connName);
    else
        db = QSqlDatabase::addDatabase(connName);
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("1234");
    db.open();
}

void MySqlModel::toNextPage()
{
    toPage(mCurrentPage + 1);
}

void MySqlModel::toPrevPage()
{
    toPage(mCurrentPage - 1);
}

void MySqlModel::toFirstPage()
{
    toPage(0);
}

void MySqlModel::toLastPage()
{
    toPage(mCurrentPage - 1);
}

void MySqlModel::toPage(const int page)
{
    if ( (page >= 0) &&
          (page != mCurrentPage) &&
          (page < mPageCount) ) {
        setCurrentPage(page);
        doSelect();
    }
}

void MySqlModel::setAdvancedKey(Table *key)
{
    mAdvancedKey = key;
}

void MySqlModel::getRowInfo(Table *key)
{

}

bool MySqlModel::doSelect()
{
    // 重置参数
    mRowCount = 0;
    mColumnCount = 0;
    mResultCount = 0;
    mPageCount = 0;
    mCurrentPage = 0;

    // 连接数据库
    QString cnctName = "qt_table_mysql_conn";
    QSqlDatabase db;
    if (QSqlDatabase::contains(cnctName))
        db = QSqlDatabase::database(cnctName);
    else
        db = QSqlDatabase::addDatabase("QMYSQL", cnctName);
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("1234");
    if (!db.open())
        return false;

    // 运行查询器
    if (!selectByAdvacedKey())
        return false;

    // 根据总结果数和每页最多显示的行数计算出总页数
    if (mResultCount <= 0)
        setPageCount(0);
    else if (0 == mResultCount % mMaxRowPerpage)
        setPageCount(mResultCount / mMaxRowPerpage);
    else
        setPageCount(mResultCount / mMaxRowPerpage + 1);

    // 获取当前页的行数和列数
    mRowCount = mQuery.size();
    mColumnCount = mQuery.record().count();

    // 获取水平表头数据
    mHorHeaderData.clear();
    for (int i = 0; i < mColumnCount; i ++) {
        mHorHeaderData.append(mQuery.record().field(i).name());
    }

    return true;
}

bool MySqlModel::selectByAdvacedKey()
{
    return true;
}

qint64 MySqlModel::getRowInfoId(const int row)
{
    return 0;
}

void MySqlModel::print(QTextDocument *doc)
{

}


