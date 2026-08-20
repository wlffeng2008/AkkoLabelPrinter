#include "DialogKeyboard.h"
#include "qgraphicsitem.h"
#include "ui_DialogKeyboard.h"
#include <QKeyEvent>
struct KeyMapItem
{
    const char* name;
    int ps2ScanCode;
    int hidUsageId;
};

// US QWERTY 104键盘完整普通按键表（不含Modifier修饰键）
const KeyMapItem g_keyTable[] = {
    // -------------------- 功能键区 F1~F12 --------------------
    { "Esc",           1,    41 },
    { "F1",           59,    58 },
    { "F2",           60,    59 },
    { "F3",           61,    60 },
    { "F4",           62,    61 },
    { "F5",           63,    62 },
    { "F6",           64,    63 },
    { "F7",           65,    64 },
    { "F8",           66,    65 },
    { "F9",           67,    66 },
    { "F10",          68,    67 },
    { "F11",          87,    68 },
    { "F12",          88,    69 },
    { "PrintScreen",  99,    70 },
    { "ScrollLock",   70,    71 },
    { "Pause",       100,    72 },
    { "Cal",           0,    233 },
    { "VOLx",          0,    234 },
    { "VOL-",          0,    233 },
    { "VOL+",          0,    234 },

    // -------------------- ESC 数字行 --------------------
    { "` ~",         41,      53 },
    { "1 !",          2,      30 },
    { "2 @",          3,      31 },
    { "3 #",          4,      32 },
    { "4 $",          5,      33 },
    { "5 %",          6,      34 },
    { "6 ^",          7,      35 },
    { "7 &",          8,      36 },
    { "8 *",          9,      37 },
    { "9 (",         10,      38 },
    { "0 )",         11,      39 },
    { "- _",         12,      45 },
    { "= +",         13,      46 },
    { "Backspace",   14,      42 },
    { "Insert",      82,      73 },
    { "Home",        71,      74 },
    { "NumLock",     69,      83 },
    { "PageUp",      73,      75 },
    { "KP /",       100,      84 },
    { "KP *",        55,      85 },
    { "KP -",        74,      86 },

    // -------------------- Tab行 QWERTY --------------------
    { "Tab",         43,      15 }, //修正：Tab HID=11
    { "Q",           20,      16 },
    { "W",           26,      17 },
    { "E",           18,       8 },
    { "R",           19,      21 },
    { "T",           20,      23 },
    { "Y",           21,      28 },
    { "U",           22,      24 },
    { "I",           23,      12 },
    { "O",           21,      18 },
    { "P",           25,      19 },
    { "[ {",         26,      47 },
    { "] }",         27,      48 },
    { "\\ |",        43,      49 },
    { "Delete",      83,      76 },
    { "End",         79,      77 },
    { "PageDown",    81,      78 },
    { "KP 7 Home",   71,      95 },
    { "KP 8 Up",     72,      96 },
    { "KP 9 PgUp",   73,      97 },
    { "KP +",        78,      87 },

    // -------------------- CapsLock A‑L --------------------
    { "CapsLock",    58,      57 },
    { "A",           30,       4 },
    { "S",           31,      22 },
    { "D",           32,       7 },
    { "F",           33,       9 },
    { "G",           34,      10 },
    { "H",           35,      11 },
    { "J",           36,      13 },
    { "K",           37,      14 },
    { "L",           38,      15 },
    { "; :",         39,      28 },
    { "' \"",        40,      29 },
    { "Enter",       28,      40 },
    { "KP 4 Left",   75,      92 },
    { "KP 5",        76,      93 },
    { "KP 6 Right",  77,      94 },

    // -------------------- Shift Z‑M --------------------
    { "LShift",      42,     225 },
    { "Z",           44,      29 },
    { "X",           45,      31 },
    { "C",           46,      32 },
    { "V",           47,      33 },
    { "B",           48,      34 },
    { "N",           49,      35 },
    { "M",           50,      36 },
    { ", <",         51,      37 },
    { ". >",         52,      38 },
    { "/ ?",         53,      39 },
    { "RShift",      54,     229 }, //RShift属于Modifier
    { "Up",          72,      82 },
    { "KP 1 End",    79,      89 },
    { "KP 2 Down",   80,      90 },
    { "KP 3 PgDn",   81,      91 },
    { "KP Enter",   108,      88 },

    // -------------------- Ctrl Win Alt 空格 --------------------
    { "LCtrl",       29,     224 },
    { "LWin",        91,     227 },
    { "LAlt",        56,     226 },
    { "Space",       57,      44 },
    { "RAlt",        92,     230 },
    { "RWin",        93,     231 },
    { "Menu",        93,     101 },
    { "RCtrl",       29,     228 },
    { "Left",        75,      80 },
    { "Down",        80,      81 },
    { "Right",       77,      79 },
    { "KP 0 Ins",    82,      98 },
    { "KP . Del",    83,      99 },
};

const int g_keyTableCount = sizeof(g_keyTable)/sizeof(KeyMapItem);

DialogKeyboard::DialogKeyboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogKeyboard)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);

    qDebug()<< "g_keyTableCount:" << g_keyTableCount;

    m_sence = new CustomScene(this);

    ui->graphicsView->setScene(m_sence);

    ui->graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    QSize LSize = ui->graphicsView->size();

    int nW = LSize.width();
    int nH = LSize.height();
    ui->graphicsView->setFixedSize(nW-1,nH-1);
    m_sence->setSceneRect(QRectF(0,0,nW-1,nH-1));

    connect(m_sence,&CustomScene::itemSelected,this,[=](QGraphicsItem *item){
        auto itemKey = dynamic_cast<QGraphicsKeyItem*>(item);
        if(itemKey)
        {
            ui->spinBoxX->setValue(itemKey->x());
            ui->spinBoxY->setValue(itemKey->y());
            ui->spinBoxW->setValue(itemKey->w());
            ui->spinBoxH->setValue(itemKey->h());
        }
    });

    connect(ui->spinBoxX,&QSpinBox::textChanged,this,[=](const QString & text){
        m_sence->SetValue(ui->spinBoxX->value(),0);
    });
    connect(ui->spinBoxY,&QSpinBox::textChanged,this,[=](const QString & text){

        m_sence->SetValue(ui->spinBoxY->value(),1);
    });
    connect(ui->spinBoxW,&QSpinBox::textChanged,this,[=](const QString & text){
        m_sence->SetValue(ui->spinBoxW->value(),2);
    });
    connect(ui->spinBoxH,&QSpinBox::textChanged,this,[=](const QString & text){
        m_sence->SetValue(ui->spinBoxH->value(),3);
    });

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

    connect(ui->pushButtonReset,&QPushButton::clicked,this,[=]{
        m_sence->clear();
        QGraphicsKeyItem *NT0 = nullptr;
        float scale = 0.4 ;
        int nGapX = 20 * scale;
        int nGapY = 20 * scale;
        int nKW = 100 * scale;
        int nKY = 100 * scale;
        int nGrpGapX=30 * scale;

        int hidIndex=0;

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
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
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
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
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
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
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
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
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
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
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
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
        }

        qDebug() << m_sence->items().size() << hidIndex;
    });
}

DialogKeyboard::~DialogKeyboard()
{
    delete ui;
}

void DialogKeyboard::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << event->key() << event->nativeScanCode() << event->nativeVirtualKey();

    QDialog::keyPressEvent(event);
}
