#include "DialogRecList.h"
#include "qdebug.h"
#include "qevent.h"
#include "ui_DialogRecList.h"

#include <QTimer>
#include <QModelIndex>
#include <QDateTime>
#include <QFile>

DialogRecList::DialogRecList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogRecList)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("扫码内容,时间").split(','));
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

void DialogRecList::setPath(const QString&path)
{
    QString strText = path ;
    if(!path.contains(":"))
        strText = qApp->applicationDirPath() + "/" + path;
    m_strPath = strText;
    qDebug() << m_strPath;
}

void DialogRecList::setBoxnum(const QString&boxnum)
{
    m_strboxnum = boxnum;
}

void DialogRecList::saveToFile()
{
    QString strFile = m_strPath + QString("/datalog-") + QDateTime::currentDateTime().toString("yyyyMMdd") + QString(".csv");
    QFile SF(strFile);
    if (!SF.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qWarning() << "打开失败:" << SF.errorString();
        return;
    }
    QTextStream w(&SF);

    SF.close();
}

void DialogRecList::clear()
{
    ui->pushButtonClear->click();
    emit onTextChanged();
}

void DialogRecList::appendCode(const QString&text)
{
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QStandardItem *item0 = new QStandardItem(text.trimmed());
    QStandardItem *item1 = new QStandardItem(strTime);
    item1->setEditable(false);
    m_pModel->appendRow({item0,item1});

    m_saveList.append(text + QString(",") + strTime);
    {
        QString strFile = m_strPath + QString("/datalog-") + QDateTime::currentDateTime().toString("yyyyMMdd") + QString(".csv");
        QFile SF(strFile);
        if (!SF.open(QIODevice::WriteOnly | QIODevice::Append)) {
            qWarning() << "打开失败:" << SF.errorString();
            return;
        }
        QTextStream w(&SF);


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        w.setEncoding(QStringConverter::Utf8);
#else
        w.setCodec("UTF-8");
        //w.setCodec("GBK");
#endif
        w.setGenerateByteOrderMark(true);

        if(SF.size() == 0) w <<"二维码,时间,箱号\n";
        w << text << "," << strTime << "," << m_strboxnum << "\n";
        SF.close();
    }

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