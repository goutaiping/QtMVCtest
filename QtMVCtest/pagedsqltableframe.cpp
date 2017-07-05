#include "pagedsqltableframe.h"
#include "ui_pagedsqltableframe.h"
#include "pagedsqltablemodel.h"
#include <QHBoxLayout>

PagedSqlTableFrame::PagedSqlTableFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PagedSqlTableFrame)
{
    ui->setupUi(this);

    // ��ʼ������ģ��
    mModel = new PagedSqlTableModel(this);
    connect(mModel, SIGNAL(resultCountChanged(int)), this, SLOT(setResultCountLabel(int)));
    connect(mModel, SIGNAL(pageCountChanged(int)), this, SLOT(setPageCountLabel(int)));
    connect(mModel, SIGNAL(currentPageChanged(int)), this, SLOT(setCurrentPageLabel(int)));

    // ��ʼ��ҳ�Ű�ť������
    QHBoxLayout *hLay = new QHBoxLayout(ui->frame_pageButtons);
    mTbPageButtons = new QToolBar(this);
    hLay->addWidget(mTbPageButtons);

    // ��ʼ����ҳ, ��һҳ, ��һҳ, βҳ��ť
    mBtnFirstPage = new QAction(QString::fromLocal8Bit("��ҳ"), this);
    mBtnPrevPage = new QAction(QString::fromLocal8Bit("��һҳ"), this);
    mBtnNextPage = new QAction(QString::fromLocal8Bit("��һҳ"), this);
    mBtnLastPage = new QAction(QString::fromLocal8Bit("βҳ"), this);
    connect(mBtnFirstPage, SIGNAL(triggered()), this, SLOT(onFirstPage()));
    connect(mBtnPrevPage, SIGNAL(triggered()), this, SLOT(onPrevPage()));
    connect(mBtnNextPage, SIGNAL(triggered()), this, SLOT(onNextPage()));
    connect(mBtnLastPage, SIGNAL(triggered()), this, SLOT(onLastPage()));

    // ��ʼ����ť��,��ť�����ȷ��ͬһ�����ڵİ�ťͬʱֻ��һ������checked״̬,
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
    ui->label_rstCount->setText(QString::fromLocal8Bit("�ܹ� %1 �����,").arg(count));
}

void PagedSqlTableFrame::setPageCountLabel(int count)
{
    ui->label_pageCount->setText(QString::fromLocal8Bit("%1 ҳ,").arg(count));
}

void PagedSqlTableFrame::setCurrentPageLabel(int count)
{
    ui->label_rstCount->setText(QString::fromLocal8Bit("��ǰ�� %1 ҳ").arg(count));
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
    // ������һ�εİ�ť
    cleanPageButtons();

    // ����ܽ����Ϊ0,����ʾ�κ�ҳ�Ű�ť
    if (mModel->resultCount() <= 0)
        return;

    QList<QAction*> pageBtns;       // �������ɵ�ҳ�Ű�ť������
    int maxBtnCount = 11;               // ҳ�Ű�ť��������
    QAction *tempBtn;

    // ���ɵ�ǰҳ��ҳ�Ű�ť
    tempBtn = new QAction(QString::number(mModel->currentPage() + 1), this);
    tempBtn->setCheckable(true);
    pageBtns.append(tempBtn);

    // ÿ��ͬʱ����һ����ǰҳ�����Ҳ�İ�ť,
    // �Ծ������ֵ�ǰ��ť�������а�ť���м�λ��
    int left = mModel->currentPage();
    int right = mModel->currentPage();
    while (pageBtns.size() < maxBtnCount) {
        // ���
        -- left;
        if (left >= 0) {
            tempBtn = new QAction(QString::number(left + 1), this);
            tempBtn->setCheckable(true);
            pageBtns.append(tempBtn);
        }

        // �Ҳ�
        ++ right;
        if (right < mModel->pageCount()) {
            tempBtn = new QAction(QString::number(right + 1), this);
            tempBtn->setCheckable(true);
            pageBtns.append(tempBtn);
        }

        // �����ǰҳ�����Ҳ඼�Ѿ�û��ҳ�Ű�ť��������,
        if ((left < 0) && (right >= mModel->pageCount()))
            break;
    }

    // ��������ҳ�Ű�ť���հ�ť�ϵ������ַ�������ֵ��С��������
    sortPageButtons(pageBtns);

    // ��ҳ�Ű�ť���뵽��ť����,ȷ��ͬʱֻ��һ����ť��checked״̬
    for (int i = 0; i < pageBtns.size(); i ++)
        mGroupPageButtons->addAction(pageBtns.at(i));

    // ����ť���е����а�ť��ӵ�������
    mTbPageButtons->addAction(mBtnFirstPage);
    mTbPageButtons->addAction(mBtnPrevPage);
    mTbPageButtons->addActions(mGroupPageButtons->actions());
    mTbPageButtons->addAction(mBtnNextPage);
    mTbPageButtons->addAction(mBtnLastPage);

    // ���õ�ǰҳ��ҳ�Ű�ťΪchecked״̬
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
