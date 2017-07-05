#include "pagedsqltableframe.h"
#include "ui_pagedsqltableframe.h"
#include "pagedsqltablemodel.h"
#include <QHBoxLayout>

PagedSqlTableFrame::PagedSqlTableFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PagedSqlTableFrame)
{
    ui->setupUi(this);

    // 初始化数据模型
    mModel = new PagedSqlTableModel(this);
    connect(mModel, SIGNAL(resultCountChanged(int)), this, SLOT(setResultCountLabel(int)));
    connect(mModel, SIGNAL(pageCountChanged(int)), this, SLOT(setPageCountLabel(int)));
    connect(mModel, SIGNAL(currentPageChanged(int)), this, SLOT(setCurrentPageLabel(int)));

    // 初始化页号按钮工具条
    QHBoxLayout *hLay = new QHBoxLayout(ui->frame_pageButtons);
    mTbPageButtons = new QToolBar(this);
    hLay->addWidget(mTbPageButtons);

    // 初始化首页, 上一页, 下一页, 尾页按钮
    mBtnFirstPage = new QAction(QString::fromLocal8Bit("首页"), this);
    mBtnPrevPage = new QAction(QString::fromLocal8Bit("上一页"), this);
    mBtnNextPage = new QAction(QString::fromLocal8Bit("下一页"), this);
    mBtnLastPage = new QAction(QString::fromLocal8Bit("尾页"), this);
    connect(mBtnFirstPage, SIGNAL(triggered()), this, SLOT(onFirstPage()));
    connect(mBtnPrevPage, SIGNAL(triggered()), this, SLOT(onPrevPage()));
    connect(mBtnNextPage, SIGNAL(triggered()), this, SLOT(onNextPage()));
    connect(mBtnLastPage, SIGNAL(triggered()), this, SLOT(onLastPage()));

    // 初始化按钮组,按钮组可以确保同一个组内的按钮同时只有一个处于checked状态,
    mGroupPageButtons = new QActionGroup(this);
    connect(mGroupPageButtons, SIGNAL(triggered(QAction*)), this, SLOT(onPageButton(QAction*)));
}

PagedSqlTableFrame::~PagedSqlTableFrame()
{
    delete ui;
}

void PagedSqlTableFrame::onFirstPage()
{
    mModel->toFirstPage();
}

void PagedSqlTableFrame::onPrevPage()
{
    mModel->toPrevPage();
}

void PagedSqlTableFrame::onNextPage()
{
    mModel->toNextPage();
}

void PagedSqlTableFrame::onLastPage()
{
    mModel->toLastPage();
}

void PagedSqlTableFrame::onToPage(const int page)
{
    mModel->toPage(page);
}

void PagedSqlTableFrame::setResultCountLabel(int count)
{
    ui->label_rstCount->setText(QString::fromLocal8Bit("总共 %1 个结果,").arg(count));
}

void PagedSqlTableFrame::setPageCountLabel(int count)
{
    ui->label_pageCount->setText(QString::fromLocal8Bit("%1 页,").arg(count));
}

void PagedSqlTableFrame::setCurrentPageLabel(int count)
{
    ui->label_rstCount->setText(QString::fromLocal8Bit("当前第 %1 页").arg(count));
}

void PagedSqlTableFrame::setModelQueryFields(const QString &fields)
{
    mModel->setFields(fields);
}

void PagedSqlTableFrame::setModelQueryFilter(const QString &filter)
{
    mModel->setFilter(filter);
}

void PagedSqlTableFrame::setModelQueryTable(const QString &table)
{
    mModel->setTable(table);
}

void PagedSqlTableFrame::setModelQueryOrderByFields(const QString &fields)
{
    mModel->setOrderByFields(fields);
}

void PagedSqlTableFrame::cleanPageButtons()
{
    mTbPageButtons->clear();
    QAction *act;
    while (mGroupPageButtons->actions().size() > 0) {
        act = mGroupPageButtons->actions().at(0);
        mGroupPageButtons->removeAction(act);
        delete act;
    }
}

void PagedSqlTableFrame::createPageButtons()
{
    // 清理上一次的按钮
    cleanPageButtons();

    // 如果总结果数为0,不显示任何页号按钮
    if (mModel->resultCount() <= 0)
        return;

    QList<QAction*> pageBtns;       // 容纳生成的页号按钮的容器
    int maxBtnCount = 11;               // 页号按钮的最大个数
    QAction *tempBtn;

    // 生成当前页的页号按钮
    tempBtn = new QAction(QString::number(mModel->currentPage() + 1), this);
    tempBtn->setCheckable(true);
    pageBtns.append(tempBtn);

    // 每次同时创建一个当前页左侧和右侧的按钮,
    // 以尽量保持当前按钮处于所有按钮的中间位置
    int left = mModel->currentPage();
    int right = mModel->currentPage();
    while (pageBtns.size() < maxBtnCount) {
        // 左侧
        -- left;
        if (left >= 0) {
            tempBtn = new QAction(QString::number(left + 1), this);
            tempBtn->setCheckable(true);
            pageBtns.append(tempBtn);
        }

        // 右侧
        ++ right;
        if (right < mModel->pageCount()) {
            tempBtn = new QAction(QString::number(right + 1), this);
            tempBtn->setCheckable(true);
            pageBtns.append(tempBtn);
        }

        // 如果当前页左侧和右侧都已经没有页号按钮可生成了,
        if ((left < 0) && (right >= mModel->pageCount()))
            break;
    }

    // 将创建的页号按钮按照按钮上的整数字符的整数值从小到大排序
    sortPageButtons(pageBtns);

    // 将页号按钮加入到按钮组中,确保同时只有一个按钮是checked状态
    for (int i = 0; i < pageBtns.size(); i ++)
        mGroupPageButtons->addAction(pageBtns.at(i));

    // 将按钮组中的所有按钮添加到工具条
    mTbPageButtons->addAction(mBtnFirstPage);
    mTbPageButtons->addAction(mBtnPrevPage);
    mTbPageButtons->addActions(mGroupPageButtons->actions());
    mTbPageButtons->addAction(mBtnNextPage);
    mTbPageButtons->addAction(mBtnLastPage);

    // 设置当前页的页号按钮为checked状态
    for (int i = 0; i < pageBtns.size(); i ++) {
        if (pageBtns.at(i)->text().toInt() == mModel->currentPage() + 1) {
            pageBtns[i]->setChecked(true);
            break;
        }
    }
}

void PagedSqlTableFrame::sortPageButtons(QList<QAction *> &actions)
{
    for (int i = 0; i < actions.size(); i ++) {
        for (int j = i + 1; j < actions.size(); j ++) {
            if (actions.at(i)->text().toInt() > actions.at(j)->text().toInt())
                actions.swap(i, j);
        }
    }
}

void PagedSqlTableFrame::onPageButton(QAction *action)
{
    onToPage(action->text().toInt() - 1);
}
