#include "DialogRecList.h"
#include "ui_DialogRecList.h"

DialogRecList::DialogRecList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogRecList)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("扫码内容").split(','));
    ui->tableView->setModel(m_pModel);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButtonClear,&QPushButton::clicked,this,[=]{
        m_pModel->setRowCount(0);
        emit onTextChanged();
    });
    connect(ui->pushButtonDel,&QPushButton::clicked,this,[=]{
        if(m_nSelected>=0)
        {
            m_pModel->removeRow(m_nSelected);
            emit onTextChanged();
            m_nSelected = -1;
        }
    });

    connect(ui->pushButtonDel,&QPushButton::clicked,this,[=]{
        emit onTextChanged();
    });

    connect(ui->pushButtonCancel,&QPushButton::clicked,this,[=]{
        emit onTextChanged();
        hide();
    });

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();
    });
    connect(m_pModel,&QStandardItemModel::itemChanged,this,[=](QStandardItem *item){
        emit onTextChanged();
    });
}

DialogRecList::~DialogRecList()
{
    delete ui;
}

void DialogRecList::clear()
{
    ui->pushButtonClear->click();
    emit onTextChanged();
}

void DialogRecList::appendCode(const QString&text)
{
    QStandardItem *item = new QStandardItem(text.trimmed());
    m_pModel->appendRow(item);
    emit onTextChanged();
}

int DialogRecList::count()
{
    return m_pModel->rowCount();
}

bool DialogRecList::contains(const QString&text)
{
    int count = m_pModel->rowCount();
    for(int i=0; i<count; i++)
    {
        if(text == m_pModel->item(i)->text())
            return true;
    }
    return false;
}

QString DialogRecList::toText(int colCount)
{
    QString text;

    int count = m_pModel->rowCount();

    for(int i=0; i<count; i++)
    {
        QString strTail = "\t" ;
        if(i%colCount || colCount == 1)
            strTail = "\n" ;
        text += m_pModel->item(i,0)->text().trimmed() + strTail;
        qDebug() << i << text ;
    }

    return text.trimmed();
}