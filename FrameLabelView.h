#ifndef FRAMELABELVIEW_H
#define FRAMELABELVIEW_H

#include "CustomItems.h"
#include "qglobal.h"
#include "qjsonarray.h"
#include "qjsondocument.h"

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
    }

    bool addItem(QGraphicsItem *item)
    {
        auto itemKey = dynamic_cast<QGraphicsKeyItem*>(item);
        if(itemKey)
        {
            QString strName = itemKey->name();
            if(!strName.isEmpty())
            {
                for (auto item : items())
                {
                    if (auto kItem = dynamic_cast<QGraphicsKeyItem*>(item))
                    {
                        if(kItem->name().toUpper() == strName.toUpper())
                            return false;
                    }
                }
            }

            int hid = itemKey->hid();
            if(hid > 0)
            {
                for (auto item : items())
                {
                    if (auto kItem = dynamic_cast<QGraphicsKeyItem*>(item))
                    {
                        if(kItem->hid() == hid)
                            return false;
                    }
                }
            }

            if(hid == 233 || hid == 234 || hid == 235)
            {
                if(m_bVolRoundButton)
                {
                    int h = itemKey->h()/3*3;
                    itemKey->setSize(h/3,h);
                }
                else
                {
                    itemKey->setSize(m_nKeyW,m_nKeyH);
                }
            }
        }

        if(item->zValue() == 0)
            item->setZValue(this->items().count()+1);
        QGraphicsScene::addItem(item);
        emit itemAdded(item);

        return true;
    }

    bool isVolRoundButton(){ return m_bVolRoundButton;}
    void setVolRoundButon(bool set=true)
    {
        m_bVolRoundButton = set;
        LoadFromJson();
    }

    void removeItem(QGraphicsItem *item)
    {
        emit itemRemoved(item);
        QGraphicsScene::removeItem(item);
    }

    QGraphicsKeyItem *getKeyItem(const QString&strName)
    {
        if(!strName.isEmpty())
        {
            for (auto item : items())
            {
                if (auto Item = dynamic_cast<QGraphicsKeyItem*>(item))
                {
                    if(Item->name().trimmed().toUpper() == strName.toUpper())
                        return Item;
                }
            }
        }

        return nullptr;
    }

    CustomTextItem *getTextItem(const QString&strName)
    {
        if(!strName.isEmpty())
        {
            for (auto item : items())
            {
                if (auto textItem = dynamic_cast<CustomTextItem*>(item))
                {
                    if(textItem->m_strName.trimmed().toUpper() == strName.toUpper())
                        return textItem;
                }
            }
        }

        return nullptr;
    }

    CustomPixmapItem *getPixmapItem(const QString&strName)
    {
        if(!strName.isEmpty())
        {
            for (auto item : items())
            {
                if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
                {
                    if(pixmapItem->m_strName.trimmed().toUpper() == strName.toUpper())
                        return pixmapItem;
                }
            }
        }

        return nullptr;
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

    bool SetItemPos(const QString&strName,int x,int y)
    {
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
            pixmapItem->setPos(pt);
            return true;
        }
        return false;
    }

    bool SetItemSize(const QString&strName,int w,int h)
    {
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
            pixmapItem->setItemRect(QRectF(pt.x(),pt.y(),w,h));
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

    void DeleteItem()
    {
        QList<QGraphicsItem*>items = selectedItems();
        foreach(auto item,items)
            removeItem(item);
        emit scenceReset();
    }

    void SelectAll(bool select=true)
    {
        QList<QGraphicsItem*>items = this->items();
        foreach(auto item,items)
            item->setSelected(select);
    }

    void MoveItem(int step,int dire=0)
    {
        QList<QGraphicsItem*>items = selectedItems();
        foreach(auto item,items)
        {
            switch(dire)
            {
            case 0: item->setY(item->y() - step); break;
            case 1: item->setY(item->y() + step); break;
            case 2: item->setX(item->x() - step); break;
            case 3: item->setX(item->x() + step); break;
            }
        }
    }

    void SetValue(int value,int type)
    {
        QList<QGraphicsItem*>items = selectedItems();
        foreach(auto itemKey,items)
        {
            auto item = dynamic_cast<QGraphicsKeyItem*>(itemKey);
            if(!item) continue;
            switch(type)
            {
            case 0: item->setX(value); break;
            case 1: item->setY(value); break;
            case 2: item->setW(value); break;
            case 3: item->setH(value); break;
            }
        }
    }

    void Lock(bool lock)
    {
        m_bLocking = lock;
        QList<QGraphicsItem*>items = this->items();
        foreach(auto itemKey,items)
        {
            auto item = dynamic_cast<QGraphicsKeyItem*>(itemKey);
            if(item) item->Lock(lock);
        }
    }

    bool SetLastItemText(const QString&text)
    {
        if(m_pLastItem)
        {
            auto item = dynamic_cast<QGraphicsKeyItem*>(m_pLastItem);
            if(item)
            {
                item->setText(text);
                emit itemChanged(m_pLastItem);
                return true;
            }
        }
        return false;
    }

    bool SetLastItemHid(int hid)
    {
        if(m_pLastItem)
        {
            auto item = dynamic_cast<QGraphicsKeyItem*>(m_pLastItem);
            if(item)
            {
                item->setHid(hid);
                emit itemChanged(m_pLastItem);
                return true;
            }
        }

        return false;
    }

    QString m_strJsonFile;

    void SaveToJson(const QString&strJsonFile="")
    {
        QString strFile = strJsonFile;
        if(strFile.isEmpty())
            strFile = m_strJsonFile;

        if(strFile.isEmpty()) return;

        qDebug() << "SaveToJson" << strFile;
        QFile file(strFile);
        if(file.open( QIODevice::WriteOnly ))
        {
            QJsonArray jItems;
            for (auto item : items())
            {
                if (auto Item = dynamic_cast<QGraphicsKeyItem*>(item))
                {
                    jItems.push_back(Item->toJsonObject());
                }
            }

            QJsonObject jVer;
            jVer["validflag"]="Akko88647749";
            jVer["keycount"]=jItems.count();
            jVer["volroundbtn"]=m_bVolRoundButton;
            jVer["keyW"]=m_nKeyW;
            jVer["keyH"]=m_nKeyH;
            jVer["datetime"]=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

            QJsonObject jRoot;
            jRoot["version"] = jVer;
            jRoot["items"] = jItems;
            QJsonDocument jDoc(jRoot);
            file.write(jDoc.toJson());
            file.close();
            m_strJsonFile = strFile;
        }
    }

    static bool getJsonInfo(const QString&strJsoFile,QJsonArray &jItems,QJsonObject&jVer)
    {
        QFile file(strJsoFile);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();
            file.close();

            QJsonDocument jDoc(QJsonDocument::fromJson(data));
            if(jDoc.isObject())
            {
                QJsonObject jRoot = jDoc.object();
                if(!jRoot["version"].isNull())
                {
                    jVer   = jRoot["version"].toObject();
                    jItems = jRoot["items"].toArray();
                    return true;
                }
            }
        }
        return false;
    }

    void LoadFromJson(const QString&strJsoFile="")
    {
        SaveToJson();

        QTimer::singleShot(200,this,[=]{
            QString strFile = strJsoFile;
            if(strFile.isEmpty())
                strFile = m_strJsonFile;
            if(strFile.isEmpty()) return;

            qDebug() << "LoadFromJson" << strFile;
            QJsonArray jItems;
            QJsonObject jVer;
            if(getJsonInfo(strFile,jItems,jVer))
            {
                this->clear();
                m_bVolRoundButton = jVer["volroundbtn"].toBool();
                for(const QJsonValue &jValue : std::as_const(jItems))
                {
                    QGraphicsKeyItem *itemKey = new QGraphicsKeyItem();
                    itemKey->SetJsonObject(jValue.toObject());
                    addItem(itemKey);
                }

                m_strJsonFile = strFile;
                emit scenceReset();
            }
        });
    }

    bool isDraging(){ return (m_pLastItem != nullptr); }

signals:
    void itemSelected(QGraphicsItem *item);
    void itemChanged(QGraphicsItem *item);
    void itemAdded(QGraphicsItem *item);
    void itemRemoved(QGraphicsItem *item);
    void viewPosition(const QPoint&piont);
    void scenceReset();

protected:
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) override
    {
        if(m_pLastItem)
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
        if(!item)
        {
            clearSelection();
        }
        else
        {
            emit itemSelected(item);
            if(m_bLocking)
            {
                item->setSelected(!item->isSelected());
                event->accept();
                return;
            }
        }

        QGraphicsScene::mousePressEvent(event);
    }

    void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) override
    {
        emit viewPosition(event->scenePos().toPoint());
        if(m_bDraging && !m_pLastItem)
        {
            QRectF rcFrame(m_clkPt,event->scenePos());
            for (auto item : items())
            {
                QRectF itemSceneRect = item->mapToScene(item->boundingRect()).boundingRect();
                bool bHit = rcFrame.intersects(itemSceneRect);
                item->setSelected(bHit);
                if(bHit) emit itemSelected(item);
            }
        }

        QGraphicsScene::mouseMoveEvent(event);
    }

private:
    QPointF m_clkPt;
    bool m_bDraging  = false;
    bool m_bLocking  = false;
    bool m_bVolRoundButton = false;
    int m_nKeyW = 40;
    int m_nKeyH = 40;
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

    void resizeEvent(QResizeEvent *event) override
    {
        QGraphicsView::resizeEvent(event);
        {
            if(m_scence)
            {
                QSize LSize = this->size();

                int nW = LSize.width();
                int nH = LSize.height();
                m_scence->setSceneRect(QRectF(1,1,nW-1,nH-1));
            }
        }
    }

    CustomScene *m_scence=nullptr;
    void bindScence(CustomScene *scence)
    {
        this->setScene(scence);

        int nW = this->size().width();
        int nH = this->size().height();
        scence->setSceneRect(QRectF(1,1,nW-2,nH-2));

        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_scence = scence;
    }

    QGraphicsKeyItem *getItem(const QString&strName,bool createIfNot=false)
    {
        if(!m_scence) return nullptr;

        QString name = strName.trimmed();
        if(!name.isEmpty())
        {
            QList<QGraphicsItem*> items = m_scence->items();
            foreach (QGraphicsItem *item, items)
            {
                if(auto tmp = dynamic_cast<QGraphicsKeyItem*>(item))
                {
                    if(tmp->name() == name)
                        return tmp;
                }
            }
        }

        if(createIfNot)
        {
            QGraphicsKeyItem *item = new QGraphicsKeyItem();
            if(name.isEmpty())
                name = QString("item%1").arg(time(nullptr));
            item->setName(name);
            item->setHid(-1);
            item->setPos(100,20);
            ((CustomScene *)scene())->addItem(item);
            return item;
        }

        return nullptr;
    }

    void addText(const QString&text,const QString&strName="")
    {
        QGraphicsKeyItem *item = getItem(strName,true);
        item->setText(text);
        item->setH(50);
        item->setFontSize(24);
    }

    void addImage(const QImage&image,const QString&strName="")
    {
        QGraphicsKeyItem *item = getItem(strName,true);
        item->setImage(image);
    }

    void addLine(int len,bool horizone=true,const QString&strName="")
    {
        QGraphicsKeyItem *item = getItem(strName,true);

        item->setName(strName);
        if(horizone)
            item->setHLine(len);
        else
            item->setVLine(len);
    }

    void setItemFont(const QFont&font,bool forAll=false)
    {
        for( auto item : this->items() )
        {
            if(auto tmp = dynamic_cast<QGraphicsKeyItem*>(item))
            {
                if(forAll || tmp->isSelected())
                {
                    tmp->setFont(font);
                }
            }
        }
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

    void moveGroup(int type,float step, bool moveAll=false)
    {
        if(type < 0) return;
        if(type > 8) return;

        int nw = size().width()-4;
        int nh = size().height()-4;
        for( auto item : this->items() )
        {
            if( item->isSelected() || moveAll)
            {
                QPointF pos   = item->pos();
                qreal fscale  = item->scale();
                qreal fscaleN = item->scale();

                int ox = pos.x();
                int oy = pos.y();
                int nx = ox;
                int ny = oy;

                if(type == 2)  nx -= step;
                if(type == 3)  nx += step;

                if(type == 0)  ny -= step;
                if(type == 1)  ny += step;

                if(type == 5) fscaleN -= 0.05;
                if(type == 6) fscaleN += 0.05;
                if(type == 7) fscaleN = 1;
                if(type == 8) fscaleN = 1;

                if(nx < 0) nx = 0;
                if(ny < 0) ny = 0;
                if(nx > nw)nx = nw;
                if(ny > nh)ny = nh;

                if(nx != ox || ny != oy)
                {
                    item->setPos(nx,ny);
                    ((CustomScene *)scene())->itemChanged(item);
                    ((CustomScene *)scene())->itemSelected(item);
                }

                if(fscaleN != fscale)
                {
                    item->setScale(fscaleN);
                    ((CustomScene *)scene())->itemChanged(item);
                    ((CustomScene *)scene())->itemSelected(item);
                }
            }
        }
    }

    void Lock(bool lock)
    {
        m_bLocking=lock;
        ((CustomScene *)scene())->Lock(lock);
    }

protected:
    int m_nCols = 1;
    int m_nRows = 1;
    QPointF m_clkPt0;
    QPointF m_clkPt1;
    bool m_bDraging = false;
    bool m_bLocking  = false;

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

    void keyPressEvent(QKeyEvent *event) override
    {
        bool bCtrlPress = (event->modifiers() & Qt::ControlModifier);
        bool bShiftPress = (event->modifiers() & Qt::ShiftModifier);

        if(event->key() == Qt::Key_A && bCtrlPress)
        {
            ((CustomScene *)scene())->SelectAll(scene()->selectedItems().size() == 0);
            event->accept();
            return;
        }

        if(!m_bLocking)
        {
            if(event->key() == Qt::Key_Delete)
            {
                ((CustomScene *)scene())->DeleteItem();
                event->accept();
                return;
            }

            if(event->key() == Qt::Key_Z)
            {
                ((CustomScene *)scene())->LoadFromJson();
                event->accept();
                return;
            }

            if(event->key() == Qt::Key_S)
            {
                ((CustomScene *)scene())->SaveToJson();
                event->accept();
                return;
            }

            {
                int type = -1;

                auto nKey = event->key();
                if(nKey == Qt::Key_Left)  type=2;
                if(nKey == Qt::Key_Right) type=3;

                if(nKey == Qt::Key_Up)    type=0;
                if(nKey == Qt::Key_Down)  type=1;

                if(nKey == Qt::Key_Minus) type=5;
                if(nKey == Qt::Key_Equal) type=6;

                if(nKey == Qt::Key_Underscore)type=7;
                if(nKey == Qt::Key_Plus)      type=8;

                float step = 4;
                if(bShiftPress) step = 1;
                if(type != -1)
                    moveGroup(type,step,bCtrlPress);
            }
        }

        QGraphicsView::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent *event) override
    {
        QGraphicsView::keyReleaseEvent(event);
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

    QList<QGraphicsItem *>GetItems();

    QFont GetFont();
    void SetFont(QFont&font);

signals:
    void onItemSelected(QGraphicsItem *item);
    void onItemChanged(QGraphicsItem *item);
    void onItemAdded(QGraphicsItem *item);
    void onItemDeleted(QGraphicsItem *item);
    void onItemLoaded(int paperW,int paperH);

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
