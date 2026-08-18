#ifndef CUSTOMITEMS_H
#define CUSTOMITEMS_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QCursor>
#include <QTextCursor>
#include <QObject>
#include <QDebug>
#include <QPainter>
#include <QRgba64>
#include <QWidget>

class CustomBaseItem : public QObject
{
    Q_OBJECT

public:
    explicit CustomBaseItem(QGraphicsItem *itemIn) : m_item(itemIn)
    {
        if(m_item)
        {
            m_item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsFocusable);
            m_item->setAcceptHoverEvents(true);
        }
    }

    QRectF getItemRect()
    {
        QRectF rect = m_item->boundingRect();

        qreal scale = m_item->scale();
        if(scale <= 0.05)
            scale = 1;

        if(m_item->data(0).toInt() == 1 || m_item->data(0).toInt() == 2)
            scale = 1;

        //rect = QRectF(m_item->pos().x(), m_item->pos().y(), rect.width()*scale, rect.height()*scale);
        return rect;
    };

    void setItemRect(const QRectF &rectNew)
    {
        QRectF rect_last = getItemRect();
        int w_last = rect_last.width();
        int h_last = rect_last.height();
        int w_new = rectNew.width();
        int h_new = rectNew.height();

        qreal w_scale_new = w_new / w_last;
        qreal h_scale_new = h_new / h_last;
        qreal newScale = qMax( w_scale_new, h_scale_new);

        if(m_item->data(0).toInt() == 1 || m_item->data(0).toInt() == 2)
            newScale = 1;

        m_item->setScale(newScale);
        m_item->setPos(rectNew.topLeft());

        emitRectSig();
    }

    void handleMousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        QRectF rect = m_item->boundingRect();
        QPointF pos = event->pos();
        const qreal edgeThreshold = 2.0;

        if(!resizing || !moving)
        {
            initialScenePos = event->scenePos();
            initialRect     = getItemRect();
        }

        if(isNearEdge(pos, rect, edgeThreshold))
        {
            resizing = true;
        }
        else
        {
            moving = true;
            m_item->setCursor(Qt::SizeAllCursor);
        }
        event->accept();
    }

    void handleMouseMoveEvent( QGraphicsSceneMouseEvent *event )
    {
        if( resizing )
        {
            QPointF delta = event->scenePos() - initialScenePos;
            qreal newWidth = initialRect.width() + delta.x();
            qreal newHeight = initialRect.height() + delta.y();
            m_item->setScale(qMax( newWidth / initialRect.width(), newHeight / initialRect.height()));  // Scaling logic
        }
        else if( moving )
        {
            m_item->setPos(m_item->pos() + (event->scenePos() - initialScenePos));  // Move item
            initialScenePos = event->scenePos();
            event->accept();
        }
        event->accept();

        //emitRectSig();
    }

    void handleMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        m_item->setCursor(Qt::ArrowCursor);  // Reset cursor
        m_item->setSelected(!m_item->isSelected());
        m_item->setFocus();
        moving = false;
        resizing = false;
        event->accept();

        emitRectSig();
    }

    void handleHoverMoveEvent( QGraphicsSceneHoverEvent *event )
    {
        QRectF rect = m_item->boundingRect();
        QPointF pos = event->pos();
        const qreal edgeThreshold = 2.0;

        if (isNearEdge(pos, rect, edgeThreshold))
        {
            m_item->setCursor(Qt::SizeFDiagCursor);  // Change cursor to resizing
        }
        else
        {
            m_item->setCursor(Qt::PointingHandCursor);  // Normal cursor
        }
    }


signals:
    void itemChanged();

private:
    QGraphicsItem *m_item=nullptr;
    bool resizing=false;
    bool moving=false;
    QPointF initialScenePos;
    QRectF initialRect;

    bool isNearEdge(const QPointF &pos, const QRectF &rect, qreal threshold)
    {
        return ( qAbs(pos.x() - rect.right())  < threshold ||
                 qAbs(pos.y() - rect.bottom()) < threshold );
    }
    void emitRectSig()
    {
        emit itemChanged();
    }
};


class CustomTextItem : public QGraphicsTextItem
{
     Q_OBJECT
signals:
    void sigRectChanged();
public:
    CustomTextItem(const QString &text, QGraphicsTextItem *parentGraph = nullptr)
        : QGraphicsTextItem(text, parentGraph), base(this)
    {
        connect( &base, &CustomBaseItem::itemChanged, [=](){
            emit sigRectChanged();
        } );
    }

    QString m_strName;
    bool m_bShowRect = false;
    void setName(const QString&strName) { m_strName = strName; }
    QString getName(){ return m_strName; }

    void setItemRect(const QRectF&rectNew ){ base.setItemRect(rectNew); }
    QRectF getItemRect(){  return base.getItemRect(); }

protected:
    CustomBaseItem base;  // Instance of Resizable to handle resizing

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        base.handleMousePressEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        base.handleMouseMoveEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        base.handleMouseReleaseEvent(event);
        emit sigRectChanged();
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override {
        base.handleHoverMoveEvent(event);
    }

    QRectF boundingRect() const override {
        return QGraphicsTextItem::boundingRect();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {

        QPen pen = QPen(Qt::red,2,Qt::DashLine);
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(pen);
        QGraphicsTextItem::paint(painter, option, widget);

        if(m_bShowRect)
        {
            QPen pen = QPen(Qt::black,2);
            painter->setPen(pen);
            QRectF rect =  boundingRect();
            rect.adjust(2,2,-2,-2);
            painter->drawRect(rect);
        }
    }

    QTextCursor cursor = textCursor();
    // 捕获双击事件
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        // 启用编辑模式，允许用户编辑文本
        setTextInteractionFlags(Qt::TextEditorInteraction);

        // 将光标移动到双击的位置
        cursor.clearSelection();  // 清除任何选中状态
        setTextCursor(cursor);

        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }

    // 捕获失去焦点事件，当文本框失去焦点时，禁用编辑模式
    void focusOutEvent( QFocusEvent *event ) override {
        cursor.clearSelection();

        // 禁用编辑模式，防止文本随时被修改
        setTextInteractionFlags(Qt::NoTextInteraction);
        unsetCursor();
        QGraphicsTextItem::focusOutEvent(event);
    }
};


class CustomPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

signals:
    void sigRectChanged();

public:
    CustomPixmapItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr)
        : QObject(), QGraphicsPixmapItem( pixmap, parent ), base(this)
    {
        connect( &base, &CustomBaseItem::itemChanged, this, [=](){
            emit sigRectChanged();
        } );
    }

    QString m_strName;

    void setName(const QString&strName) { m_strName = strName; }
    QString getName(){ return m_strName; }

    void setItemRect(const QRectF &rectNew){ base.setItemRect( rectNew ); }
    QRectF getItemRect(){  return base.getItemRect(); }

protected:
    CustomBaseItem base;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        base.handleMousePressEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        base.handleMouseMoveEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        base.handleMouseReleaseEvent(event);
        emit sigRectChanged();
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override {
        base.handleHoverMoveEvent(event);
    }

    QRectF boundingRect() const override { return QGraphicsPixmapItem::boundingRect(); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        //QGraphicsPixmapItem::paint(painter, option, widget);

        QRectF rect = boundingRect();
        int m_nLineType = data(0).toInt();
        if(m_nLineType != 0)
        {
            painter->fillRect(rect,Qt::black);
        }
        else
        {
            QPixmap pix = this->pixmap();
            painter->drawImage(rect,pix.toImage());
        }

        if(isSelected())
        {
            painter->setPen(QPen(Qt::red,1,Qt::DashLine));
            painter->drawRect(rect);
        }
    }
};


#endif // CUSTOMITEMS_H
