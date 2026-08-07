#include "DialogFieldPickup.h"
#include "ui_DialogFieldPickup.h"

#include <QSettings>

DialogFieldPickup::DialogFieldPickup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogFieldPickup)
{
    ui->setupUi(this);

    QString strCfgPath = QApplication::applicationDirPath() + "/config";
    static QSettings fieldSet(strCfgPath + "/field.ini",QSettings::IniFormat);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("字段名称").split(','));
    ui->tableView->setModel(m_pModel);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();

        QStringList flags;
        int count = m_pModel->rowCount();
        for(int i=0; i<count; i++)
        {
            flags.append(m_pModel->item(i,0)->checkState() == Qt::Checked ? "1" : "0");
        }
        fieldSet.setValue("flags",flags);
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

    QStringList flags = fieldSet.value("flags").toStringList();
    if(flags.isEmpty()) flags = QStringList{"1","1","1","1","1","1","1"};
    for(int i=0; i<fileds.count(); i++)
    {
        QStandardItem *item0 = new QStandardItem(fileds[i]);
        item0->setCheckable(true);
        item0->setEditable(false);
        if(flags[i] == "1") item0->setCheckState(Qt::Checked);
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

