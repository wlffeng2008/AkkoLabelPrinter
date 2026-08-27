#ifndef CUSTOMITEMS_H
#define CUSTOMITEMS_H

//#include "FrameLabelView.h"
#include "qgraphicsscene.h"
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
#include <QLineEdit>
#include <QJsonObject>
#include <QGraphicsProxyWidget>
#include <QBuffer>


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
    CustomTextItem(const QString &text, QGraphicsItem *parent = nullptr)
        : QGraphicsTextItem(text, parent), base(this)
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



class QGraphicsKeyItem: public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit QGraphicsKeyItem(QGraphicsItem *parent = nullptr): QObject(), QGraphicsPixmapItem(QPixmap(), parent)
    {
        init();
    }

    explicit QGraphicsKeyItem(const QImage&image,QGraphicsItem *parent = nullptr): QObject(), QGraphicsPixmapItem(QPixmap(), parent)
    {
        init();
        setImage(image);
    }

    explicit QGraphicsKeyItem(const QString&text,QGraphicsItem *parent = nullptr): QObject(), QGraphicsPixmapItem(QPixmap(), parent)
    {
        init();

        QImage image(text);
        if(image.isNull())
        {
            setSize(42,42);
            setText(text);
        }
        else
        {
            setSize(100,100);
            setImage(image);
        }
    }

    static QImage ImageFromBase64(const QString &imageData)
    {
        QByteArray byteArray = QByteArray::fromBase64(imageData.toLatin1());
        QImage image;
        image.loadFromData(byteArray, "PNG");
        return image;
    }

    static QString ImageToBase64(const QImage &image)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        return  QString::fromLatin1(byteArray.toBase64());
    }

    ~QGraphicsKeyItem() = default;

    QJsonObject toJsonObject()
    {
        QJsonObject jObj;
        jObj["x"] = x();
        jObj["y"] = y();
        jObj["w"] = w();
        jObj["h"] = h();
        jObj["rx"] = rx();
        jObj["ry"] = ry();
        jObj["text"] = this->text();
        jObj["name"] = m_name;
        jObj["hid"]  = m_hid;
        jObj["type"] = m_type;
        jObj["font"] = m_font.toString();
        jObj["image"] = ImageToBase64(m_image);

        return jObj;
    }

    void SetJsonObject(const QJsonObject&jObj){
        setPos(jObj["x"].toInt(),jObj["y"].toInt());
        setSize(jObj["w"].toInt(),jObj["h"].toInt());
        setText(jObj["text"].toString());
        setName(jObj["name"].toString());
        m_hid = jObj["hid"].toInt();
        m_type = jObj["type"].toInt();
        m_image = ImageFromBase64(jObj["image"].toString());
    }

private:
    int m_nW;
    int m_nH;
    int m_hid=0;
    QFont m_font = QFont("微软雅黑",9,600);
    QString m_name;
    QString m_text;
    QImage m_image;
    int m_type=0;
    bool m_bLocking=false;

    void init()
    {
        Lock(false);
        setAcceptHoverEvents(true);
        setSize(100,100);
        setY(5);
        setCursor(Qt::PointingHandCursor);
    }

public:
    QString name(){ return m_name; }
    void setName(const QString&name){ m_name=name; }

    void setFontSize(int size){ m_font.setPixelSize(size); }
    int  fontSize(){ return m_font.pixelSize(); }
    bool bold(){ m_font.bold(); }
    void setBold(bool bold){ m_font.setBold(bold); }
    QFont font(){return m_font;}
    void setFont(const QFont&font){ m_font=font; }

    int hid(){ return m_hid;}
    void setHid(int hid){m_hid = hid;}

    int x(){ return pos().x(); }
    int y(){ return pos().y(); }
    int w(){ return m_nW; }
    int h(){ return m_nH; }

    void setX(int x){ this->setPos(x,pos().y()); }
    void setY(int y){ this->setPos(pos().x(),y); }
    void setW(int w){ setSize(w,m_nH); }
    void setH(int h){ setSize(m_nW,h); }
    int rx(){ return (x() + w());}
    int ry(){ return (y() + h());}
    void setRX(int rx){ setW(rx - x()); }
    void setRY(int ry){ setH(ry - y()); }

    void Lock(bool lock=true)
    {
        m_bLocking=lock;

        QGraphicsItem::GraphicsItemFlags flags;
        flags |= QGraphicsItem::ItemIsSelectable;
        flags |= QGraphicsItem::ItemSendsGeometryChanges;
        flags |= QGraphicsItem::ItemIsFocusable;

        if (!lock)
        {
            flags |= QGraphicsItem::ItemIsMovable;
        }

        setFlags(flags);
    }

    QString text()
    {
        QString tmp(m_text);
        return tmp.replace("\n","\\n");
    }

    void setText(const QString&text)
    {
        QString tmp(text);
        m_text = tmp.replace("\\n","\n");
        m_type=0;
    }

    void setImage(const QImage&image)
    {
        m_image=image;
        m_type=1;
    }

    void setVLine(int height=100){
        setSize(2,height);
        m_type=2;
    }

    void setHLine(int width=200){

        setSize(width,2);
        m_type=3;
    }

    QSize size(){ return QSize(m_nW,m_nH); }
    void setSize(int w,int h)
    {
        QImage image(w,h,QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        setPixmap(QPixmap::fromImage(image));
        setScale(1);
        m_nW = w;
        m_nH = h;
    }

    void move(int step,int type){
        int nx = x();
        int ny = y();
        int lx =this->scene()->sceneRect().right()-2;
        int ly =this->scene()->sceneRect().bottom()-2;
        switch(type)
        {
        case 0: ny -= step;break;
        case 1: ny += step;break;
        case 2: nx -= step;break;
        case 3: nx += step;break;
        }
        if(nx>lx) nx=lx;
        if(ny>ly) ny=ly;
        setX(nx);
        setY(ny);
    }

    void extend(int toValue,int type){
        switch(type)
        {
        case 0:
        {
            int nm = y() - toValue;
            int nd = h() + nm;
            setY(toValue);
            setH(nd);
        }
            break;
        case 1:
        {
            int nh = toValue - y() + h();
            setH(nh);
        }
            break;

        case 2:
        {
            int nm = x() - toValue;
            int nd = w() + nm;
            setX(toValue);
            setW(nd);
        }
            break;

        case 3:
        {
            int nw = toValue - x() + w();
            setW(nw);
        }
            break;
        }
    }

    void setAlignTo(int toValue,int type)
    {
        switch(type)
        {
        case 0:
        {
            setY(toValue);
        }
        break;
        case 1:
        {
            setY(toValue - h());
        }
        break;

        case 2:
        {
            setX(toValue);
        }
        break;

        case 3:
        {
            setW(toValue - w());
        }
        break;
        }
    }

    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addRect(boundingRect());
        return path;
    }

    QRectF boundingRect() const override { return QGraphicsPixmapItem::boundingRect(); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->save();

        QRectF rect = boundingRect();

        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        bool isVolRound = false;
        bool bDone = false;
        if(m_hid == 233 || m_hid == 234 || m_hid == 235 || m_hid == 236)
        {
            if(h()/w() == 3)
                isVolRound = true;
        }

        if(isVolRound)
        {
            int h = this->h();
            int s = h/3;
            QImage img(h,h,QImage::Format_ARGB32);
            img.fill(Qt::transparent);
            QPainter tmp(&img);
            tmp.setRenderHint(QPainter::Antialiasing);
            QRect rcCopy;
            if(m_hid == 233)
            {
                rcCopy = QRect(0,0,s,h);
                tmp.setBrush(isSelected()?Qt::gray:Qt::red);
            }
            else if(m_hid == 234)
            {
                rcCopy = QRect(s*2,0,s,h);
                tmp.setBrush(isSelected()?Qt::gray:Qt::blue);
            }
            else if(m_hid == 235 || m_hid == 236)
            {
                rcCopy = QRect(s,0,s,h);
                tmp.setBrush(isSelected()?Qt::gray:Qt::green);
            }
            if(!rcCopy.isEmpty())
            {
                bDone = true;
                tmp.drawRoundedRect(img.rect().adjusted(1,1,-1,-1),h/2-1,h/2-1);
                painter->drawImage(rect,img.copy(rcCopy));
            }
        }

        if(!bDone)
        {
            if(m_type == 0)
            {
                painter->setRenderHint(QPainter::Antialiasing);
                if(isSelected())
                {
                    //painter->setPen(QPen(Qt::red,1,Qt::DashLine));
                    painter->setBrush(Qt::gray);
                }
                else
                {
                    painter->setBrush(Qt::white);
                }
                painter->drawRoundedRect(rect,14,14);

                QString text = m_text;
                if(m_bEditing) text.clear();

                painter->setFont(m_font);
                painter->drawText(rect,text,QTextOption(Qt::AlignCenter));
            }
            else
            {
                if(m_type == 1)
                {
                    QPainterPath path;
                    path.addRoundedRect(rect,14,14);
                    painter->setClipPath(path);
                    painter->drawImage(rect,m_image);
                    if(isSelected())
                    {
                        painter->setPen(QPen(Qt::red,1,Qt::DashLine));
                        painter->drawRoundedRect(rect,14,14);
                    }
                }
                else
                {
                    painter->fillRect(rect,Qt::black);
                }
            }
        }

        painter->restore();
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        qDebug() << QString::asprintf("X:%4d  Y:%4d  RX:%4d  RY:%4d  W:%3d  H:%3d",x(),y(),rx(),ry(),w(),h());
        setSelected(!isSelected());
        event->accept();
        //QGraphicsPixmapItem::mousePressEvent(event);
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        event->accept();
        //QGraphicsPixmapItem::mouseReleaseEvent(event);
    }

    bool m_bEditing = false;
    QLineEdit *edit = nullptr;
    QGraphicsProxyWidget *proxy = nullptr;

    bool sceneEventFilter(QGraphicsItem *watched,QEvent *event) override {
        if(proxy == watched && m_bEditing)
        {
            if(event->type() == QEvent::FocusOut)
            {
                this->setText(edit->text().trimmed());
                m_bEditing = false;
                proxy->hide();
                return true;
            }
        }
        return QGraphicsPixmapItem::sceneEventFilter(watched,event);
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        if(m_type == 0 && !m_bLocking)
        {
            if(!edit)
            {
                edit = new QLineEdit("");
                edit->setGeometry(x(),y(),w(),h());
                edit->setStyleSheet("QLineEdit{border:none;border-radius:14px;background-color:transparent;}");
                connect(edit,&QLineEdit::editingFinished,this,[=]{
                    this->setText(edit->text().trimmed());
                    m_bEditing = false;
                    proxy->hide();
                });
                proxy = scene()->addWidget(edit);
                proxy->setZValue(zValue() + 1);

                proxy->installSceneEventFilter(this);
            }

            m_bEditing = true;
            QFont font = m_font;
            font.setPixelSize(14);
            edit->setFont(font);
            edit->setText(this->text());
            edit->selectAll();
            edit->setAlignment(Qt::AlignCenter);
            proxy->show();
            edit->setFocus();
            update();
            event->accept();
            return;
        }

        QGraphicsPixmapItem::mouseDoubleClickEvent(event);
    }
};



#endif // CUSTOMITEMS_H
