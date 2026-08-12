#include "DialogLabelEdit.h"
#include "ui_DialogLabelEdit.h"

DialogLabelEdit::DialogLabelEdit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogLabelEdit)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("标签名称,x,y,w,h,Scale").split(','));
    ui->tableView->setModel(m_pModel);
}

DialogLabelEdit::~DialogLabelEdit()
{
    delete ui;
}
