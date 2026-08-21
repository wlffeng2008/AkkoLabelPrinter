#include "FrameLabelView.h"
#include "ui_FrameLabelView.h"

#include "CLabelSave.h"
extern "C" {
#include "zint.h"
}

#include <QDir>
#include <QPrinter>
#include <QPrinterInfo>
#include <QTimer>
#include <QRectF>
#include <QStandardPaths>
#include <QFileDialog>

static QImage genQrCodeImage(const QString&strText,const QString&strFile="/1234567Qr.bmp",int symbology=BARCODE_QRCODE,int showText=0)
{
    QImage imgBar;
    struct zint_symbol *symbol = ZBarcode_Create();
    if (symbol != NULL)
    {
        symbol->scale = 4;
        symbol->symbology = symbology;
        symbol->show_hrt = showText; //可显示信息，如果设置为1，则需要设置text值
        //symbol->input_mode = UNICODE_MODE;
        strcpy_s(symbol->outfile, strFile.toStdString().c_str());

        static char szQrText[2048]={0};
        strcpy_s(szQrText, strText.toStdString().c_str());

        int nLen = strlen(szQrText);
        int nRet = ::ZBarcode_Encode(symbol,(const unsigned char *)szQrText,nLen); //编码
        if (nRet == 0)
            nRet = ::ZBarcode_Print(symbol,0); //antate angle 旋转角度

        // qDebug() << "genQrCode" << nRet << strFile;

        ::ZBarcode_Delete(symbol);

        imgBar.load(symbol->outfile);
    }
    return imgBar;
}

static QFont s_font;

FrameLabelView::FrameLabelView(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FrameLabelView)
{
    ui->setupUi(this);
    s_font.setFamily("微软雅黑");
    s_font.setPointSize(24);

    m_pView = ui->graphicsView;
    m_pScene = new CustomScene(this);

    m_pView->setScene(m_pScene);

    m_pScene->setSceneRect(QRectF(0,0,m_pView->size().width(),m_pView->size().width()));
    m_pView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_pView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    connect( m_pScene, &CustomScene::itemSelected, this, [=](QGraphicsItem *item){
        emit onItemSelected(item);
    });
    connect( m_pScene, &CustomScene::itemChanged, this, [=](QGraphicsItem *item){
        emit onItemChanged(item);
    });
    connect( m_pScene, &CustomScene::itemAdded, this, [=](QGraphicsItem *item){
        emit onItemAdded(item);
    });
    connect( m_pScene, &CustomScene::itemDeleted, this, [=](QGraphicsItem *item){
        emit onItemDeleted(item);
    });
}

FrameLabelView::~FrameLabelView()
{
    delete ui;
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

    m_paperW = width;
    m_paperH = height;
}

void FrameLabelView::Load(const QString&srtFile)
{
    if(!m_strTemlate.isEmpty())
        Save();

    m_strTemlate = srtFile;
    if(srtFile.isEmpty())
        return;

    int paperW,paperH;
    CLabelSave::loadSceneWithImages(m_pScene,srtFile,paperW,paperH);

    SetPaperSize(paperW,paperH);

    emit onItemLoaded(m_paperW,m_paperH);
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
    CLabelSave::saveSceneWithImages(m_pScene,m_strTemlate,m_paperW,m_paperH);
}

void FrameLabelView::SetPrintMatrix(int nRows,int nCols)
{
    m_pView->setPrintMatrix(nRows,nCols);
}

void FrameLabelView::Preview()
{
    m_pView->preview();
}

void FrameLabelView::Print()
{
    m_pView->print();
}

void FrameLabelView::AddText(const QString&strText, const QString&strName)
{
    m_pScene->clearSelection();

    CustomTextItem* textItem = m_pScene->getTextItem(strName);
    if(!textItem)
    {
        textItem = new CustomTextItem(strText);
        m_pScene->addItem(textItem);
        textItem->setPos(10, 20);
        textItem->setName(strName);

        textItem->setFont(s_font);
    }
    textItem->setPlainText(strText);
}

void*FrameLabelView::AddImage(const QImage&image, const QString&strName)
{
    if(image.isNull())
        return nullptr;

    m_pScene->clearSelection();

    CustomPixmapItem* imgItem = m_pScene->getPixmapItem(strName);
    if(!imgItem)
    {
        imgItem = new CustomPixmapItem(QPixmap::fromImage(image));
        m_pScene->addItem(imgItem);
        imgItem->setPos(10, 40);
        imgItem->setName(strName);
    }
    imgItem->setPixmap(QPixmap::fromImage(image));
    return imgItem;
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
    QImage image = genQrCodeImage(strQrText,strFile);
    AddImage(image,strName);
}

void FrameLabelView::AddLine(bool horizontal,const QString&strName)
{
    QImage image(QSize(741,2),QImage::Format_RGB32);
    if(!horizontal)
        image = QImage(QSize(2,727),QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(image.rect(),Qt::black);
    CustomPixmapItem* imgItem = (CustomPixmapItem*)AddImage(image,strName);
    imgItem->setData(0, (horizontal ? 1 : 2));
    imgItem->setZValue(9999);
}

void FrameLabelView::AddImage128(const QString&strQrText, const QString&strName)
{
    QString localAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir D(localAppData);
    if(!D.exists()) D.mkdir(localAppData);
    QString strFile = localAppData + QString("/barcode2026.bmp");
    QFile::remove(strFile);
    QImage image = ::genQrCodeImage(strQrText,strFile,BARCODE_CODE128,0);
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

QList<QGraphicsItem *> FrameLabelView::GetItems()
{
    return m_pScene->items();
}

QPixmap FrameLabelView::toPixmap()
{
   return m_pView->toPixmap();
}

void FrameLabelView::Delete()
{
    QList<QGraphicsItem *>items = m_pScene->selectedItems();
    for( QGraphicsItem *item : std::as_const(items))
    {
        m_pScene->removeItem( item );
        delete item;
    }
    Save();
    emit onItemLoaded(m_paperW,m_paperH);
}


void FrameLabelView::Clear()
{
    m_pScene->clear();
    Save();
    emit onItemLoaded(m_paperW,m_paperH);
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


    int nw = size().width()-4;
    int nh = size().height()-4;

    for( auto item : m_pScene->items() )
    {
        if( item->isSelected() || bCtrlPress )
        {
            // qDebug() << "FrameLabelView::keyReleaseEvent" << m_bShiftPress << nKey << Qt::hex << nKey;
            QPointF pos   = item->pos();
            qreal fscale  = item->scale();
            qreal fscaleN = item->scale();

            int ox = pos.x();
            int oy = pos.y();
            int nx = ox;
            int ny = oy;

            int step = 4;
            if(m_bShiftPress) step = 1;

            if(nKey == Qt::Key_Left)  nx -= step;
            if(nKey == Qt::Key_Right) nx += step;

            if(nKey == Qt::Key_Up)    ny -= step;
            if(nKey == Qt::Key_Down)  ny += step;

            if(nKey == Qt::Key_Minus) fscaleN -= 0.05;
            if(nKey == Qt::Key_Equal) fscaleN += 0.05;

            if(nKey == Qt::Key_Underscore)fscaleN = 1.0;
            if(nKey == Qt::Key_Plus)      fscaleN = 1.0;

            if(nx < 0) nx = 0;
            if(ny < 0) ny = 0;
            if(nx > nw)nx = nw;
            if(ny > nh)ny = nh;

            if(fscaleN<0.05) fscaleN =  0.05;

            if(nx != ox || ny != oy)
                item->setPos(nx,ny);

            if(fscaleN != fscale)
                item->setScale(fscaleN);

            emit onItemChanged(item);
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

