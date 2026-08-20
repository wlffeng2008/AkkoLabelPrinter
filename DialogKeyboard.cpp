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

    ui->graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    QSize LSize = ui->graphicsView->size();

    int nW = LSize.width();
    int nH = LSize.height();
    ui->graphicsView->setFixedSize(nW-1,nH-1);
    m_sence->setSceneRect(QRectF(0,0,nW-1,nH-1));

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

    // for(int i=0; i < 26; i++)
    // {
    //     QGraphicsKeyItem *NT0 = new QGraphicsKeyItem();
    //     NT0->setSize(42,42);
    //     NT0->setText(QString('A' + i));
    //     NT0->setPos(20 + (i %10)* 52 , 110 + 50 *(i/10));
    //     m_sence->addItem(NT0);
    // }

    QGraphicsKeyItem *NT0 = new QGraphicsKeyItem("D:\\radioChecked.png");
    //NT0->setY(300);
    //m_sence->addItem(NT0);

    float scale = .4 ;
    int nGapX = 20 * scale;
    int nGapY = 20 * scale;
    int nKW = 100 * scale;
    int nKY = 100 * scale;
    int nGrpGapX=30 * scale;

    int nPosX = 20 ;
    int nPosY = 20 ;
    QStringList strKeys = QString("Esc,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,PtrSc,ScrLk,Pause,Cal,Vx,V-,V+").split(',');
    int count = strKeys.count();
    for(int i=0; i<count; i++)
    {
        NT0 = new QGraphicsKeyItem(strKeys[i]);
        NT0->setPos(nPosX,nPosY);
        NT0->setSize(nKW,nKY);
        m_sence->addItem(NT0);
        nPosX += NT0->w() + nGapX;
        if(i==0 ) nPosX += NT0->w() + nGapX;
        if(i == 4 || i == 8) nPosX += NT0->w()*0.65;

        if(i == 12 || i == 15) nPosX += nGrpGapX;
    }

    nPosX  = 20;
    nPosY += nKY+nGapY+10;
    strKeys = QString("~\n`,!\n1,@\n2,#\n3,$\n4,%\n5,^\n6,&\n7,*\n8,(\n9,)\n0,-\n_,+\n=,BackSpace,Ins,Home,PgUp,Num,/,*,-").split(',');
    count = strKeys.count();
    for(int i=0; i<count; i++)
    {
        NT0 = new QGraphicsKeyItem(strKeys[i]);
        NT0->setPos(nPosX,nPosY);
        NT0->setSize(nKW,nKY);
        m_sence->addItem(NT0);
        if(i==13){ NT0->setW((nKW+nGapX)*1.95);}
        nPosX += NT0->w() + nGapX;

        if(i==13 || i==16) nPosX += nGrpGapX;
    }

    nPosX=20;
    nPosY += nKY+nGapY;
    strKeys = QString("Tab,Q,W,E,R,T,Y,U,I,O,P,{\n[,}\n},|\n\\,Del,End,PnDn,7\nHome,8\n↑,9\nPgUp,+").split(',');
    count = strKeys.count();
    for(int i=0; i<count; i++)
    {
        NT0 = new QGraphicsKeyItem(strKeys[i]);
        NT0->setPos(nPosX,nPosY);
        NT0->setSize(nKW,nKY);
        m_sence->addItem(NT0);
        if(i==0)  NT0->setW((nKW+nGapX)*1.30);
        if(i==13) NT0->setW((nKW+nGapX)*1.50);
        nPosX += NT0->w() + nGapX;

        if(i==13 || i==16) nPosX += nGrpGapX;
    }
    NT0->setH(nKW*2+nGapY);

    nPosX  = 20;
    nPosY += nKY+nGapY;
    strKeys = QString("Cap Lock,A,S,D,F,G,H,J,K,L,:\n;,\"\n\',Enter,4\n←,5,6\n→").split(',');
    count = strKeys.count();
    for(int i=0; i<count; i++)
    {
        NT0 = new QGraphicsKeyItem(strKeys[i]);
        NT0->setPos(nPosX,nPosY);
        NT0->setSize(nKW,nKY);
        m_sence->addItem(NT0);
        if(i==0)  NT0->setW((nKW+nGapX)*1.50);
        if(i==12) NT0->setW((nKW+nGapX)*2.30) ;
        nPosX += NT0->w() + nGapX;
        if(i==12) nPosX += (nKW+nGapX)*3;

        if(i==12) nPosX += nGrpGapX*2;
    }

    nPosX  = 20;
    nPosY+= nKY+nGapY;
    strKeys = QString("Shift,Z,X,C,V,B,N,M,<\n，,>\n.,?\n/,Shift,↑,1\nEnd,2\n↓,3\nPgDn,Enter").split(',');
    count = strKeys.count();
    for(int i=0; i<count; i++)
    {
        NT0 = new QGraphicsKeyItem(strKeys[i]);
        NT0->setPos(nPosX,nPosY);
        NT0->setSize(nKW,nKY);
        m_sence->addItem(NT0);
        if(i==0)  NT0->setW((nKW+nGapX)*2.00);
        if(i==11) NT0->setW((nKW+nGapX)*2.80);
        nPosX += NT0->w() + nGapX;
        if(i==11) nPosX += nKW+nGapX;
        if(i==12) nPosX += nKW+nGapX;

        if(i==11 || i==12) nPosX += nGrpGapX;
    }
    NT0->setW(nKW);
    NT0->setH(nKW*2+nGapY);

    nPosX  = 20;
    nPosY+= nKY+nGapY;
    strKeys = QString("Ctrl,Win,Alt,Space,Alt,Win,Fn,Ctrl,←,↓,→,0\nIns,Del").split(',');
    count = strKeys.count();
    for(int i=0; i<count; i++)
    {
        NT0 = new QGraphicsKeyItem(strKeys[i]);
        NT0->setPos(nPosX,nPosY);
        NT0->setSize(nKW,nKY);
        m_sence->addItem(NT0);
        if(i<=7 )  NT0->setW(nKW*1.50);
        if(i==3)   NT0->setW((nKW+nGapX)*5.00);
        if(i==11)  NT0->setW(nKW*2+nGapX);
        nPosX += NT0->w() + nGapX;

        if(i==7 || i==10) nPosX += nGrpGapX;
    }

    qDebug() << m_sence->items().size();

}

DialogKeyboard::~DialogKeyboard()
{
    delete ui;
}
