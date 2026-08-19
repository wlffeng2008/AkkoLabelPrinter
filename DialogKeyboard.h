#ifndef DIALOGKEYBOARD_H
#define DIALOGKEYBOARD_H

#include <QDialog>
#include <QGraphicsScene>

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QPainter>

#include "FrameLabelView.h"

class QGraphicsKeyItem: public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit QGraphicsKeyItem(QGraphicsItem *parent = nullptr): QObject(), QGraphicsPixmapItem(QPixmap(), parent)
    {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsFocusable);
        setAcceptHoverEvents(true);
        setSize(100,100);
    };

    ~QGraphicsKeyItem() = default;

    void setText(const QString&text)
    {
        m_text=text;
        m_bTextmode=true;
    }
    void setImage(const QImage&image)
    {
        m_image=image;
        m_bTextmode=false;
    }
    void setSize(int w,int h)
    {
        QImage image(w,h,QImage::Format_ARGB32);
        //image.fill(Qt::transparent);
        QPainter painter(&image);
        setPixmap(QPixmap::fromImage(image));
        setScale(1);
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
        //QGraphicsPixmapItem::paint(painter, option, widget);
        QRectF rect = boundingRect();
        if(m_bTextmode)
        {
            painter->fillRect(rect,Qt::gray);
            painter->drawText(rect,m_text,QTextOption(Qt::AlignCenter));
        }
        else
        {
            painter->drawImage(rect,m_image);
        }

        if(isSelected())
        {
            painter->setPen(QPen(Qt::red,1,Qt::DashLine));
            painter->drawRect(rect);
        }
    }
private:
    QString m_text;
    QImage m_image;
    bool m_bTextmode=false;
};

namespace Ui {
class DialogKeyboard;
}

class DialogKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKeyboard(QWidget *parent = nullptr);
    ~DialogKeyboard();

private:
    Ui::DialogKeyboard *ui;

    CustomScene *m_sence=nullptr;
};

#endif // DIALOGKEYBOARD_H
