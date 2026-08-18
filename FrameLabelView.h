#ifndef FRAMELABELVIEW_H
#define FRAMELABELVIEW_H

#include "CustomItems.h"

#include <QFrame>
#include <QDebug>

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QTimer>

class CustomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent)
    {
        //connect(this, &QGraphicsScene::selectionChanged,[=]{qDebug() <<"QGraphicsScene::selectionChanged";});
    }

    void addItem(QGraphicsItem *item)
    {
        QGraphicsScene::addItem(item);
        emit itemAdded(item);
    }

    void removeItem(QGraphicsItem *item)
    {
        emit itemDeleted(item);
        QGraphicsScene::removeItem(item);
    }

    CustomTextItem *getTextItem(const QString&strName)
    {
        for (auto item : items())
        {
            if (auto textItem = dynamic_cast<CustomTextItem*>(item))
            {
                if(textItem->m_strName.trimmed().toUpper() == strName.toUpper())
                    return textItem;
            }
        }

        return nullptr;
    }

    CustomPixmapItem *getPixmapItem(const QString&strName)
    {
        for (auto item : items())
        {
            if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
            {
                if(pixmapItem->m_strName.trimmed().toUpper() == strName.toUpper())
                    return pixmapItem;
            }
        }

        return nullptr;
    }

    QStringList getAllItems()
    {
        QStringList names;
        for (auto item : items())
        {
            if (auto textItem = dynamic_cast<CustomTextItem*>(item))
                names.push_back(textItem->m_strName.trimmed());
            if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
                names.push_back(pixmapItem->m_strName.trimmed());
        }
        return names;
    }

    bool Remove(const QString&strName)
    {
        CustomTextItem *textItem = getTextItem(strName);
        if(textItem)
        {
            removeItem(textItem);
            return true;
        }

        CustomPixmapItem *pixmapItem = getPixmapItem(strName);
        if(pixmapItem)
        {
            removeItem(pixmapItem);
            return true;
        }
        return false;
    }

    bool SetItemPos(const QString&strName,int x,int y){
        CustomTextItem *textItem = getTextItem(strName);
        if(textItem)
        {
            QPoint pt = textItem->pos().toPoint();
            if(x != -1) pt.setX(x);
            if(y != -1) pt.setY(y);
            textItem->setPos(pt);
            return true;
        }

        CustomPixmapItem *pixmapItem = getPixmapItem(strName);
        if(pixmapItem)
        {
            QPoint pt = pixmapItem->pos().toPoint();
            if(x != -1) pt.setX(x);
            if(y != -1) pt.setY(y);
            textItem->setPos(pt);
            return true;
        }
        return false;
    }

    bool SetItemSize(const QString&strName,int w,int h){
        CustomTextItem *textItem = getTextItem(strName);
        if(textItem)
        {
            QPoint pt = textItem->pos().toPoint();
            textItem->setItemRect(QRectF(pt.x(),pt.y(),w,h));
            return true;
        }

        CustomPixmapItem *pixmapItem = getPixmapItem(strName);
        if(pixmapItem)
        {
            QPoint pt = pixmapItem->pos().toPoint();
            textItem->setItemRect(QRectF(pt.x(),pt.y(),w,h));
            return true;
        }
        return false;
    }

    bool SetItemScale(const QString&strName,float scale)
    {
        CustomTextItem *textItem = getTextItem(strName);
        if(textItem)
        {
            textItem->setScale(scale);
            return true;
        }

        CustomPixmapItem *pixmapItem = getPixmapItem(strName);
        if(pixmapItem)
        {
            pixmapItem->setScale(scale);
            return true;
        }

        return false;
    }

    bool GetItem(const QString&strName,int &x,int &y,int &w, int&h,float &scale)
    {
        CustomTextItem *textItem = getTextItem(strName);
        if(textItem)
        {
            scale = textItem->scale();
            QPoint pt = textItem->pos().toPoint();
            QRectF rc = textItem->getItemRect();
            x = pt.x();
            y = pt.y();
            w = rc.width();
            h = rc.height();
            return true;
        }

        CustomPixmapItem *pixmapItem = getPixmapItem(strName);
        if(pixmapItem)
        {
            scale = pixmapItem->scale();
            QPoint pt = pixmapItem->pos().toPoint();
            QRectF rc = pixmapItem->getItemRect();
            x = pt.x();
            y = pt.y();
            w = rc.width();
            h = rc.height();
            return true;
        }
        return false;
    }

    bool isDraging(){ return (m_pLastItem != nullptr); }

signals:
    void itemSelected(QGraphicsItem *item);
    void itemChanged(QGraphicsItem *item);
    void itemAdded(QGraphicsItem *item);
    void itemDeleted(QGraphicsItem *item);

protected:
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) override
    {
        if( m_pLastItem )
        {
            if(m_bDraging)
            {
                emit itemChanged(m_pLastItem);
            }
        }

        m_bDraging  = false;

        QGraphicsScene::mouseReleaseEvent(event);
    }

    void mousePressEvent( QGraphicsSceneMouseEvent *event ) override
    {
        m_bDraging = true;
        m_clkPt = event->scenePos();
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        m_pLastItem = item;
        if( !item )
        {
            clearSelection();
        }
        else
        {
            emit itemSelected(item);
        }

        QGraphicsScene::mousePressEvent(event);
    }

    void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) override
    {
        if(m_bDraging)
        {
            QRectF rcFrame(m_clkPt,event->scenePos());
            for (auto item : items())
            {
                // ✅ item局部包围盒 → 转换到scene全局坐标
                QRectF itemSceneRect = item->mapToScene(item->boundingRect()).boundingRect();

                // ✅ 矩形相交：只要有重叠就选中，标准框选
                bool bHit = rcFrame.intersects(itemSceneRect);
                item->setSelected(bHit);
            }
        }

        QGraphicsScene::mouseMoveEvent(event);
    }

private:
    QPointF m_clkPt;
    bool m_bDraging  = false;
    QGraphicsItem *m_pLastItem=nullptr;
};

class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomView(QWidget *parent = nullptr) : QGraphicsView(parent)
    {
        setBackgroundBrush(QBrush(Qt::white));
    }

    void setPrintMatrix(int nRows=1,int nCols=1)
    {
        m_nCols = nCols;
        m_nRows = nRows;
    }

    void preview()
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::NativeFormat);

        QPrintPreviewDialog previewDialog(&printer, this);
        connect(&previewDialog, &QPrintPreviewDialog::paintRequested, this, [=](QPrinter *printer) {
            this->doPrint(printer);
        });

        previewDialog.exec();
    }

    void print()
    {
        QPrinter printer(QPrinter::HighResolution);      // 使用高分辨率的 QPrinter
        printer.setOutputFormat(QPrinter::NativeFormat);  // 使用系统默认的打印机
        this->doPrint(&printer);
    }

    QPixmap toPixmap()
    {
        scene()->clearSelection();
        int nW = size().width();
        int nH = size().height();
        QPixmap pixmap(nW, nH);
        pixmap.fill(Qt::white);
        QPainter painter(&pixmap);
        scene()->render(&painter, QRectF(pixmap.rect()), this->rect());
        return pixmap;
    }

protected:
    QPointF m_clkPt0;
    QPointF m_clkPt1;
    bool m_bDraging = false;

    void mousePressEvent(QMouseEvent *event) override
    {
        QGraphicsView::mousePressEvent(event);

        if(!((CustomScene *)scene())->isDraging())
        {
            m_clkPt0 = event->pos();
            m_clkPt1 = event->pos();
            m_bDraging=true;
        }
        update();
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        QGraphicsView::mouseMoveEvent(event);

        if(m_bDraging)
        {
            m_clkPt1 = event->pos();
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QGraphicsView::mouseReleaseEvent(event);
        m_clkPt0 = event->pos();
        m_clkPt1 = event->pos();
        m_bDraging=false;
        update();

    }

    void drawForeground(QPainter *painter, const QRectF &rect) override
    {
        QGraphicsView::drawForeground(painter, rect);

        if(m_bDraging)
        {
            QRectF drawRect(m_clkPt0,m_clkPt1);
            painter->fillRect(drawRect, QColor(0,0,255,100)); //半透明蓝色
        }
    }

private:
    QPixmap grabPixmapMatrix()
    {
        int nCols = m_nCols;
        int nRows = m_nRows;
        if(nCols < 1) nCols = 1;
        if(nRows < 1) nRows = 1;

        int nW = size().width();
        int nH = size().height();
        QPixmap pixmap2(nW * nCols, nH * nRows);
        pixmap2.fill(Qt::white);

        QPainter painter2(&pixmap2);
        for(int i=0; i<nRows; i++)
        {
            for(int j=0; j<nCols; j++)
            {
                QPixmap pixmap = toPixmap();
                painter2.drawPixmap(j*nW,i*nH,nW,nH,pixmap);
            }
        }

        return pixmap2;
    }

    void doPrint(QPrinter *printer) {
        scene()->clearSelection();
        QPixmap pixmap = grabPixmapMatrix();
        if (pixmap.isNull())
            return;
        QPainter painter(printer);
        QRect rect = painter.viewport();
        QSize size = pixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);  // 保持比例缩放
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    }

    int m_nCols = 1;
    int m_nRows = 1;
};

namespace Ui {
class FrameLabelView;
}

class FrameLabelView : public QFrame
{
    Q_OBJECT
public:
    explicit FrameLabelView(QWidget *parent = nullptr);
    ~FrameLabelView();

    void Load(const QString&srtFile);
    void Save();
    void Preview();
    void Print();
    void SetPrintMatrix(int nRows=1,int nCols=1);
    void Delete();
    bool Remove(const QString&strName);
    void Clear();

    QPixmap toPixmap();

    void AddText(const QString&strText, const QString&strName);
    void AddImageFile(const QString&strFile, const QString&strName);
    void AddImageQR(const QString&strQrText, const QString&strName);
    void AddImage128(const QString&str128Text, const QString&strName);
    void*AddImage(const QImage & image, const QString&strName);
    void AddLine(bool horizontal,const QString&strName);

    void SetItemPos(const QString&strName,int x,int y);
    void SetItemScale(const QString&strName,float scale);
    void GetItem(const QString&strName,int &x,int &y,float &scale);

    void SetPaperSize(qreal width,qreal height);

    QList<QGraphicsItem *>GetItems() ;

    QFont GetFont();
    void SetFont(QFont&font);

signals:
    void onItemSelected(QGraphicsItem *item);
    void onItemChanged(QGraphicsItem *item);
    void onItemAdded(QGraphicsItem *item);
    void onItemDeleted(QGraphicsItem *item);
    void onItemLoaded(int paperW,int paperH);

protected:
    void keyPressEvent( QKeyEvent *event) override;
    void keyReleaseEvent( QKeyEvent *event) override;

private:
    Ui::FrameLabelView *ui;

    QString m_strTemlate;
    bool m_bCtrlPress = false;
    bool m_bShiftPress = false;

    int m_paperW=100;
    int m_paperH=100;
    QObject *m_pSelect  = nullptr;
    CustomView *m_pView = nullptr;
    CustomScene *m_pScene = nullptr;
};

#endif // FRAMELABELVIEW_H
