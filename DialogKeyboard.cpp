#include "DialogKeyboard.h"
#include "qgraphicsitem.h"
#include "ui_DialogKeyboard.h"
#include <QKeyEvent>
struct KeyMapItem
{
    const char* name;
    int hidUsageId;
    int ps2ScanCode;
};

// US QWERTY 104键盘完整普通按键表（不含Modifier修饰键）
const KeyMapItem g_keyTable[] = {
    // -------------------- 功能键区 F1~F12 --------------------
    { "Esc",          41,     1 },
    { "F1",           58,    59 },
    { "F2",           59,    60 },
    { "F3",           60,    61 },
    { "F4",           61,    62 },
    { "F5",           62,    63 },
    { "F6",           63,    64 },
    { "F7",           64,    65 },
    { "F8",           65,    66 },
    { "F9",           66,    67 },
    { "F10",          67,    68 },
    { "F11",          68,    87 },
    { "F12",          69,    88 },
    { "Print-Screen", 70, 57436 },
    { "Scroll-Lock" , 71,    70 },
    { "Pause-Break" , 72,    69 },
    { "Cal",         233,     0 },
    { "VOLx",        234,     0 },
    { "VOL-",        233,     0 },
    { "VOL+",        234,     0 },

    // -------------------- ESC 数字行 --------------------
    { "` ~",          53,    41 },
    { "1 !",          30,     2 },
    { "2 @",          31,     3 },
    { "3 #",          32,     4 },
    { "4 $",          33,     5 },
    { "5 %",          34,     6 },
    { "6 ^",          35,     7 },
    { "7 &",          36,     8 },
    { "8 *",          37,     9 },
    { "9 (",          38,    10 },
    { "0 )",          39,    11 },
    { "- _",          45,    12 },
    { "= +",          46,    13 },
    { "BackSpace",    42,    14 },
    { "Insert",       73, 57426 },
    { "Home",         74, 57415 },
    { "Pgup",         75, 57417 },
    { "Num Lock",     83, 57413 },
    { "Num /",        84, 57397 },
    { "Num *",        85,    55 },
    { "Num -",        86,    74 },

    // -------------------- Tab行 QWERTY --------------------
    { "Tab",          43,    15 },
    { "Q",            20,    16 },
    { "W",            26,    17 },
    { "E",             8,    18 },
    { "R",            21,    19 },
    { "T",            23,    20 },
    { "Y",            28,    21 },
    { "U",            24,    22 },
    { "I",            12,    23 },
    { "O",            18,    24 },
    { "P",            19,    25 },
    { "[ {",          47,    26 },
    { "] }" ,         48,    27 },
    { "\\ |",         49,    43 },
    { "Delete",       76, 57427 },
    { "End",          77, 57423 },
    { "Pgdn",         78, 57425 },
    { "Num 7",        95,    71 },
    { "Num 8",        96,    72 },
    { "Num 9",        97,    73 },
    { "Num +",        87,    78 },

    // -------------------- CapsLock A‑L --------------------
    { "Caps-Lock",    57,    58 },
    { "A",             4,    30 },
    { "S",            22,    31 },
    { "D",             7,    32 },
    { "F",             9,    33 },
    { "G",            10,    34 },
    { "H",            11,    35 },
    { "J",            13,    36 },
    { "K",            14,    37 },
    { "L",            15,    38 },
    { "; :",          51,    39 },
    { "' \"",         52,    40 },
    { "Enter",        40,    28 },
    { "Num 4",        92,    75 },
    { "Num 5",        93,    76 },
    { "Num 6",        94,    77 },

    // -------------------- Shift Z‑M --------------------
    { "L-Shift",     225,    42 },
    { "Z",            29,    44 },
    { "X",            27,    45 },
    { "C",             6,    46 },
    { "V",            25,    47 },
    { "B",             5,    48 },
    { "N",            17,    49 },
    { "M",            16,    50 },
    { ", <",          54,    51 },
    { ". >",          55,    52 },
    { "/ ?",          56,    53 },
    { "R-Shift",     229,    54 },
    { "↑",           82, 57416 },
    { "Num 1",        89,    79 },
    { "Num 2",        90,    80 },
    { "Num 3",        91,    81 },
    { "Num Enter",    88,   108 },

    // -------------------- Ctrl Win Alt 空格 --------------------
    { "L-Ctrl",      224,    29 },
    { "L-Alt",       226,    56 },
    { "L-Win",       227, 57435 },
    { "Space",        44,    57 },
    { "R-Alt",       230, 57400 },
    { "R-Win",       231, 57436 },
    { "Fn",            0,    0  },
    { "R-Ctrl",      228, 57373 },
    { "←"    ,       80, 57419 },
    { "↓",           81, 57424 },
    { "→",           79, 57421 },
    { "Num 0",        98,    82 },
    { "Num .",        99,    83 },
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

    qDebug() << "graphicsView:" << LSize;
    int nW = LSize.width();
    int nH = LSize.height();
    //ui->graphicsView->setFixedSize(nW-1,nH-1);
    m_sence->setSceneRect(QRectF(0,0,nW-1,nH-1));

    static bool updating = false;
    connect(m_sence,&CustomScene::itemSelected,this,[=](QGraphicsItem *item){
        updating = true;
        auto itemKey = dynamic_cast<QGraphicsKeyItem*>(item);
        if(itemKey)
        {
            ui->spinBoxX->setValue(itemKey->x());
            ui->spinBoxY->setValue(itemKey->y());
            ui->spinBoxW->setValue(itemKey->w());
            ui->spinBoxH->setValue(itemKey->h());
            ui->lineEditKeytext->setText(itemKey->text());
            ui->lineEditKeytHid->setText(QString("%1").arg(itemKey->hid()));
        }
        QTimer::singleShot(200,this,[=]{updating = false;});
    });

    connect(ui->spinBoxX,&QSpinBox::textChanged,this,[=](const QString & text){
        Q_UNUSED(text)
        if(updating) return;
        m_sence->SetValue(ui->spinBoxX->value(),0);
    });
    connect(ui->spinBoxY,&QSpinBox::textChanged,this,[=](const QString & text){
        Q_UNUSED(text)
        if(updating) return;
        m_sence->SetValue(ui->spinBoxY->value(),1);
    });
    connect(ui->spinBoxW,&QSpinBox::textChanged,this,[=](const QString & text){
        Q_UNUSED(text)
        if(updating) return;
        m_sence->SetValue(ui->spinBoxW->value(),2);
    });
    connect(ui->spinBoxH,&QSpinBox::textChanged,this,[=](const QString & text){
        Q_UNUSED(text)
        if(updating) return;
        m_sence->SetValue(ui->spinBoxH->value(),3);
    });

    QImage img(200,120,QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.fillRect(img.rect(),Qt::blue);

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
        int nBaseRx =0;
        QStringList strKeys = QString("Esc,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,PtrSc,ScrLk,Pause,Cal,Vx,V-,V+").split(',');
        int count = strKeys.count();
        for(int i=0; i<count; i++)
        {
            NT0 = new QGraphicsKeyItem(strKeys[i]);
            NT0->setPos(nPosX,nPosY);
            NT0->setSize(nKW,nKY);
            m_sence->addItem(NT0);
            if(i == 12) nBaseRx = NT0->rx();
            nPosX += NT0->w() + nGapX;
            if(i == 0) nPosX += NT0->w() + nGapX;
            if(i == 4 || i == 8) nPosX += NT0->w()*0.65;

            if(i == 12 || i == 15) nPosX += nGrpGapX;
            NT0->setHid(g_keyTable[hidIndex++].hidUsageId);
        }

        nPosX  = 20;
        nPosY += nKY+nGapY+5;
        strKeys = QString("~\n`,!\n1,@\n2,#\n3,$\n4,%\n5,^\n6,&\n7,*\n8,(\n9,)\n0,-\n_,+\n=,BackSpace,Ins,Home,PgUp,Num,/,*,-").split(',');
        count = strKeys.count();
        for(int i=0; i<count; i++)
        {
            NT0 = new QGraphicsKeyItem(strKeys[i]);
            NT0->setPos(nPosX,nPosY);
            NT0->setSize(nKW,nKY);
            m_sence->addItem(NT0);
            if(i==13){ NT0->setW((nKW+nGapX)*1.95); NT0->setRX(nBaseRx);}
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
            if(i==13) {NT0->setW((nKW+nGapX)*1.50);NT0->setRX(nBaseRx);}
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
            if(i==12){ NT0->setW((nKW+nGapX)*2.30);NT0->setRX(nBaseRx);}
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
            if(i==11) {NT0->setW((nKW+nGapX)*2.80);NT0->setRX(nBaseRx);}
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
            if(i==3 )  NT0->setW((nKW+nGapX)*5.00);
            if(i==11)  NT0->setW(nKW*2+nGapX);
            if(i==7) NT0->setRX(nBaseRx);
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
