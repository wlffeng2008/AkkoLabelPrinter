#include "DialogReferId.h"
#include "ui_DialogReferId.h"

#include <QSettings>

DialogReferId::DialogReferId(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogReferId)
{
    ui->setupUi(this);

    QString strCfgPath = QApplication::applicationDirPath() + "/config";
    static QSettings referSet(strCfgPath + "/referid.ini");

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("主播ID").split(','));
    ui->tableView->setModel(m_pModel);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();
    });

    QStringList strIds =  referSet.value("referids").toStringList();
    if(strIds.isEmpty()) strIds = {"4030147376716972","3352829052847833"};
    for(int i=0; i<strIds.count(); i++)
    {
        m_pModel->appendRow(new QStandardItem(strIds[i]));
    }
    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();

        QStringList strIds;
        int count = m_pModel->rowCount();
        for(int i=0; i<count; i++)
        {
            strIds.append(m_pModel->item(i,0)->text());
        }
        referSet.setValue("referids",strIds);
    });

    connect(ui->pushButtonAdd,&QPushButton::clicked,this,[=]{
        QStandardItem *item0 = new QStandardItem("id0001");
        item0->setCheckable(true);
        m_pModel->appendRow(item0);
    });

    connect(ui->pushButtonDel,&QPushButton::clicked,this,[=]{
        if(m_nSelected != -1)
        {
            m_pModel->removeRow(m_nSelected);
            m_nSelected = -1;
        }
    });

}

DialogReferId::~DialogReferId()
{
    delete ui;
}

QString DialogReferId::getIds()
{
    QString text;
    int count = m_pModel->rowCount();
    for(int i=0; i<count; i++)
    {
        text += m_pModel->item(i,0)->text();
        if(i != count-1) text += ",";
    }
    return text;
}
