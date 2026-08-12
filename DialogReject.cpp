#include "DialogReject.h"
#include "qdebug.h"
#include "ui_DialogReject.h"

#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QDateTime>

DialogReject::DialogReject(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogReject)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);

    m_strFile = QApplication::applicationDirPath() + "/config/rejectdata.csv";

    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderLabels(QString("厂商名称,品名/规格,物料编码,数量,退料日期,线别,是否打印").split(','));
    ui->tableView->setModel(m_model);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();

    pHeader->setSectionResizeMode(QHeaderView::Stretch);
    pHeader->setSectionResizeMode(0,QHeaderView::Fixed);
    pHeader->resizeSection(0,180);
    pHeader->setSectionResizeMode(1,QHeaderView::Fixed);
    pHeader->resizeSection(1,300);

    connect(ui->pushButtonAdd,&QPushButton::clicked,this,[=]{
        AddRow("请修改厂商名称","请修改品名/规格","100-456-9987","49",QDateTime::currentDateTime().toString("yyyy-MM-dd"),"A线");
    });

    connect(ui->pushButtonDel,&QPushButton::clicked,this,[=]{
        if(m_nCurItem == -1) return;
        m_model->removeRows(m_nCurItem,1);
        m_nCurItem = -1;
    });
    connect(ui->pushButtonClear,&QPushButton::clicked,this,[=]{
        m_model->removeRows(0,m_model->rowCount());
    });

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nCurItem = index.row();
    });

    saveLoadData(false);
    connect(m_model,&QStandardItemModel::itemChanged,this,[=](QStandardItem *item){
        Q_UNUSED(item)
        if(m_bLoading) return;
        saveLoadData(true);
        getPrintItems();
    });
}

const QString strRepl("<--#$-->");
void DialogReject::saveLoadData(bool save)
{
    QFile DFile(m_strFile);
    int count = m_model->rowCount();
    if(save)
    {
        if(!DFile.open(QIODevice::WriteOnly))
        {
            qDebug() << DFile.errorString() << m_strFile;
            return;
        }
        QTextStream out(&DFile);
        out << QString("厂商名称,品名/规格,物料编码,数量,退料日期,线别,是否打印\n").toUtf8();
        for(int i=0; i<count; i++)
        {
            QString val0 = m_model->item(i,0)->text().replace(",",strRepl).trimmed();
            QString val1 = m_model->item(i,1)->text().replace(",",strRepl).trimmed();
            QString val2 = m_model->item(i,2)->text().replace(",",strRepl).trimmed();
            QString val3 = m_model->item(i,3)->text().replace(",",strRepl).trimmed();
            QString val4 = m_model->item(i,4)->text().replace(",",strRepl).trimmed();
            QString val5 = m_model->item(i,5)->text().replace(",",strRepl).trimmed();
            QString val6 = m_model->item(i,6)->checkState() == Qt::Checked ? "Y": "N";
            QString line = QString("%1,%2,%3,%4,%5,%6,%7\n").arg(val0,val1,val2,val3,val4,val5,val6);
            out << line;
        }
    }
    else
    {
        m_model->removeRows(0,count);
        if(!DFile.open(QIODevice::ReadOnly))
        {
            qDebug() << DFile.errorString() << m_strFile;
            return;
        }
        QTextStream in(&DFile);
        qDebug() << in.readLine();
        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList vals = line.split(',');

            QString val0 = vals[0].replace(strRepl,",").trimmed();
            QString val1 = vals[1].replace(strRepl,",").trimmed();
            QString val2 = vals[2].replace(strRepl,",").trimmed();
            QString val3 = vals[3].replace(strRepl,",").trimmed();
            QString val4 = vals[4].replace(strRepl,",").trimmed();
            QString val5 = vals[5].replace(strRepl,",").trimmed();
            bool   val6  = (vals[6].replace(strRepl,",").trimmed() == "Y");

            AddRow(val0,val1,val2,val3,val4,val5,val6);
        }
    }
}

void DialogReject::AddRow(const QString&val0,const QString&val1,const QString&val2,const QString&val3,const QString&val4,const QString&val5,bool toPrint)
{
    QStandardItem *item0 = new QStandardItem(val0);
    QStandardItem *item1 = new QStandardItem(val1);
    QStandardItem *item2 = new QStandardItem(val2);
    QStandardItem *item3 = new QStandardItem(val3);
    QStandardItem *item4 = new QStandardItem(val4);
    QStandardItem *item5 = new QStandardItem(val5);
    QStandardItem *item6 = new QStandardItem("");
    item6->setEditable(false);
    item6->setCheckable(true);
    if(toPrint)
        item6->setCheckState(Qt::Checked);

    m_bLoading=true;
    QTimer::singleShot(100,this,[=]{m_bLoading=false;});
    m_model->appendRow({item0,item1,item2,item3,item4,item5,item6});
}

QList<int>DialogReject::getPrintItems()
{
    QList<int>items;
    int count = m_model->rowCount();
    for(int i=0; i<count; i++)
    {
        if(m_model->item(i,6)->checkState() == Qt::Checked)
        {
            items.push_back(i);
        }
    }
    ui->labelPrintCount->setText(QString::asprintf("打印数量: %d",items.count()));
    return items;
}

QStringList DialogReject::getItemData(int item)
{
    QString val0 = m_model->item(item,0)->text().trimmed();
    QString val1 = m_model->item(item,1)->text().trimmed();
    QString val2 = m_model->item(item,2)->text().trimmed();
    QString val3 = m_model->item(item,3)->text().trimmed();
    QString val4 = m_model->item(item,4)->text().trimmed();
    QString val5 = m_model->item(item,5)->text().trimmed();
    return QStringList{val0,val1,val2,val3,val4,val5};
}

DialogReject::~DialogReject()
{
    delete ui;
}
