#include "DialogReferId.h"
#include "ui_DialogReferId.h"

#include <QSettings>

DialogReferId::DialogReferId(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogReferId)
{
    ui->setupUi(this);

    QString strCfgPath = QApplication::applicationDirPath() + "/config";
    static QSettings referSet(strCfgPath + "/referid.ini",QSettings::IniFormat);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("主播ID").split(','));
    ui->tableView->setModel(m_pModel);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();
    });

    QStringList strIds = referSet.value("referids").toStringList();
    QStringList strFlags = referSet.value("referflags").toStringList();
    if(strIds.isEmpty())
    {
        strIds = {"4030147376716972","3352829052847833"};
        strFlags ={"1","1"};
    }
    for(int i=0; i<strIds.count(); i++)
    {
        QStandardItem *item0 = new QStandardItem(strIds[i]);
        item0->setCheckable(true);
        if(strFlags[i] == "1")
            item0->setCheckState(Qt::Checked);
        m_pModel->appendRow(item0);
    }
    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();

        QStringList strIds;
        QStringList strFlags;
        int count = m_pModel->rowCount();
        for(int i=0; i<count; i++)
        {
            QStandardItem *item0 = m_pModel->item(i,0);
            strIds.append(item0->text());
            if(item0->checkState() == Qt::Checked)
                strFlags.append("1");
            else
                strFlags.append("0");
        }
        referSet.setValue("referids",strIds);
        referSet.setValue("referflags",strFlags);
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
        QStandardItem *item0 = m_pModel->item(i,0);
        if(item0->checkState() == Qt::Checked)
        {
            text += item0->text();
            if(i != count-1) text += ",";
        }
    }
    return text;
}
