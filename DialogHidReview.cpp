#include "DialogHidReview.h"
#include "ui_DialogHidReview.h"


QString strPage0(R"(0x00 – 0x3F 基础键
Hex,Dec,Name
0x00,0,No Event / None
0x01,1,ErrorRollOver
0x02,2,POSTFail
0x03,3,ErrorUndefined
0x04,4,Keyboard a / A
0x05,5,Keyboard b / B
0x06,6,Keyboard c / C
0x07,7,Keyboard d / D
0x08,8,Keyboard e / E
0x09,9,Keyboard f / F
0x0A,10,Keyboard g / G
0x0B,11,Keyboard h / H
0x0C,12,Keyboard i / I
0x0D,13,Keyboard j / J
0x0E,14,Keyboard k / K
0x0F,15,Keyboard l / L
0x10,16,Keyboard m / M
0x11,17,Keyboard n / N
0x12,18,Keyboard o / O
0x13,19,Keyboard p / P
0x14,20,Keyboard q / Q
0x15,21,Keyboard r / R
0x16,22,Keyboard s / S
0x17,23,Keyboard t / T
0x18,24,Keyboard u / U
0x19,25,Keyboard v / V
0x1A,26,Keyboard w / W
0x1B,27,Keyboard x / X
0x1C,28,Keyboard y / Y
0x1D,29,Keyboard z / Z
0x1E,30,Keyboard 1 / !
0x1F,31,Keyboard 2 / @
0x20,32,Keyboard 3 / #
0x21,33,Keyboard 4 / $
0x22,34,Keyboard 5 / %
0x23,35,Keyboard 6 / ^
0x24,36,Keyboard 7 / &
0x25,37,Keyboard 8 / *
0x26,38,Keyboard 9 / (
0x27,39,Keyboard 0 / )
0x28,40,Keyboard Enter
0x29,41,Keyboard Escape
0x2A,42,Keyboard Backspace
0x2B,43,Keyboard Tab
0x2C,44,Keyboard Spacebar
0x2D,45,Keyboard - / _
0x2E,46,Keyboard = / +
0x2F,47,Keyboard [ / {
0x30,48,Keyboard ] / }
0x31,49,Keyboard \ / |
0x32,50,Keyboard Non-US # / ~
0x33,51,Keyboard ; / :
0x34,52,"Keyboard ' / """
0x35,53,Keyboard ` / ~
0x36,54,"Keyboard , / <"
0x37,55,Keyboard . / >
0x38,56,Keyboard / / ?
0x39,57,Keyboard Caps Lock
0x3A,58,Keyboard F1
0x3B,59,Keyboard F2
0x3C,60,Keyboard F3
0x3D,61,Keyboard F4
0x3E,62,Keyboard F5
0x3F,63,Keyboard F6)");

QString strPage1(R"(0x40 – 0x67 功能键 / 编辑键 / 小键盘
Hex,Dec,Name
0x40,64,Keyboard F7
0x41,65,Keyboard F8
0x42,66,Keyboard F9
0x43,67,Keyboard F10
0x44,68,Keyboard F11
0x45,69,Keyboard F12
0x46,70,Keyboard PrintScreen
0x47,71,Keyboard Scroll Lock
0x48,72,Keyboard Pause
0x49,73,Keyboard Insert
0x4A,74,Keyboard Home
0x4B,75,Keyboard PageUp
0x4C,76,Keyboard Delete (Forward)
0x4D,77,Keyboard End
0x4E,78,Keyboard PageDown
0x4F,79,Keyboard Right Arrow
0x50,80,Keyboard Left Arrow
0x51,81,Keyboard Down Arrow
0x52,82,Keyboard Up Arrow
0x53,83,Keypad Num Lock / Clear
0x54,84,Keypad /
0x55,85,Keypad *
0x56,86,Keypad -
0x57,87,Keypad +
0x58,88,Keypad Enter
0x59,89,Keypad 1 / End
0x5A,90,Keypad 2 / Down
0x5B,91,Keypad 3 / PageDn
0x5C,92,Keypad 4 / Left
0x5D,93,Keypad 5
0x5E,94,Keypad 6 / Right
0x5F,95,Keypad 7 / Home
0x60,96,Keypad 8 / Up
0x61,97,Keypad 9 / PageUp
0x62,98,Keypad 0 / Insert
0x63,99,Keypad . / Delete
0x64,100,Keyboard Non-US \ / |
0x65,101,Keyboard Application (Menu)
0x66,102,Keyboard Power
0x67,103,Keypad =)");

QString strPage2(R"(0x68 – 0x83 扩展 F 键与系统键
Hex,Dec,Name
0x68,104,Keyboard F13
0x69,105,Keyboard F14
0x6A,106,Keyboard F15
0x6B,107,Keyboard F16
0x6C,108,Keyboard F17
0x6D,109,Keyboard F18
0x6E,110,Keyboard F19
0x6F,111,Keyboard F20
0x70,112,Keyboard F21
0x71,113,Keyboard F22
0x72,114,Keyboard F23
0x73,115,Keyboard F24
0x74,116,Keyboard Execute
0x75,117,Keyboard Help
0x76,118,Keyboard Menu
0x77,119,Keyboard Select
0x78,120,Keyboard Stop
0x79,121,Keyboard Again
0x7A,122,Keyboard Undo
0x7B,123,Keyboard Cut
0x7C,124,Keyboard Copy
0x7D,125,Keyboard Paste
0x7E,126,Keyboard Find
0x7F,127,Keyboard Mute
0x80,128,Keyboard Volume Up
0x81,129,Keyboard Volume Down
0x82,130,Keyboard Locking Caps Lock
0x83,131,Keyboard Locking Num Lock)");

QString strPage3(R"(0x84 – 0xA4 锁定 / 国际化 / 旧系统键
Hex,Dec,Name
0x84,132,Keyboard Locking Scroll Lock
0x85,133,Keypad Comma
0x86,134,Keypad Equal Sign (AS/400)
0x87,135,Keyboard International 1 (Ro)
0x88,136,Keyboard International 2 (Kana)
0x89,137,Keyboard International 3 (Yen)
0x8A,138,Keyboard International 4
0x8B,139,Keyboard International 5
0x8C,140,Keyboard International 6
0x8D,141,Keyboard International 7
0x8E,142,Keyboard International 8
0x8F,143,Keyboard International 9
0x90,144,Keyboard LANG 1 (Hangul/English)
0x91,145,Keyboard LANG 2 (Hanja)
0x92,146,Keyboard LANG 3 (Katakana)
0x93,147,Keyboard LANG 4 (Hiragana)
0x94,148,Keyboard LANG 5
0x95,149,Keyboard LANG 6
0x96,150,Keyboard LANG 7
0x97,151,Keyboard LANG 8
0x98,152,Keyboard LANG 9
0x99,153,Keyboard Alternate Erase
0x9A,154,Keyboard SysReq / Attention
0x9B,155,Keyboard Cancel
0x9C,156,Keyboard Clear
0x9D,157,Keyboard Prior
0x9E,158,Keyboard Return
0x9F,159,Keyboard Separator)");

QString strPage4(R"(0xA0 – 0xE7 保留区与 Modifier
Hex,Dec,Name
0xA0,160,Keyboard Out
0xA1,161,Keyboard Oper
0xA2,162,Keyboard Clear / Again
0xA3,163,Keyboard CrSel / Props
0xA4,164,Keyboard ExSel
0xA5–0xAF,—,Reserved
0xB0,176,Keypad 00
0xB1,177,Keypad 000
0xB2,178,Thousands Separator
0xB3,179,Decimal Separator
0xB4,180,Currency Unit
0xB5,181,Currency Sub-unit
0xB6,182,Keypad (
0xB7,183,Keypad )
0xB8,184,Keypad {
0xB9,185,Keypad }
0xBA,186,Keypad Tab
0xBB,187,Keypad Backspace
0xBC,188,Keypad A
0xBD,189,Keypad B
0xBE,190,Keypad C
0xBF,191,Keypad D
0xC0,192,Keypad E
0xC1,193,Keypad F
0xC2,194,Keypad XOR
0xC3,195,Keypad ^
0xC4,196,Keypad %
0xC5,197,Keypad <
0xC6,198,Keypad >
0xC7,199,Keypad &
0xC8,200,Keypad &&
0xC9,201,Keypad |
0xCA,202,Keypad ||
0xCB,203,Keypad :
0xCC,204,Keypad #
0xCD,205,Keypad Space
0xCE,206,Keypad @
0xCF,207,Keypad !
0xD0,208,Keypad Mem Store
0xD1,209,Keypad Mem Recall
0xD2,210,Keypad Mem Clear
0xD3,211,Keypad Mem Add
0xD4,212,Keypad Mem Subtract
0xD5,213,Keypad Mem Multiply
0xD6,214,Keypad Mem Divide
0xD7,215,Keypad +/-
0xD8,216,Keypad Clear
0xD9,217,Keypad Clear Entry
0xDA,218,Keypad Binary
0xDB,219,Keypad Octal
0xDC,220,Keypad Decimal
0xDD,221,Keypad Hexadecimal
0xDE–0xDF,—,Reserved
0xE0,224,Keyboard Left Control (Modifier bit0)
0xE1,225,Keyboard Left Shift (Modifier bit1)
0xE2,226,Keyboard Left Alt (Modifier bit2)
0xE3,227,Keyboard Left GUI (Win/Cmd) (bit3)
0xE4,228,Keyboard Right Control (bit4)
0xE5,229,Keyboard Right Shift (bit5)
0xE6,230,Keyboard Right Alt (bit6)
0xE7,231,Keyboard Right GUI (bit7)
)");

QString strPage5(R"(速查要点
字母 A​ = 0x04，Enter​ = 0x28，Esc​ = 0x29，Space​ = 0x2C，Backspace​ = 0x2A
F1–F12​ = 0x3A–0x45，F13–F24​ = 0x68–0x73
方向键：Left 0x50 / Right 0x4F / Down 0x51 / Up 0x52
Modifier 不在普通键字节里发，而是写在 Byte0 的对应 bit；但单个 Modifier 的 Usage ID 就是 0xE0–0xE7
完整规范见 USB-IF HID Usage Tables §10 Keyboard/Keypad Page (0x07))");

QString strUrl("https://onlinedocs.microchip.com/oxy/GUID-49CD424A-D8EB-4F60-95E5-12E07036AA34-en-US-4/GUID-70C4159D-8412-4C45-A6F8-9824A327EF6E.html");
QString strDefine(R"(

#define HID_KEYID_NOT_FOUND (-1)

#define HID_KEY_NONE 0x00

#define HID_ERROR_ROLLOVER 0x01

#define HID_POST_FAIL 0x02

#define HID_ERROR_UNDEFINED 0x03

#define HID_A 0x04

#define HID_B 0x05

#define HID_C 0x06

#define HID_D 0x07

#define HID_E 0x08

#define HID_F 0x09

#define HID_G 0x0A

#define HID_H 0x0B

#define HID_I 0x0C

#define HID_J 0x0D

#define HID_K 0x0E

#define HID_L 0x0F

#define HID_M 0x10

#define HID_N 0x11

#define HID_O 0x12

#define HID_P 0x13

#define HID_Q 0x14

#define HID_R 0x15

#define HID_S 0x16

#define HID_T 0x17

#define HID_U 0x18

#define HID_V 0x19

#define HID_W 0x1A

#define HID_X 0x1B

#define HID_Y 0x1C

#define HID_Z 0x1D

#define HID_1 0x1E

#define HID_2 0x1F

#define HID_3 0x20

#define HID_4 0x21

#define HID_5 0x22

#define HID_6 0x23

#define HID_7 0x24

#define HID_8 0x25

#define HID_9 0x26

#define HID_0 0x27

#define HID_RETURN 0x28

#define HID_ESCAPE 0x29

#define HID_BACKSPACE 0x2A

#define HID_TAB 0x2B

#define HID_SPACEBAR 0x2C

#define HID_UNDERSCORE 0x2D

#define HID_EQUAL 0x2E

#define HID_OPEN_BRACE 0x2F

#define HID_CLOSE_BRACE 0x30

#define HID_BACKSLASH 0x31

#define HID_HASH_TILDE 0x32

#define HID_SEMICOLON 0x33

#define HID_APOSTROPHE 0x34

#define HID_GRAVE_TILDE 0x35

#define HID_COMMA 0x36

#define HID_DOT 0x37

#define HID_SLASH 0x38

#define HID_CAPS_LOCK 0x39

#define HID_F1 0x3A

#define HID_F2 0x3B

#define HID_F3 0x3C

#define HID_F4 0x3D

#define HID_F5 0x3E

#define HID_F6 0x3F

#define HID_F7 0x40

#define HID_F8 0x41

#define HID_F9 0x42

#define HID_F10 0x43

#define HID_F11 0x44

#define HID_F12 0x45

#define HID_F13 0x68

#define HID_F14 0x69

#define HID_F15 0x6A

#define HID_F16 0x6B

#define HID_F17 0x6C

#define HID_F18 0x6D

#define HID_F19 0x6E

#define HID_F20 0x6F

#define HID_F21 0x70

#define HID_F22 0x71

#define HID_F23 0x72

#define HID_F24 0x73

#define HID_PRINTSCREEN 0x46

#define HID_SCROLL_LOCK 0x47

#define HID_PAUSE 0x48

#define HID_INSERT 0x49

#define HID_HOME 0x4A

#define HID_PAGEUP 0x4B

#define HID_DELETE 0x4C

#define HID_END 0x4D

#define HID_PAGEDOWN 0x4E

#define HID_RIGHT 0x4F

#define HID_LEFT 0x50

#define HID_DOWN 0x51

#define HID_UP 0x52

#define HID_KEYPAD_NUM_LOCK 0x53

#define HID_KEYPAD_SLASH 0x54

#define HID_KEYPAD_ASTERISK 0x55

#define HID_KEYPAD_MINUS 0x56

#define HID_KEYPAD_PLUS 0x57

#define HID_KEYPAD_ENTER 0x58

#define HID_KEYPAD_1 0x59

#define HID_KEYPAD_2 0x5A

#define HID_KEYPAD_3 0x5B

#define HID_KEYPAD_4 0x5C

#define HID_KEYPAD_5 0x5D

#define HID_KEYPAD_6 0x5E

#define HID_KEYPAD_7 0x5F

#define HID_KEYPAD_8 0x60

#define HID_KEYPAD_9 0x61

#define HID_KEYPAD_0 0x62

#define HID_KEYPAD_DOT 0x63

#define HID_KEYPAD_EQUAL 0x67

#define HID_KEYPAD_COMMA 0x85

#define HID_KEYPAD_EQUALSIGN 0x86

#define HID_KEYPAD_00 0xB0

#define HID_KEYPAD_000 0xB1

#define HID_KEYPAD_LEFT_PARENTHESIS 0xB6

#define HID_KEYPAD_RIGHT_PARENTHESIS 0xB7

#define HID_KEYPAD_OPEN_BRACE 0xB8

#define HID_KEYPAD_CLOSE_BRACE 0xB9

#define HID_KEYPAD_TAB 0xBA

#define HID_KEYPAD_BACKSPACE 0xBB

#define HID_KEYPAD_A 0xBC

#define HID_KEYPAD_B 0xBD

#define HID_KEYPAD_C 0xBE

#define HID_KEYPAD_D 0xBF

#define HID_KEYPAD_E 0xC0

#define HID_KEYPAD_F 0xC1

#define HID_KEYPAD_XOR 0xC2

#define HID_KEYPAD_POWER_TO 0xC3

#define HID_KEYPAD_PERCENT 0xC4

#define HID_KEYPAD_LEFT_ANGLE_BRACE 0xC5

#define HID_KEYPAD_RIGHT_ANGLE_BRACE 0xC6

#define HID_KEYPAD_AND 0xC7

#define HID_KEYPAD_AND_AND 0xC8

#define HID_KEYPAD_OR 0xC9

#define HID_KEYPAD_OR_OR 0xCA

#define HID_KEYPAD_COLON 0xCB

#define HID_KEYPAD_HASH 0xCC

#define HID_KEYPAD_SPACE 0xCD

#define HID_KEYPAD_AT 0xCE

#define HID_KEYPAD_EXCLAMATION 0xCF

#define HID_KEYPAD_MEM_STORE 0xD0

#define HID_KEYPAD_MEM_RECALL 0xD1

#define HID_KEYPAD_MEM_CLEAR 0xD2

#define HID_KEYPAD_MEM_ADD 0xD3

#define HID_KEYPAD_MEM_SUBTRACT 0xD4

#define HID_KEYPAD_MEM_MULTIPLY 0xD5

#define HID_KEYPAD_MEM_DIVIDE 0xD6

#define HID_KEYPAD_PLUS_MINUS 0xD7

#define HID_KEYPAD_CLEAR 0xD8

#define HID_KEYPAD_CLEAR_ENTRY 0xD9

#define HID_KEYPAD_BINARY 0xDA

#define HID_KEYPAD_OCTAL 0xDB

#define HID_KEYPAD_DECIMAL 0xDC

#define HID_KEYPAD_HEXADECIMAL 0xDD

#define HID_AT102 0x64

#define HID_APPLICATION 0x65

#define HID_POWER 0x66

#define HID_EXECUTE 0x74

#define HID_HELP 0x75

#define HID_MENU 0x76

#define HID_SELECT 0x77

#define HID_STOP 0x78

#define HID_AGAIN 0x79

#define HID_UNDO 0x7A

#define HID_CUT 0x7B

#define HID_COPY 0x7C

#define HID_PASTE 0x7D

#define HID_FIND 0x7E

#define HID_MUTE 0x7F

#define HID_VOLUME_UP 0x80

#define HID_VOLUME_DOWN 0x81

#define HID_LOCK_CAPS_LOCK 0x82

#define HID_LOCK_NUM_LOCK 0x83

#define HID_LOCK_SCROLL_LOCK 0x84

#define HID_INTERNATIONAL_1 0x87

#define HID_INTERNATIONAL_2 0x88

#define HID_INTERNATIONAL_3 0x89

#define HID_INTERNATIONAL_4 0x8A

#define HID_INTERNATIONAL_5 0x8B

#define HID_INTERNATIONAL_6 0x8C

#define HID_INTERNATIONAL_7 0x8D

#define HID_INTERNATIONAL_8 0x8E

#define HID_INTERNATIONAL_9 0x8F

#define HID_LANG_1 0x90

#define HID_LANG_2 0x91

#define HID_LANG_3 0x92

#define HID_LANG_4 0x93

#define HID_LANG_5 0x94

#define HID_LANG_6 0x95

#define HID_LANG_7 0x96

#define HID_LANG_8 0x97

#define HID_LANG_9 0x98

#define HID_KB_ALT_ERASE 0x99

#define HID_KB_SYSREQ 0x9A

#define HID_KB_CANCEL 0x9B

#define HID_KB_CLEAR 0x9C

#define HID_KB_PRIOR 0x9D

#define HID_KB_RETURN 0x9E

#define HID_KB_SEPARATOR 0x9F

#define HID_KB_OUT 0xA0

#define HID_KB_OPER 0xA1

#define HID_KB_CLEAR_AGAIN 0xA2

#define HID_KB_CRSEL 0xA3

#define HID_KB_EXSEL 0xA4

#define HID_1K_SEPARATOR 0xB2

#define HID_DECIMAL_SEPARATOR 0xB3

#define HID_CURRENCY_UNIT 0xB4

#define HID_CURRENCY_SUB_UNIT 0xB5

#define HID_LEFT_CTRL 0xE0

#define HID_LEFT_SHIFT 0xE1

#define HID_LEFT_ALT 0xE2

#define HID_LEFT_GUI 0xE3

#define HID_RIGHT_CTRL 0xE4

#define HID_RIGHT_SHIFT 0xE5

#define HID_RIGHT_ALT 0xE6

#define HID_RIGHT_GUI 0xE7

#define HID_MEDIA_PLAYPAUSE 0xE8

#define HID_MEDIA_STOPCD 0xE9

#define HID_MEDIA_PREVIOUSSONG 0xEA

#define HID_MEDIA_NEXTSONG 0xEB

#define HID_MEDIA_EJECTCD 0xEC

#define HID_MEDIA_VOLUMEUP 0xED

#define HID_MEDIA_VOLUMEDOWN 0xEE

#define HID_MEDIA_MUTE 0xEF

#define HID_MEDIA_WWW 0xF0

#define HID_MEDIA_BACK 0xF1

#define HID_MEDIA_FORWARD 0xF2

#define HID_MEDIA_STOP 0xF3

#define HID_MEDIA_FIND 0xF4

#define HID_MEDIA_SCROLLUP 0xF5

#define HID_MEDIA_SCROLLDOWN 0xF6

#define HID_MEDIA_EDIT 0xF7

#define HID_MEDIA_SLEEP 0xF8

#define HID_MEDIA_COFFEE 0xF9

#define HID_MEDIA_REFRESH 0xFA

#define HID_MEDIA_CALC 0xFB
)");

DialogHidReview::DialogHidReview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogHidReview)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);
    ui->textEdit->setText(strPage0);

    connect(ui->radioButton_1,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage0);
    });

    connect(ui->radioButton_2,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage1);
    });

    connect(ui->radioButton_3,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage2);
    });

    connect(ui->radioButton_4,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage3);
    });

    connect(ui->radioButton_5,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage4);
    });

    connect(ui->radioButton_6,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage5 + QString("\n\n\n\n") + strUrl + strDefine);
    });
    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();
    });

}

DialogHidReview::~DialogHidReview()
{
    delete ui;
}
