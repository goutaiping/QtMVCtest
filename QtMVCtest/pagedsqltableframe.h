/**************************************************************
 *
 * 该类配合 PagedSqlTableModel 使用,
 * 可以显示 PagedSqlTableModel 模型中的查询结果信息以及分页信息,
 * 同时界面提供翻页按钮,配合 PagedSqlTableModel 中的翻页功能使用.
 *
 * *************************************************************/

#ifndef PAGEDSQLTABLEFRAME_H
#define PAGEDSQLTABLEFRAME_H

#include <QFrame>
#include <QToolBar>
#include <QAction>

class PagedSqlTableModel;

namespace Ui {
class PagedSqlTableFrame;
}

class PagedSqlTableFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PagedSqlTableFrame(QWidget *parent = 0);
    ~PagedSqlTableFrame();

public slots:
    void onBeginSearch();
    void onFirstPage();
    void onPrevPage();
    void onNextPage();
    void onLastPage();
    void onToPage(const int page);

    void setResultCountLabel(int count);
    void setPageCountLabel(int count);
    void setCurrentPageLabel(int page);

    void setModelQueryFields(const QString &fields);
    void setModelQueryFilter(const QString &filter);
    void setModelQueryTable(const QString &table);
    void setModelQueryOrderByFields(const QString &fields);
    void setModelDbName(const QString &name);
    void setModelDbHost(const QString &host);
    void setModelDbPort(const int port);
    void setModelDbUser(const QString &user);
    void setModelDbPasswd(const QString &passwd);

protected slots:
    void cleanPageButtons();
    void createPageButtons();
    void sortPageButtons(QList<QAction*> &actions);

    void onPageButton(QAction *action);

private:
    Ui::PagedSqlTableFrame *ui;

    PagedSqlTableModel *mModel;
    QToolBar    *mTbPageButtons;
    QActionGroup *mGroupPageButtons;
    QAction *mBtnFirstPage;
    QAction *mBtnPrevPage;
    QAction *mBtnNextPage;
    QAction *mBtnLastPage;
};

#endif // PAGEDSQLTABLEFRAME_H
