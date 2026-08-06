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

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent)
    {
        connect(this, &QGraphicsScene::selectionChanged, [=](){
            QList<QGraphicsItem *> items = this->selectedItems();
            for( int i = 0; i < items.size(); i++ )
            {
                if( m_pLastItem != items[i] )
                    items[i]->setSelected(false);
            }
        });
    }

    void setView(QGraphicsView *pView) { pView_ = pView ;}

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
            if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
            {
                names.push_back(pixmapItem->m_strName.trimmed());
            }
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
            textItem->setItemRect(QRectF(pt.x(),pt.y(),w,h));;
            return true;
        }
        return false;
    }

    bool SetItemScale(const QString&strName,float scale){
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

    bool GetItem(const QString&strName,int &x,int &y,int &w, int&h,float &scale){
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

signals:
    void itemtemSelected();

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event ) override
    {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if( !item )
        {
            clearSelection();
            emit itemtemSelected();
            pView_->viewport()->repaint();
        }
        m_pLastItem = item ;
        QGraphicsScene::mousePressEvent(event);
    }
private:
    QGraphicsView *pView_ = nullptr;
    QGraphicsItem *m_pLastItem=nullptr;
};

class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomView(QWidget *parent = nullptr) : QGraphicsView(parent) {
        setBackgroundBrush(QBrush(Qt::white));
    }
    void setPrintMatrix(int nRows=1,int nCols=1){m_nCols = nCols; m_nRows = nRows; };

    QPixmap grabSpecifiedArea(const QRectF &area)
    {
        if (area.isEmpty())
            return QPixmap();

        int nW = area.width();
        int nH = area.height();

        QPixmap pixmap(nW, nH);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        scene()->render(&painter, QRectF(pixmap.rect()), area);  // 将场景的指定区域绘制到 QPixmap

        int nCols = m_nCols;
        int nRows = m_nRows;

        if(nCols <= 0 || nRows <= 0)
            return pixmap;

        QPixmap pixmap2(nW * nCols, nH * nRows);
        pixmap2.fill(Qt::white);

        QPainter painter2(&pixmap2);
        for(int i=0; i<nRows; i++)
        {
            for(int j=0; j<nCols; j++)
            {
                painter2.drawPixmap(j*nW,i*nH,nW,nH,pixmap);
            }
        }

        return pixmap2;
    }

    void printPreviewAndPrint(const QRectF &area) {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::NativeFormat);

        QPrintPreviewDialog previewDialog(&printer, this);

        connect(&previewDialog, &QPrintPreviewDialog::paintRequested, this, [=](QPrinter *printer) {
            this->printArea(printer, area);
        });

        previewDialog.exec();
    }

    void print(const QRectF &area) {
        QPrinter printer(QPrinter::HighResolution);       // 使用高分辨率的 QPrinter
        printer.setOutputFormat(QPrinter::NativeFormat);  // 使用系统默认的打印机
        printArea(&printer, area);
    }

private:
    void printArea(QPrinter *printer, const QRectF &area) {
        scene()->clearSelection();
        QPixmap pixmap = grabSpecifiedArea(area);
        if (!pixmap.isNull())
        {
            QPainter painter(printer);
            QRect rect = painter.viewport();
            QSize size = pixmap.size();
            size.scale(rect.size(), Qt::KeepAspectRatio);  // 保持比例缩放
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(pixmap.rect());
            painter.drawPixmap(0, 0, pixmap);
        }
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

    void AddText(const QString&strText, const QString&strName);
    void AddImageFile(const QString&strFile, const QString&strName);
    void AddImageQR(const QString&strQrText, const QString&strName);
    void AddImage128(const QString&str128Text, const QString&strName);
    void AddImage(const QImage & image, const QString&strName);
    void AddLine(bool horizontal,const QString&strName);

    void SetItemPos(const QString&strName,int x,int y);
    void SetItemScale(const QString&strName,float scale);
    void GetItem(const QString&strName,int &x,int &y,float &scale);

    void SetPaperSize(qreal width,qreal height);

    QFont GetFont();
    void SetFont(QFont&font);

signals:
    void onItemSelected(QObject *pSender);
    void onItemLoaded();

protected:
    void keyPressEvent( QKeyEvent *event) override;
    void keyReleaseEvent( QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::FrameLabelView *ui;

    QString m_strTemlate;
    bool m_bCtrlPress = false;
    bool m_bShiftPress = false;

    QObject *m_pSelect  = nullptr;
    CustomView *m_pView = nullptr;
    CustomScene *m_pScene = nullptr;
};

#endif // FRAMELABELVIEW_H
