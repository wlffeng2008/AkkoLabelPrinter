#include "DialogSell.h"
#include "ui_DialogSell.h"
#include "DialogLabelEdit.h"

#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QDateTime>
#include <QDebug>

DialogSell::DialogSell(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSell)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);

    m_strFile = QApplication::applicationDirPath() + "/config/selldata.csv";

    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderLabels(QString("厂商名称,采购订单号,品名/规格,物料编码,数量,单箱容量,生产日期,送货日期,供方质检结果").split(','));
    ui->tableView->setModel(m_model);
    QHeaderView *pHeader = ui->tableView->horizontalHeader();

    pHeader->setSectionResizeMode(QHeaderView::Stretch);
    pHeader->setSectionResizeMode(0,QHeaderView::Fixed);
    pHeader->resizeSection(0,170);
    pHeader->setSectionResizeMode(1,QHeaderView::Fixed);
    pHeader->resizeSection(1,140);
    pHeader->setSectionResizeMode(2,QHeaderView::Fixed);
    pHeader->resizeSection(2,280);
    pHeader->setSectionResizeMode(3,QHeaderView::Fixed);
    pHeader->resizeSection(3,140);
    pHeader->setSectionResizeMode(8,QHeaderView::Fixed);
    pHeader->resizeSection(8,90);

    ui->tableView->setItemDelegateForColumn(5,new SideValueDelegate(this));
    ui->tableView->setItemDelegateForColumn(4,new SideValueDelegate(this));
    ui->tableView->setItemDelegateForColumn(6,new DateTimeDelegate(this));
    ui->tableView->setItemDelegateForColumn(7,new DateTimeDelegate(this));
    ui->tableView->setItemDelegateForColumn(8,new ComboBoxDelegate(this));

    connect(ui->pushButtonAdd,&QPushButton::clicked,this,[=]{
        QString strDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
        AddRow("请修改厂商名称","请修改采购订单号","请修改品名/规格","100-456-9987","1000","100",strDate,strDate,"PASS");
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
        emit itemChanged(m_nCurItem);
    });

    saveLoadData(false);
    connect(m_model,&QStandardItemModel::itemChanged,this,[=](QStandardItem *item){
        Q_UNUSED(item)
        if(m_bLoading) return;
        saveLoadData(true);
        emit itemChanged(item->row());
    });
}

const QString strRepl("<--#$-->");
void DialogSell::saveLoadData(bool save)
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
        out << QString("厂商名称,采购订单号,品名/规格,物料编码,数量,单箱容量,生产日期,送货日期,供方质检结果\n").toUtf8();
        for(int i=0; i<count; i++)
        {
            QString val0 = m_model->item(i,0)->text().replace(",",strRepl).trimmed();
            QString val1 = m_model->item(i,1)->text().replace(",",strRepl).trimmed();
            QString val2 = m_model->item(i,2)->text().replace(",",strRepl).trimmed();
            QString val3 = m_model->item(i,3)->text().replace(",",strRepl).trimmed();
            QString val4 = m_model->item(i,4)->text().replace(",",strRepl).trimmed();
            QString val5 = m_model->item(i,5)->text().replace(",",strRepl).trimmed();
            QString val6 = m_model->item(i,6)->text().replace(",",strRepl).trimmed();
            QString val7 = m_model->item(i,7)->text().replace(",",strRepl).trimmed();
            QString val8 = m_model->item(i,8)->text().replace(",",strRepl).trimmed();
            QString line = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9\n").arg(val0,val1,val2,val3,val4,val5,val6,val7,val8);
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
            QString val6 = vals[6].replace(strRepl,",").trimmed();
            QString val7 = vals[7].replace(strRepl,",").trimmed();
            QString val8 = vals[8].replace(strRepl,",").trimmed();

            AddRow(val0,val1,val2,val3,val4,val5,val6,val7,val8);
        }
    }
}

void DialogSell::AddRow(const QString&val0, const QString&val1, const QString&val2, const QString&val3, const QString&val4, const QString&val5, const QString &val6, const QString &val7, const QString &val8)
{
    QStandardItem *item0 = new QStandardItem(val0);
    QStandardItem *item1 = new QStandardItem(val1);
    QStandardItem *item2 = new QStandardItem(val2);
    QStandardItem *item3 = new QStandardItem(val3);
    QStandardItem *item4 = new QStandardItem(val4);
    QStandardItem *item5 = new QStandardItem(val5);
    QStandardItem *item6 = new QStandardItem(val6);
    QStandardItem *item7 = new QStandardItem(val7);
    QStandardItem *item8 = new QStandardItem(val8);

    m_bLoading=true;
    QTimer::singleShot(100,this,[=]{m_bLoading=false;});
    m_model->appendRow({item0,item1,item2,item3,item4,item5,item6,item7,item8});
}

QStringList DialogSell::getItemData(int item)
{
    QString val0 = m_model->item(item,0)->text().trimmed();
    QString val1 = m_model->item(item,1)->text().trimmed();
    QString val2 = m_model->item(item,2)->text().trimmed();
    QString val3 = m_model->item(item,3)->text().trimmed();
    QString val4 = m_model->item(item,4)->text().trimmed();
    QString val5 = m_model->item(item,5)->text().trimmed();
    QString val6 = m_model->item(item,6)->text().trimmed();
    QString val7 = m_model->item(item,7)->text().trimmed();
    QString val8 = m_model->item(item,8)->text().trimmed();

    return QStringList{val0,val1,val2,val3,val4,val5,val6,val7,val8};
}

DialogSell::~DialogSell()
{
    delete ui;
}
