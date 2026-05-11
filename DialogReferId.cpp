#include "DialogReferId.h"
#include "ui_DialogReferId.h"

DialogReferId::DialogReferId(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogReferId)
{
    ui->setupUi(this);
    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("主播ID").split(','));
    ui->tableView->setModel(m_pModel);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();
    });

    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();
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
