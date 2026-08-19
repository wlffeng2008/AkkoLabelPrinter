#include "DialogKeyboard.h"
#include "qgraphicsitem.h"
#include "ui_DialogKeyboard.h"


DialogKeyboard::DialogKeyboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogKeyboard)
{
    ui->setupUi(this);

    m_sence = new CustomScene(this);

    ui->graphicsView->setScene(m_sence);

    QSize LSize = ui->graphicsView->size();

    int nW = LSize.width();
    int nH = LSize.height();
    ui->graphicsView->setFixedSize(nW-1,nH-1);
    m_sence->setSceneRect(QRectF(0,0,nW-10,nH-10));

    QImage img(200,120,QImage::Format_ARGB32);

    QPainter painter(&img);
    painter.fillRect(img.rect(),Qt::blue);

    // QGraphicsTextItem *item0 = new QGraphicsTextItem("1");
    // QGraphicsPixmapItem *item1 = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    // QGraphicsLineItem *item2 = new QGraphicsLineItem(20,20,200,200);

    // item0->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsFocusable);
    // item0->setAcceptHoverEvents(true);
    // item2->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsFocusable);
    // item2->setAcceptHoverEvents(true);

    // item1->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsFocusable);
    // item1->setAcceptHoverEvents(true);
    // m_sence->addItem(item0);
    // m_sence->addItem(item1);
    // m_sence->addItem(item2);

    // item2->setPen(QPen(Qt::blue,2));

    // item0->setPlainText("1234567890");
    // QFont font = this->font();
    // font.setPixelSize(24);
    // item0->setFont(font);
    // item0->setScale(3);


    // QGraphicsKeyItem *NT0 = new QGraphicsKeyItem();
    // QGraphicsKeyItem *NT1 = new QGraphicsKeyItem();
    // NT0->setText("供方质检结果");
    // NT0->setSize(100,80);
    // m_sence->addItem(NT0);
    // NT1->setImage(QImage("D:\\drug1.jfif"));

    // m_sence->addItem(NT1);

    for(int i=0; i< 10; i++)
    {
        QGraphicsKeyItem *NT0 = new QGraphicsKeyItem();
        NT0->setSize(40,40);
        NT0->setText(QString('A' + i));
        NT0->setPos(20+ i* 45 , 20);
        m_sence->addItem(NT0);
    }
}

DialogKeyboard::~DialogKeyboard()
{
    delete ui;
}
