#include "DialogReferId.h"
#include "ui_DialogReferId.h"

DialogReferId::DialogReferId(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogReferId)
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
}

DialogReferId::~DialogReferId()
{
    delete ui;
}
