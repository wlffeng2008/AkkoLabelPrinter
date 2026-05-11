#include "DialogRecList.h"
#include "ui_DialogRecList.h"

#include <QTimer>
#include <QModelIndex>

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

        QItemSelectionModel* selection = ui->tableView->selectionModel();
        QModelIndexList rows = selection->selectedRows();

        foreach (QModelIndex index, rows) {
            m_pModel->removeRow(index.row());
        }
        emit onTextChanged();
    });

    connect(ui->pushButtonAdd,&QPushButton::clicked,this,[=]{
        appendCode(QString("%1").arg(time(nullptr)));
        QTimer::singleShot(100,this,[=]{
            QModelIndex index = ui->tableView->model()->index(m_pModel->rowCount()-1, 0);
            ui->tableView->setCurrentIndex(index);
            ui->tableView->edit(index);
        });
    });

    connect(ui->pushButtonCancel,&QPushButton::clicked,this,[=]{
        emit onTextChanged();
        hide();
    });

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();
    });
    connect(m_pModel,&QStandardItemModel::itemChanged,this,[=](QStandardItem *item){
        Q_UNUSED(item)
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
    }

    return text.trimmed();
}