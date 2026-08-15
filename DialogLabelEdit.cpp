#include "DialogLabelEdit.h"
#include "ui_DialogLabelEdit.h"

#include <QTimer>



DialogLabelEdit::DialogLabelEdit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogLabelEdit)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QString("名称,类型,X,Y,W,H,Scale,内容").split(','));
    ui->tableView->setModel(m_pModel);

    QHeaderView *pHeader = ui->tableView->horizontalHeader();
    pHeader->setSectionResizeMode(QHeaderView::Stretch);
    pHeader->setSectionResizeMode(0,QHeaderView::Fixed);
    pHeader->resizeSection(0,120);
    pHeader->setSectionResizeMode(1,QHeaderView::Fixed);
    pHeader->resizeSection(1,80);
    pHeader->setSectionResizeMode(6,QHeaderView::Fixed);
    pHeader->resizeSection(6,100);
    pHeader->setSectionResizeMode(7,QHeaderView::Fixed);
    pHeader->resizeSection(7,300);

    pDele0 = new SideValueDelegate(this);
    pDele1 = new SideValueDelegate(this);
    pDele2 = new SideValueDelegate(this);
    pDele3 = new SideValueDelegate(this);
    pDele4 = new SideValueDelegate(this);
    pDele0->setTableView(ui->tableView) ;
    pDele1->setTableView(ui->tableView) ;
    pDele2->setTableView(ui->tableView) ;
    pDele3->setTableView(ui->tableView) ;
    pDele4->setTableView(ui->tableView) ;
    pDele4->setFloatMode(true,0.1);
    ui->tableView->setItemDelegateForColumn(2,pDele0);
    ui->tableView->setItemDelegateForColumn(3,pDele1);
    ui->tableView->setItemDelegateForColumn(4,pDele2);
    ui->tableView->setItemDelegateForColumn(5,pDele3);
    ui->tableView->setItemDelegateForColumn(6,pDele4);

    connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();

        QGraphicsItem *pLabel = (QGraphicsItem *)m_pModel->item(index.row(),0)->data().toInt();
        pLabel->scene()->clearSelection();
        pLabel->setSelected(true);
    });

    connect(ui->tableView,&QTableView::doubleClicked,this,[=](const QModelIndex &index){
        m_nSelected = index.row();

        QGraphicsItem *pLabel = (QGraphicsItem *)m_pModel->item(index.row(),0)->data().toInt();
        pLabel->scene()->clearSelection();
        pLabel->setSelected(true);
        if(index.column() == 7)
        {

        }
    });

    connect(m_pModel,&QStandardItemModel::itemChanged,this,[=](QStandardItem *item){
        if(m_bLoading) return;

        int col = item->column();
        int row = item->row();
        QString strName=m_pModel->item(row,0)->text().trimmed();
        QString strText=m_pModel->item(row,7)->text().trimmed();

        QGraphicsItem *pLabel = (QGraphicsItem *)m_pModel->item(row,0)->data().toInt() ;
        auto textItem = dynamic_cast<CustomTextItem*>(pLabel);
        auto pixmapItem = dynamic_cast<CustomPixmapItem*>(pLabel);
        int imgType=0;
        if(pixmapItem)
        {
            imgType = pixmapItem->data(0).toInt();
        }

        int x = m_pModel->item(row,2)->text().toUInt();
        int y = m_pModel->item(row,3)->text().toUInt();
        int w = m_pModel->item(row,4)->text().toUInt();
        int h = m_pModel->item(row,5)->text().toUInt();
        float scale = m_pModel->item(row,6)->text().toFloat();
        if(imgType == 1) h=2;
        if(imgType == 2) w=2;

        QRect rcSet(x,y,w,h);

        switch(col)
        {
        case 0:
            if(textItem)  textItem->setName(strName);
            if(pixmapItem)pixmapItem->setName(strName);
             break;
        case 2:
        case 3:
            pLabel->setPos(x,y);
            break;

        case 4:
        case 5:
        {
            if(textItem)  textItem->setItemRect(rcSet);
            if(pixmapItem)
            {
                if(imgType==0)
                {
                    pixmapItem->setItemRect(rcSet);
                }
                else
                {
                    QImage image(QSize(w,h),QImage::Format_RGB32);
                    QPainter painter(&image);
                    painter.fillRect(image.rect(),Qt::black);
                    pixmapItem->setPixmap(QPixmap::fromImage(image));
                    pixmapItem->setScale(1);
                }
            }
        }
            break;
        case 6:
            pLabel->setScale(scale);
            break;
        case 7:
            if(textItem)  textItem->setPlainText(strText);
            break;

        }
    });

    connect(ui->pushButtonDelete,&QPushButton::clicked,[=]{
        m_pView->Delete();
    });

    connect(ui->pushButtonAddText,&QPushButton::clicked,[=]{
        QString strName = QString("text%1").arg(time(nullptr));
        m_pView->AddText("请修改文本",strName);

        LoadLabels();
    });

    connect(ui->pushButtonAddImage,&QPushButton::clicked,[=]{
        QString strName = QString("image%1").arg(time(nullptr));

        QImage image(QSize(100,100),QImage::Format_RGB32);
        QPainter painter(&image);
        painter.fillRect(image.rect(),Qt::black);
        painter.setPen(Qt::blue);
        painter.drawText(image.rect(),"请更换图片",QTextOption(Qt::AlignCenter));
        m_pView->AddImage(image,strName);

        LoadLabels();
    });
    connect(ui->pushButtonAddHLine,&QPushButton::clicked,[=]{
        QString strName = QString("HLine%1").arg(time(nullptr));
        m_pView->AddLine(true,strName);

        LoadLabels();
    });
    connect(ui->pushButtonAddVLine,&QPushButton::clicked,[=]{
        QString strName = QString("VLine%1").arg(time(nullptr));
        m_pView->AddLine(false,strName);

        LoadLabels();
    });
}

void DialogLabelEdit::BindLabelView(FrameLabelView *pView)
{
    m_pView = pView;

    connect(pView,&FrameLabelView::onItemSelected,this,[=](QGraphicsItem *item){
        int count = m_pModel->rowCount();
        for(int i=0; i<count; i++)
        {
            QGraphicsItem *pLabel = (QGraphicsItem *)m_pModel->item(i,0)->data().toInt();
            if(pLabel == item)
            {
                ui->tableView->selectRow(i);
                auto textItem = dynamic_cast<CustomTextItem*>(pLabel);
                auto pixmapItem = dynamic_cast<CustomPixmapItem*>(pLabel);
                break;
            }
        }

    });
    connect(pView,&FrameLabelView::onItemLoaded,this,[=](){
        LoadLabels();
    });
}

void DialogLabelEdit::AppendRow(QGraphicsItem *item)
{
    QString strName,strType,strX,strY,strW,strH,strScale,strData;
    strX = QString("%1").arg(item->pos().x());
    strY = QString("%1").arg(item->pos().y());
    strScale= QString("%1").arg(item->scale());
    bool bEdit  = true;
    if (auto textItem = dynamic_cast<CustomTextItem*>(item))
    {
        strName = textItem->getName();
        strType = "文本";
        strW = QString::asprintf("%d",textItem->getItemRect().toRect().width());
        strH = QString::asprintf("%d",textItem->getItemRect().toRect().height());
        strData = textItem->toPlainText();
    }

    if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
    {
        strName = pixmapItem->getName();
        strType = "图片";
        strW = QString::asprintf("%d",pixmapItem->getItemRect().toRect().width());
        strH = QString::asprintf("%d",pixmapItem->getItemRect().toRect().height());
        if(pixmapItem->data(0) == 1) strType = "水平线";
        if(pixmapItem->data(0) == 2) strType = "垂直线";
        strData = "双击切换图片";
        bEdit = false;
    }

    QStandardItem *item0 = new QStandardItem(strName);
    QStandardItem *item1 = new QStandardItem(strType);
    QStandardItem *item2 = new QStandardItem(strX);
    QStandardItem *item3 = new QStandardItem(strY);
    QStandardItem *item4 = new QStandardItem(strW);
    QStandardItem *item5 = new QStandardItem(strH);
    QStandardItem *item6 = new QStandardItem(strScale);
    QStandardItem *item7 = new QStandardItem(strData);

    item7->setEditable(bEdit);

    item0->setData((int)item,Qt::UserRole+1);
    item1->setEditable(false);

    m_pModel->appendRow({item0,item1,item2,item3,item4,item5,item6,item7});
}

void DialogLabelEdit::LoadLabels()
{
    qDebug() << "DialogLabelEdit::LoadLabels()";
    pDele0->resetText();
    pDele1->resetText();
    pDele2->resetText();
    pDele3->resetText();
    pDele4->resetText();

    m_bLoading = true;
    m_pModel->removeRows(0,m_pModel->rowCount());
    QList<QGraphicsItem *> items = this->m_pView->GetItems();
    for( int i = 0; i < items.size(); i++ )
    {
        AppendRow(items[i]);
    }
    QTimer::singleShot(100,this,[=]{ m_bLoading = false;});
}

DialogLabelEdit::~DialogLabelEdit()
{
    delete ui;
}
