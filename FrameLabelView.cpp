#include "FrameLabelView.h"
#include "ui_FrameLabelView.h"

#include "CLabelSave.h"

#include "zint.h"

#include <QDir>
#include <QPrinter>
#include <QPrinterInfo>
#include <QTimer>
#include <QRectF>
#include <QStandardPaths>
#include <QFileDialog>

static QImage genQrCode(const std::string &strText,const std::string&strFile="/1234567Qr.bmp",int symbology=BARCODE_QRCODE,int showText=0,int nH=15)
{
    QImage imgBar;
    struct zint_symbol *symbol = ZBarcode_Create();
    if (symbol != NULL)
    {
        symbol->scale = 4;
        symbol->symbology = symbology;
        symbol->show_hrt = showText; //可显示信息，如果设置为1，则需要设置text值
        //symbol->input_mode = UNICODE_MODE;
        strcpy_s(symbol->outfile, strFile.c_str());

        const char *lpszText = strText.c_str();
        int nLen = strlen(lpszText);
        int nRet = ::ZBarcode_Encode(symbol,(const unsigned char *)lpszText,nLen); //编码
        if (nRet == 0)
            nRet = ::ZBarcode_Print(symbol,0); //antate angle 旋转角度

        qDebug() << "genQrCode" << nRet << strFile;

        ::ZBarcode_Delete(symbol);

        imgBar.load(strFile.c_str());
    }
    return imgBar;
}

static QFont s_font;

FrameLabelView::FrameLabelView(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FrameLabelView)
{
    ui->setupUi(this);
    s_font.setFamily("方正兰亭黒_GBK");
    s_font.setPointSize(9);

    m_pView = ui->graphicsView;

    m_pScene = new CustomScene(this);

    m_pView->setScene(m_pScene);
    m_pScene->setView(m_pView);

    m_pScene->setSceneRect(QRectF(0,0,m_pView->size().width(),m_pView->size().width()));
    m_pView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_pView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    connect( m_pScene, &CustomScene::itemtemSelected, this, [=](){
        //emit onItemSelected(nullptr);
        qDebug() << "itemtemSelected";
    });
}

FrameLabelView::~FrameLabelView()
{
    delete ui;
}

void FrameLabelView::resizeEvent(QResizeEvent *event)
{
    // SetPaperSize(600,600);

    QFrame::resizeEvent(event);
}

void FrameLabelView::SetPaperSize(qreal width, qreal height)
{
    QSize LSize = this->size();

    int nW = LSize.width();
    int nH = LSize.height();
    qreal ratio = width/height;

    if(ratio > 1)
    {
        nH = nW / ratio;
    }

    if(ratio < 1)
    {
        nW = nH * ratio;
    }

    ui->graphicsView->setFixedSize(nW-1,nH-1);
    m_pScene->setSceneRect(QRectF(0,0,nW-1,nH-1));
}

QFont FrameLabelView::GetFont()
{
    if(!m_bCtrlPress)
    {
        for (auto item : m_pScene->items())
        {
            if (auto textItem = dynamic_cast<CustomTextItem*>(item))
            {
                if(textItem->isSelected())
                   return textItem->font();
            }
        }
    }
    return s_font;
}

void FrameLabelView::SetFont(QFont&font)
{
    if(m_bCtrlPress)
        s_font = font;

    for (auto item : m_pScene->items())
    {
        if (auto textItem = dynamic_cast<CustomTextItem*>(item))
        {
            if(textItem->isSelected() || m_bCtrlPress)
                textItem->setFont(font);
        }
    }
}

void FrameLabelView::Load(const QString&srtFile)
{
    m_strTemlate = srtFile;
    if(srtFile.isEmpty())
        return;

    emit onItemSelected(nullptr);

    CLabelSave::loadSceneWithImages(m_pScene,srtFile);

    for (auto item : m_pScene->items())
    {
        if (auto textItem = dynamic_cast<CustomTextItem*>(item))
        {
            connect( textItem, &CustomTextItem::sigRectChanged_, [this](QObject *pSender){
                emit onItemSelected(pSender);
            } );
        }
        else if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
        {
            connect( pixmapItem, &CustomPixmapItem::sigRectChanged_, [this](QObject *pSender){
                emit onItemSelected(pSender);
            } );
        }
    }
    emit onItemLoaded();
}

void FrameLabelView::Save()
{
    if(m_strTemlate.isEmpty())
    {
        QString strFile = QFileDialog::getSaveFileName(this, "保存模板文件", QApplication::applicationDirPath() + "/config", "Template Files (*.tem);;All Files (*.*)");

        if(strFile.isEmpty())
            return;

        m_strTemlate = strFile;
        setWindowTitle("标签打印模板 - " + strFile);
    }
    CLabelSave::saveSceneWithImages(m_pScene,m_strTemlate);
}

void FrameLabelView::Preview()
{
    QRectF rc(0,0,m_pView->size().width(),m_pView->size().height());
    m_pView->printPreviewAndPrint(rc);
}

void FrameLabelView::Print()
{
    QRectF rc(0,0,m_pView->size().width(),m_pView->size().height());
    m_pView->print(rc);
}

void FrameLabelView::AddText(const QString&strText, const QString&strName)
{
    CustomTextItem* textItem = m_pScene->getTextItem(strName);
    if(!textItem)
    {
        textItem = new CustomTextItem(strText);
        m_pScene->addItem(textItem);
        textItem->setPos(10, 20);
        textItem->setName(strName);

        textItem->setFont(s_font);

        connect(textItem, &CustomTextItem::sigRectChanged_, this, [=](QObject *pSender){
            emit onItemSelected(pSender);
        } );
    }
    textItem->setPlainText(strText);
}

void FrameLabelView::AddImage(const QImage & image, const QString&strName)
{
    if(image.isNull())
        return ;
    CustomPixmapItem* imgItem = m_pScene->getPixmapItem(strName);
    if(!imgItem)
    {
        imgItem = new CustomPixmapItem();
        m_pScene->addItem(imgItem);
        imgItem->setPos(10, 40);
        imgItem->setName(strName);

        connect(imgItem, &CustomPixmapItem::sigRectChanged_, this, [=](QObject *pSender){
            emit onItemSelected(pSender);
        } );
    }
    imgItem->setPixmap(QPixmap::fromImage(image));
}

void FrameLabelView::AddImageFile(const QString&strFile, const QString&strName)
{
    QImage image = QImage(strFile);
    AddImage(image,strName);
}

void FrameLabelView::AddImageQR(const QString&strQrText, const QString&strName)
{    
    QString localAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir D(localAppData);
    if(!D.exists()) D.mkdir(localAppData);
    QString strFile = localAppData + QString("/qrcode2026.bmp");
    QFile::remove(strFile);
    QImage image = genQrCode(strQrText.toStdString().c_str(),strFile.toStdString().c_str());
    AddImage(image,strName);
}

void FrameLabelView::AddLine(bool horizontal,const QString&strName)
{
    //CustomPixmapItem* imgItem = m_pScene->getPixmapItem(strName);
    //if(!imgItem)
    //    return;

    QImage image(QSize(1000,4),QImage::Format_RGB32);
    if(!horizontal)
        image = QImage(QSize(4,1000),QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(image.rect(),Qt::black);
    AddImage(image,strName);
}

void FrameLabelView::AddImage128(const QString&strQrText, const QString&strName)
{
    QString localAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir D(localAppData);
    if(!D.exists()) D.mkdir(localAppData);
    QString strFile = localAppData + QString("/barcode2026.bmp");
    QFile::remove(strFile);
    QImage image = genQrCode(strQrText.toStdString().c_str(),strFile.toStdString().c_str(),BARCODE_CODE128,0);
    QRect rcCut(0,0,image.size().width(),image.size().height()*0.3);
    AddImage(image.copy(rcCut),strName);
}


void FrameLabelView::SetItemPos(const QString&strName,int x,int y)
{
    m_pScene->SetItemPos(strName, x, y);
}

void FrameLabelView::SetItemScale(const QString&strName,float scale)
{
    m_pScene->SetItemScale(strName,scale);
}

void FrameLabelView::Delete()
{
    QList<QGraphicsItem *> items = m_pScene->selectedItems();
    for( QGraphicsItem *item : std::as_const(items))
    {
        m_pScene->removeItem( item );
        delete item;
    }
    Save();
}

bool FrameLabelView::Remove(const QString&strName)
{
    return m_pScene->Remove(strName);
}

void FrameLabelView::keyReleaseEvent(QKeyEvent *event)
{
    auto nKey = event->key();

    bool bCtrlPress = (event->modifiers() & Qt::ControlModifier);
    m_bCtrlPress = bCtrlPress;
    m_bShiftPress = (event->modifiers() & Qt::ShiftModifier);

    for( auto item : m_pScene->items() )
    {
        if( item->isSelected() || bCtrlPress)
        {
            QPointF pos = item->pos();
            qreal fscale = item->scale();
            qreal fscaleN = item->scale();

            int x = pos.x();
            int y = pos.y();
            int nx = x;
            int ny = y;

            int step = 5;
            if(m_bShiftPress)
                step = 1;

            if(nKey == Qt::Key_Left)
                nx -= step;
            if(nKey == Qt::Key_Right)
                nx += step;

            if(nKey == Qt::Key_Up)
                ny -= step;
            if(nKey  == Qt::Key_Down)
                ny += step;

            if(nKey == Qt::Key_Minus)
            {
                fscaleN *= 0.95;
                if(m_bShiftPress) fscaleN = 1.0;
            }

            if(nKey == Qt::Key_Equal)
            {
                fscaleN *= 1.05;
                if(m_bShiftPress) fscaleN = 1.0;
            }

            if(nx<0) nx = 0;
            if(ny<0) ny = 0;

            if(nx != x || ny != y)
                item->setPos(nx,ny);

            if(fscaleN != fscale)
                item->setScale(fscaleN);
        }
    }

    QFrame::keyReleaseEvent(event);
}

void FrameLabelView::keyPressEvent(QKeyEvent *event)
{
    auto nKey = event->key();

    if (nKey == Qt::Key_Delete)
    {
        Delete();
    }

    if(event->modifiers() == Qt::ControlModifier)
    {
        m_bCtrlPress = true;
        if(event->key() == Qt::Key_S)
            this->Save();
    }
    if(event->modifiers() == Qt::ShiftModifier)
    {
        m_bShiftPress = true;
    }

    QFrame::keyPressEvent(event);
}

