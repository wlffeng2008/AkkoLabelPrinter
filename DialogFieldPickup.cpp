#include "DialogFieldPickup.h"
#include "ui_DialogFieldPickup.h"

DialogFieldPickup::DialogFieldPickup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogFieldPickup)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("字段名称").split(','));
    ui->tableView->setModel(m_pModel);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();
    });

    QStringList fileds=
    {
        "店铺名称",
        "产品名称",
        "产品属性",
        "收货地址",
        "订单日期",
        "支付日期"
    };

    for(int i=0; i<fileds.count(); i++)
    {
        QStandardItem *item0 = new QStandardItem(fileds[i]);
        item0->setCheckable(true);
        item0->setEditable(false);
        item0->setCheckState(Qt::Checked);
        m_pModel->appendRow(item0);
    }
}

DialogFieldPickup::~DialogFieldPickup()
{
    delete ui;
}

bool DialogFieldPickup::getChecked(int index)
{
    QStandardItem *item0 = m_pModel->item(index);
    if(item0)
        return (item0->checkState() == Qt::Checked);
    return false;
}

