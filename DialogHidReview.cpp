#include "DialogHidReview.h"
#include "qglobal.h"
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
0x34,52,Keyboard ' / "
0x35,53,Keyboard ` / ~
0x36,54,Keyboard , / <
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
#define HID_KEYID_NOT_FOUND            0xFF  // -1
#define HID_KEY_NONE                   0x00  //  0
#define HID_ERROR_ROLLOVER             0x01  //  1
#define HID_POST_FAIL                  0x02  //  2
#define HID_ERROR_UNDEFINED            0x03  //  3
#define HID_A                          0x04  //  4
#define HID_B                          0x05  //  5
#define HID_C                          0x06  //  6
#define HID_D                          0x07  //  7
#define HID_E                          0x08  //  8
#define HID_F                          0x09  //  9
#define HID_G                          0x0A  // 10
#define HID_H                          0x0B  // 11
#define HID_I                          0x0C  // 12
#define HID_J                          0x0D  // 13
#define HID_K                          0x0E  // 14
#define HID_L                          0x0F  // 15
#define HID_M                          0x10  // 16
#define HID_N                          0x11  // 17
#define HID_O                          0x12  // 18
#define HID_P                          0x13  // 19
#define HID_Q                          0x14  // 20
#define HID_R                          0x15  // 21
#define HID_S                          0x16  // 22
#define HID_T                          0x17  // 23
#define HID_U                          0x18  // 24
#define HID_V                          0x19  // 25
#define HID_W                          0x1A  // 26
#define HID_X                          0x1B  // 27
#define HID_Y                          0x1C  // 28
#define HID_Z                          0x1D  // 29
#define HID_1                          0x1E  // 30
#define HID_2                          0x1F  // 31
#define HID_3                          0x20  // 32
#define HID_4                          0x21  // 33
#define HID_5                          0x22  // 34
#define HID_6                          0x23  // 35
#define HID_7                          0x24  // 36
#define HID_8                          0x25  // 37
#define HID_9                          0x26  // 38
#define HID_0                          0x27  // 39
#define HID_RETURN                     0x28  // 40
#define HID_ESCAPE                     0x29  // 41
#define HID_BACKSPACE                  0x2A  // 42
#define HID_TAB                        0x2B  // 43
#define HID_SPACEBAR                   0x2C  // 44
#define HID_UNDERSCORE                 0x2D  // 45
#define HID_EQUAL                      0x2E  // 46
#define HID_OPEN_BRACE                 0x2F  // 47
#define HID_CLOSE_BRACE                0x30  // 48
#define HID_BACKSLASH                  0x31  // 49
#define HID_HASH_TILDE                 0x32  // 50
#define HID_SEMICOLON                  0x33  // 51
#define HID_APOSTROPHE                 0x34  // 52
#define HID_GRAVE_TILDE                0x35  // 53
#define HID_COMMA                      0x36  // 54
#define HID_DOT                        0x37  // 55
#define HID_SLASH                      0x38  // 56
#define HID_CAPS_LOCK                  0x39  // 57
#define HID_F1                         0x3A  // 58
#define HID_F2                         0x3B  // 59
#define HID_F3                         0x3C  // 60
#define HID_F4                         0x3D  // 61
#define HID_F5                         0x3E  // 62
#define HID_F6                         0x3F  // 63
#define HID_F7                         0x40  // 64
#define HID_F8                         0x41  // 65
#define HID_F9                         0x42  // 66
#define HID_F10                        0x43  // 67
#define HID_F11                        0x44  // 68
#define HID_F12                        0x45  // 69
#define HID_F13                        0x68  //104
#define HID_F14                        0x69  //105
#define HID_F15                        0x6A  //106
#define HID_F16                        0x6B  //107
#define HID_F17                        0x6C  //108
#define HID_F18                        0x6D  //109
#define HID_F19                        0x6E  //110
#define HID_F20                        0x6F  //111
#define HID_F21                        0x70  //112
#define HID_F22                        0x71  //113
#define HID_F23                        0x72  //114
#define HID_F24                        0x73  //115
#define HID_PRINTSCREEN                0x46  // 70
#define HID_SCROLL_LOCK                0x47  // 71
#define HID_PAUSE                      0x48  // 72
#define HID_INSERT                     0x49  // 73
#define HID_HOME                       0x4A  // 74
#define HID_PAGEUP                     0x4B  // 75
#define HID_DELETE                     0x4C  // 76
#define HID_END                        0x4D  // 77
#define HID_PAGEDOWN                   0x4E  // 78
#define HID_RIGHT                      0x4F  // 79
#define HID_LEFT                       0x50  // 80
#define HID_DOWN                       0x51  // 81
#define HID_UP                         0x52  // 82
#define HID_KEYPAD_NUM_LOCK            0x53  // 83
#define HID_KEYPAD_SLASH               0x54  // 84
#define HID_KEYPAD_ASTERISK            0x55  // 85
#define HID_KEYPAD_MINUS               0x56  // 86
#define HID_KEYPAD_PLUS                0x57  // 87
#define HID_KEYPAD_ENTER               0x58  // 88
#define HID_KEYPAD_1                   0x59  // 89
#define HID_KEYPAD_2                   0x5A  // 90
#define HID_KEYPAD_3                   0x5B  // 91
#define HID_KEYPAD_4                   0x5C  // 92
#define HID_KEYPAD_5                   0x5D  // 93
#define HID_KEYPAD_6                   0x5E  // 94
#define HID_KEYPAD_7                   0x5F  // 95
#define HID_KEYPAD_8                   0x60  // 96
#define HID_KEYPAD_9                   0x61  // 97
#define HID_KEYPAD_0                   0x62  // 98
#define HID_KEYPAD_DOT                 0x63  // 99
#define HID_KEYPAD_EQUAL               0x67  //103
#define HID_KEYPAD_COMMA               0x85  //133
#define HID_KEYPAD_EQUALSIGN           0x86  //134
#define HID_KEYPAD_00                  0xB0  //176
#define HID_KEYPAD_000                 0xB1  //177
#define HID_KEYPAD_LEFT_PARENTHESIS    0xB6  //182
#define HID_KEYPAD_RIGHT_PARENTHESIS   0xB7  //183
#define HID_KEYPAD_OPEN_BRACE          0xB8  //184
#define HID_KEYPAD_CLOSE_BRACE         0xB9  //185
#define HID_KEYPAD_TAB                 0xBA  //186
#define HID_KEYPAD_BACKSPACE           0xBB  //187
#define HID_KEYPAD_A                   0xBC  //188
#define HID_KEYPAD_B                   0xBD  //189
#define HID_KEYPAD_C                   0xBE  //190
#define HID_KEYPAD_D                   0xBF  //191
#define HID_KEYPAD_E                   0xC0  //192
#define HID_KEYPAD_F                   0xC1  //193
#define HID_KEYPAD_XOR                 0xC2  //194
#define HID_KEYPAD_POWER_TO            0xC3  //195
#define HID_KEYPAD_PERCENT             0xC4  //196
#define HID_KEYPAD_LEFT_ANGLE_BRACE    0xC5  //197
#define HID_KEYPAD_RIGHT_ANGLE_BRACE   0xC6  //198
#define HID_KEYPAD_AND                 0xC7  //199
#define HID_KEYPAD_AND_AND             0xC8  //200
#define HID_KEYPAD_OR                  0xC9  //201
#define HID_KEYPAD_OR_OR               0xCA  //202
#define HID_KEYPAD_COLON               0xCB  //203
#define HID_KEYPAD_HASH                0xCC  //204
#define HID_KEYPAD_SPACE               0xCD  //205
#define HID_KEYPAD_AT                  0xCE  //206
#define HID_KEYPAD_EXCLAMATION         0xCF  //207
#define HID_KEYPAD_MEM_STORE           0xD0  //208
#define HID_KEYPAD_MEM_RECALL          0xD1  //209
#define HID_KEYPAD_MEM_CLEAR           0xD2  //210
#define HID_KEYPAD_MEM_ADD             0xD3  //211
#define HID_KEYPAD_MEM_SUBTRACT        0xD4  //212
#define HID_KEYPAD_MEM_MULTIPLY        0xD5  //213
#define HID_KEYPAD_MEM_DIVIDE          0xD6  //214
#define HID_KEYPAD_PLUS_MINUS          0xD7  //215
#define HID_KEYPAD_CLEAR               0xD8  //216
#define HID_KEYPAD_CLEAR_ENTRY         0xD9  //217
#define HID_KEYPAD_BINARY              0xDA  //218
#define HID_KEYPAD_OCTAL               0xDB  //219
#define HID_KEYPAD_DECIMAL             0xDC  //220
#define HID_KEYPAD_HEXADECIMAL         0xDD  //221
#define HID_AT102                      0x64  //100
#define HID_APPLICATION                0x65  //101
#define HID_POWER                      0x66  //102
#define HID_EXECUTE                    0x74  //116
#define HID_HELP                       0x75  //117
#define HID_MENU                       0x76  //118
#define HID_SELECT                     0x77  //119
#define HID_STOP                       0x78  //120
#define HID_AGAIN                      0x79  //121
#define HID_UNDO                       0x7A  //122
#define HID_CUT                        0x7B  //123
#define HID_COPY                       0x7C  //124
#define HID_PASTE                      0x7D  //125
#define HID_FIND                       0x7E  //126
#define HID_MUTE                       0x7F  //127
#define HID_VOLUME_UP                  0x80  //128
#define HID_VOLUME_DOWN                0x81  //129
#define HID_LOCK_CAPS_LOCK             0x82  //130
#define HID_LOCK_NUM_LOCK              0x83  //131
#define HID_LOCK_SCROLL_LOCK           0x84  //132
#define HID_INTERNATIONAL_1            0x87  //135
#define HID_INTERNATIONAL_2            0x88  //136
#define HID_INTERNATIONAL_3            0x89  //137
#define HID_INTERNATIONAL_4            0x8A  //138
#define HID_INTERNATIONAL_5            0x8B  //139
#define HID_INTERNATIONAL_6            0x8C  //140
#define HID_INTERNATIONAL_7            0x8D  //141
#define HID_INTERNATIONAL_8            0x8E  //142
#define HID_INTERNATIONAL_9            0x8F  //143
#define HID_LANG_1                     0x90  //144
#define HID_LANG_2                     0x91  //145
#define HID_LANG_3                     0x92  //146
#define HID_LANG_4                     0x93  //147
#define HID_LANG_5                     0x94  //148
#define HID_LANG_6                     0x95  //149
#define HID_LANG_7                     0x96  //150
#define HID_LANG_8                     0x97  //151
#define HID_LANG_9                     0x98  //152
#define HID_KB_ALT_ERASE               0x99  //153
#define HID_KB_SYSREQ                  0x9A  //154
#define HID_KB_CANCEL                  0x9B  //155
#define HID_KB_CLEAR                   0x9C  //156
#define HID_KB_PRIOR                   0x9D  //157
#define HID_KB_RETURN                  0x9E  //158
#define HID_KB_SEPARATOR               0x9F  //159
#define HID_KB_OUT                     0xA0  //160
#define HID_KB_OPER                    0xA1  //161
#define HID_KB_CLEAR_AGAIN             0xA2  //162
#define HID_KB_CRSEL                   0xA3  //163
#define HID_KB_EXSEL                   0xA4  //164
#define HID_1K_SEPARATOR               0xB2  //178
#define HID_DECIMAL_SEPARATOR          0xB3  //179
#define HID_CURRENCY_UNIT              0xB4  //180
#define HID_CURRENCY_SUB_UNIT          0xB5  //181
#define HID_LEFT_CTRL                  0xE0  //224
#define HID_LEFT_SHIFT                 0xE1  //225
#define HID_LEFT_ALT                   0xE2  //226
#define HID_LEFT_GUI                   0xE3  //227
#define HID_RIGHT_CTRL                 0xE4  //228
#define HID_RIGHT_SHIFT                0xE5  //229
#define HID_RIGHT_ALT                  0xE6  //230
#define HID_RIGHT_GUI                  0xE7  //231
#define HID_MEDIA_PLAYPAUSE            0xE8  //232
#define HID_MEDIA_STOPCD               0xE9  //233
#define HID_MEDIA_PREVIOUSSONG         0xEA  //234
#define HID_MEDIA_NEXTSONG             0xEB  //235
#define HID_MEDIA_EJECTCD              0xEC  //236
#define HID_MEDIA_VOLUMEUP             0xED  //237
#define HID_MEDIA_VOLUMEDOWN           0xEE  //238
#define HID_MEDIA_MUTE                 0xEF  //239
#define HID_MEDIA_WWW                  0xF0  //240
#define HID_MEDIA_BACK                 0xF1  //241
#define HID_MEDIA_FORWARD              0xF2  //242
#define HID_MEDIA_STOP                 0xF3  //243
#define HID_MEDIA_FIND                 0xF4  //244
#define HID_MEDIA_SCROLLUP             0xF5  //245
#define HID_MEDIA_SCROLLDOWN           0xF6  //246
#define HID_MEDIA_EDIT                 0xF7  //247
#define HID_MEDIA_SLEEP                0xF8  //248
#define HID_MEDIA_COFFEE               0xF9  //249
#define HID_MEDIA_REFRESH              0xFA  //250
#define HID_MEDIA_CALC                 0xFB  //251
)");

QString strPage6(R"(
键盘按键 HID / VK 码完整对照表
按键名称          HID码(HEX) HID码(DEC) VK码(HEX) VK码(HEX)

字母与数字键
A                 0x04	4	0x41	65
B                 0x05	5	0x42	66
C                 0x06	6	0x43	67
D                 0x07	7	0x44	68
E                 0x08	8	0x45	69
F                 0x09	9	0x46	70
G                 0x0A	10	0x47	71
H                 0x0B	11	0x48	72
I                 0x0C	12	0x49	73
J                 0x0D	13	0x4A	74
K                 0x0E	14	0x4B	75
L                 0x0F	15	0x4C	76
M                 0x10	16	0x4D	77
N                 0x11	17	0x4E	78
O                 0x12	18	0x4F	79
P                 0x13	19	0x50	80
Q                 0x14	20	0x51	81
R                 0x15	21	0x52	82
S                 0x16	22	0x53	83
T                 0x17	23	0x54	84
U                 0x18	24	0x55	85
V                 0x19	25	0x56	86
W                 0x1A	26	0x57	87
X                 0x1B	27	0x58	88
Y                 0x1C	28	0x59	89
Z                 0x1D	29	0x5A	90
数字键 0          0x1E	30	0x30	48
数字键 1          0x1F	31	0x31	49
数字键 2          0x20	32	0x32	50
数字键 3          0x21	33	0x33	51
数字键 4          0x22	34	0x34	52
数字键 5          0x23	35	0x35	53
数字键 6          0x24	36	0x36	54
数字键 7          0x25	37	0x37	55
数字键 8          0x26	38	0x38	56
数字键 9          0x27	39	0x39	57

控制与编辑键
ESC               0x29	41	0x1B	27
Enter             0x28	40	0x0D	13
Backspace         0x2A	42	0x08	8
Tab               0x2B	43	0x09	9
Space             0x2C	44	0x20	32
Caps Lock         0x39	57	0x14	20
Insert            0x49	73	0x2D	45
Delete            0x4C	76	0x2E	46
Home              0x4A	74	0x24	36
End               0x4D	77	0x23	35
Page Up           0x4B	75	0x21	33
Page Down         0x4E	78	0x22	34

方向键与功能键
Up Arrow          0x52	82	0x26	38
Right Arrow       0x4F	79	0x27	39
Down Arrow        0x51	81	0x28	40
Left Arrow        0x50	80	0x25	37
F1                0x3A	58	0x70	112
F2                0x3B	59	0x71	113
F3                0x3C	60	0x72	114
F4                0x3D	61	0x73	115
F5                0x3E	62	0x74	116
F6                0x3F	63	0x75	117
F7                0x40	64	0x76	118
F8                0x41	65	0x77	119
F9                0x42	66	0x78	120
F10               0x43	67	0x79	121
F11               0x44	68	0x7A	122
F12               0x45	69	0x7B	123
Print Screen      0x46	70	0x2C	44
Scroll Lock       0x47	71	0x91	145
Pause/Break       0x48	72	0x13	19

修饰键与系统键
Left Ctrl         0x01	1	0xA2	162
Right Ctrl        0x10	16	0xA3	163
Left Shift        0x02	2	0xA0	160
Right Shift       0x20	32	0xA1	161
Left Alt          0x04	4	0xA4	164
Right Alt         0x40	64	0xA5	165
Left Win          0x08	8	0x5B	91
Right Win         0x80	128	0x5C	92

小键盘区(Numpad)
Num Lock          0x53	83	0x90	144
Num / (除)        0x54	84	0x6F	111
Num * (乘)        0x55	85	0x6A	106
Num - (减)        0x56	86	0x6D	109
Num + (加)        0x57	87	0x6B	107
Num Enter         0x58	88	0x1C	28
Num , (点)        0x63	99	0x6E	110
Num 0             0x62	98	0x60	96
Num 1             0x59	89	0x61	97
Num 2             0x5A	90	0x62	98
Num 3             0x5B	91	0x63	99
Num 4             0x5C	92	0x64	100
Num 5             0x5D	93	0x65	101
Num 6             0x5E	94	0x66	102
Num 7             0x5F	95	0x67	103
Num 8             0x60	96	0x68	104
Num 9             0x61	97	0x69	105

其他常用键
` (反引号)        0x35	53	0x30	48
- (减号)          0x2D	45	0xBD	189
+ (加号)          0x2E	46	0xBB	187
[ (左方括号)      0x2F	47	0xDB	219
] (右方括号)      0x30	48	0xDD	221
\ (反斜杠)        0x31	49	0xDC	220
; (分号)          0x33	51	0xBA	186
' (单引号)        0x34	52	0xDE	222
, (逗号)          0x36	54	0xBC	188
. (句号)          0x37	55	0xBE	190
/ (斜杠)          0x38	56	0xBF	191
Num Pad(102型)    0x64	100	0xE0	224)");

#include <QDebug>
#include <QTimer>

// 1. 定义按键映射结构体
typedef struct {
    uint8_t hid0;       // USB HID 码
    uint8_t hid1;       // USB HID 码
    uint16_t vk0;       // Windows VK 码
    uint16_t vk1;       // Windows VK 码
    const char *name;  // 按键名称
} KeyMap;

// 2. 完整的按键映射表
const KeyMap KEYBOARD_MAP[] = {
    {0x04,  4,0x41, 65,"A"},
    {0x05,  5,0x42, 66,"B"},
    {0x06,  6,0x43, 67,"C"},
    {0x07,  7,0x44, 68,"D"},
    {0x08,  8,0x45, 69,"E"},
    {0x09,  9,0x46, 70,"F"},
    {0x0A, 10,0x47, 71,"G"},
    {0x0B, 11,0x48, 72,"H"},
    {0x0C, 12,0x49, 73,"I"},
    {0x0D, 13,0x4A, 74,"J"},
    {0x0E, 14,0x4B, 75,"K"},
    {0x0F, 15,0x4C, 76,"L"},
    {0x10, 16,0x4D, 77,"M"},
    {0x11, 17,0x4E, 78,"N"},
    {0x12, 18,0x4F, 79,"O"},
    {0x13, 19,0x50, 80,"P"},
    {0x14, 20,0x51, 81,"Q"},
    {0x15, 21,0x52, 82,"R"},
    {0x16, 22,0x53, 83,"S"},
    {0x17, 23,0x54, 84,"T"},
    {0x18, 24,0x55, 85,"U"},
    {0x19, 25,0x56, 86,"V"},
    {0x1A, 26,0x57, 87,"W"},
    {0x1B, 27,0x58, 88,"X"},
    {0x1C, 28,0x59, 89,"Y"},
    {0x1D, 29,0x5A, 90,"Z"},
    {0x1E, 30,0x30, 48,"0"},
    {0x1F, 31,0x31, 49,"1"},
    {0x20, 32,0x32, 50,"2"},
    {0x21, 33,0x33, 51,"3"},
    {0x22, 34,0x34, 52,"4"},
    {0x23, 35,0x35, 53,"5"},
    {0x24, 36,0x36, 54,"6"},
    {0x25, 37,0x37, 55,"7"},
    {0x26, 38,0x38, 56,"8"},
    {0x27, 39,0x39, 57,"9"},
    {0x29, 41,0x1B, 27,"ESC"},
    {0x28, 40,0x0D, 13,"Enter"},
    {0x2A, 42,0x08,  8,"Backspace"},
    {0x2B, 43,0x09,  9,"Tab"},
    {0x2C, 44,0x20, 32,"Space"},
    {0x39, 57,0x14, 20,"Caps Lock"},
    {0x49, 73,0x2D, 45,"Insert"},
    {0x4C, 76,0x2E, 46,"Delete"},
    {0x4A, 74,0x24, 36,"Home"},
    {0x4D, 77,0x23, 35,"End"},
    {0x4B, 75,0x21, 33,"Page Up"},
    {0x4E, 78,0x22, 34,"Page Down"},
    {0x52, 82,0x26, 38,"Up Arrow"},
    {0x4F, 79,0x27, 39,"Right Arrow"},
    {0x51, 81,0x28, 40,"Down Arrow"},
    {0x50, 80,0x25, 37,"Left Arrow"},
    {0x3A, 58,0x70,112,"F1"},
    {0x3B, 59,0x71,113,"F2"},
    {0x3C, 60,0x72,114,"F3"},
    {0x3D, 61,0x73,115,"F4"},
    {0x3E, 62,0x74,116,"F5"},
    {0x3F, 63,0x75,117,"F6"},
    {0x40, 64,0x76,118,"F7"},
    {0x41, 65,0x77,119,"F8"},
    {0x42, 66,0x78,120,"F9"},
    {0x43, 67,0x79,121,"F10"},
    {0x44, 68,0x7A,122,"F11"},
    {0x45, 69,0x7B,123,"F12"},
    {0x46, 70,0x2C, 44,"Print Screen"},
    {0x47, 71,0x91,145,"Scroll Lock"},
    {0x48, 72,0x13, 19,"Pause/Break"},
    {0x01,  1,0xA2,162,"Left Ctrl"},
    {0x10, 16,0xA3,163,"Right Ctrl"},
    {0x02,  2,0xA0,160,"Left Shift"},
    {0x20, 32,0xA1,161,"Right Shift"},
    {0x04,  4,0xA4,164,"Left Alt"},
    {0x40, 64,0xA5,165,"Right Alt"},
    {0x08,  8,0x5B, 91,"Left Win"},
    {0x80,128,0x5C, 92,"Right Win"},
    {0x53, 83,0x90,144,"Num Lock"},
    {0x54, 84,0x6F,111,"Num / (Division)"},
    {0x55, 85,0x6A,106,"Num * (Multiple)"},
    {0x56, 86,0x6D,109,"Num - (Minus)"},
    {0x57, 87,0x6B,107,"Num + (Plus)"},
    {0x58, 88,0x1C, 28,"Num Enter"},
    {0x63, 99,0x6E,110,"Num , (Dot)"},
    {0x62, 98,0x60, 96,"Num 0"},
    {0x59, 89,0x61, 97,"Num 1"},
    {0x5A, 90,0x62, 98,"Num 2"},
    {0x5B, 91,0x63, 99,"Num 3"},
    {0x5C, 92,0x64,100,"Num 4"},
    {0x5D, 93,0x65,101,"Num 5"},
    {0x5E, 94,0x66,102,"Num 6"},
    {0x5F, 95,0x67,103,"Num 7"},
    {0x60, 96,0x68,104,"Num 8"},
    {0x61, 97,0x69,105,"Num 9"},
    {0x35, 53,0x30, 48,"` ~ (Back quote)"},
    {0x2D, 45,0xBD,189," -_ (Minus)"},
    {0x2E, 46,0xBB,187,"+ = (Plus)"},
    {0x2F, 47,0xDB,219,"[ { (Left square bracket)"},
    {0x30, 48,0xDD,221,"] } (Right square bracket)"},
    {0x31, 49,0xDC,220,"\\ | (Backslash)"},
    {0x33, 51,0xBA,186,"; : (Semicolon)"},
    {0x34, 52,0xDE,222,"' \" (Apostrophe)"},
    {0x36, 54,0xBC,188,", < (Comma)"},
    {0x37, 55,0xBE,190,". > (Period)"},
    {0x38, 56,0xBF,191,"/ ? (Slash)"},
    {0x64,100,0xE0,224,"Num Pad(102 Model)"}
};

// 计算数组长度
#define MAP_SIZE (sizeof(KEYBOARD_MAP) / sizeof(KEYBOARD_MAP[0]))

// 3. HID 转 VK 码
uint16_t hid_to_vk(uint8_t hid_code) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].hid0 == hid_code) {
            return KEYBOARD_MAP[i].vk0;
        }
    }
    return 0;
}

// 4. VK 转 HID 码
uint8_t vk_to_hid(uint16_t vk_code) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].vk0 == vk_code) {
            return KEYBOARD_MAP[i].hid0;
        }
    }
    return 0;
}

const char *get_keyname_vk(uint16_t vk_code)
{
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].vk0 == vk_code) {
            return KEYBOARD_MAP[i].name;
        }
    }
    return "";
}

const char *get_keyname_hid(uint16_t hid_code)
{
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].hid0 == hid_code) {
            return KEYBOARD_MAP[i].name;
        }
    }
    return "";
}

QString strCode(R"(
// 1. 定义按键映射结构体
typedef struct {
    uint8_t hid0;       // USB HID 码
    uint8_t hid1;       // USB HID 码
    uint16_t vk0;       // Windows VK 码
    uint16_t vk1;       // Windows VK 码
    const char *name;  // 按键名称
} KeyMap;

// 2. 完整的按键映射表
const KeyMap KEYBOARD_MAP[] = {
    {0x04,  4,0x41, 65,"A"},
    {0x05,  5,0x42, 66,"B"},
    {0x06,  6,0x43, 67,"C"},
    {0x07,  7,0x44, 68,"D"},
    {0x08,  8,0x45, 69,"E"},
    {0x09,  9,0x46, 70,"F"},
    {0x0A, 10,0x47, 71,"G"},
    {0x0B, 11,0x48, 72,"H"},
    {0x0C, 12,0x49, 73,"I"},
    {0x0D, 13,0x4A, 74,"J"},
    {0x0E, 14,0x4B, 75,"K"},
    {0x0F, 15,0x4C, 76,"L"},
    {0x10, 16,0x4D, 77,"M"},
    {0x11, 17,0x4E, 78,"N"},
    {0x12, 18,0x4F, 79,"O"},
    {0x13, 19,0x50, 80,"P"},
    {0x14, 20,0x51, 81,"Q"},
    {0x15, 21,0x52, 82,"R"},
    {0x16, 22,0x53, 83,"S"},
    {0x17, 23,0x54, 84,"T"},
    {0x18, 24,0x55, 85,"U"},
    {0x19, 25,0x56, 86,"V"},
    {0x1A, 26,0x57, 87,"W"},
    {0x1B, 27,0x58, 88,"X"},
    {0x1C, 28,0x59, 89,"Y"},
    {0x1D, 29,0x5A, 90,"Z"},
    {0x1E, 30,0x30, 48,"0"},
    {0x1F, 31,0x31, 49,"1"},
    {0x20, 32,0x32, 50,"2"},
    {0x21, 33,0x33, 51,"3"},
    {0x22, 34,0x34, 52,"4"},
    {0x23, 35,0x35, 53,"5"},
    {0x24, 36,0x36, 54,"6"},
    {0x25, 37,0x37, 55,"7"},
    {0x26, 38,0x38, 56,"8"},
    {0x27, 39,0x39, 57,"9"},
    {0x29, 41,0x1B, 27,"ESC"},
    {0x28, 40,0x0D, 13,"Enter"},
    {0x2A, 42,0x08,  8,"Backspace"},
    {0x2B, 43,0x09,  9,"Tab"},
    {0x2C, 44,0x20, 32,"Space"},
    {0x39, 57,0x14, 20,"Caps Lock"},
    {0x49, 73,0x2D, 45,"Insert"},
    {0x4C, 76,0x2E, 46,"Delete"},
    {0x4A, 74,0x24, 36,"Home"},
    {0x4D, 77,0x23, 35,"End"},
    {0x4B, 75,0x21, 33,"Page Up"},
    {0x4E, 78,0x22, 34,"Page Down"},
    {0x52, 82,0x26, 38,"Up Arrow"},
    {0x4F, 79,0x27, 39,"Right Arrow"},
    {0x51, 81,0x28, 40,"Down Arrow"},
    {0x50, 80,0x25, 37,"Left Arrow"},
    {0x3A, 58,0x70,112,"F1"},
    {0x3B, 59,0x71,113,"F2"},
    {0x3C, 60,0x72,114,"F3"},
    {0x3D, 61,0x73,115,"F4"},
    {0x3E, 62,0x74,116,"F5"},
    {0x3F, 63,0x75,117,"F6"},
    {0x40, 64,0x76,118,"F7"},
    {0x41, 65,0x77,119,"F8"},
    {0x42, 66,0x78,120,"F9"},
    {0x43, 67,0x79,121,"F10"},
    {0x44, 68,0x7A,122,"F11"},
    {0x45, 69,0x7B,123,"F12"},
    {0x46, 70,0x2C, 44,"Print Screen"},
    {0x47, 71,0x91,145,"Scroll Lock"},
    {0x48, 72,0x13, 19,"Pause/Break"},
    {0x01,  1,0xA2,162,"Left Ctrl"},
    {0x10, 16,0xA3,163,"Right Ctrl"},
    {0x02,  2,0xA0,160,"Left Shift"},
    {0x20, 32,0xA1,161,"Right Shift"},
    {0x04,  4,0xA4,164,"Left Alt"},
    {0x40, 64,0xA5,165,"Right Alt"},
    {0x08,  8,0x5B, 91,"Left Win"},
    {0x80,128,0x5C, 92,"Right Win"},
    {0x53, 83,0x90,144,"Num Lock"},
    {0x54, 84,0x6F,111,"Num / (Division)\"},
    {0x55, 85,0x6A,106,"Num * (Multiple)\"},
    {0x56, 86,0x6D,109,"Num - (Minus)\"},
    {0x57, 87,0x6B,107,"Num + (Plus)\"},
    {0x58, 88,0x1C, 28,"Num Enter"},
    {0x63, 99,0x6E,110,"Num , (Dot)\"},
    {0x62, 98,0x60, 96,"Num 0"},
    {0x59, 89,0x61, 97,"Num 1"},
    {0x5A, 90,0x62, 98,"Num 2"},
    {0x5B, 91,0x63, 99,"Num 3"},
    {0x5C, 92,0x64,100,"Num 4"},
    {0x5D, 93,0x65,101,"Num 5"},
    {0x5E, 94,0x66,102,"Num 6"},
    {0x5F, 95,0x67,103,"Num 7"},
    {0x60, 96,0x68,104,"Num 8"},
    {0x61, 97,0x69,105,"Num 9"},
    {0x35, 53,0x30, 48,"` ~ (Back quote)\"},
    {0x2D, 45,0xBD,189," -_ (Minus)\"},
    {0x2E, 46,0xBB,187,"+ = (Plus)\"},
    {0x2F, 47,0xDB,219,"[ { (Left square bracket)\"},
    {0x30, 48,0xDD,221,"] } (Right square bracket)\"},
    {0x31, 49,0xDC,220,"\\ | (Backslash)\"},
    {0x33, 51,0xBA,186,"; : (Semicolon)\"},
    {0x34, 52,0xDE,222,"' \" (Apostrophe)\"},
    {0x36, 54,0xBC,188,", < (Comma)\"},
    {0x37, 55,0xBE,190,". > (Period)\"},
    {0x38, 56,0xBF,191,"/ ? (Slash)\"},
    {0x64,100,0xE0,224,"Num Pad(102 Model)\"}
};

// 计算数组长度
#define MAP_SIZE (sizeof(KEYBOARD_MAP) / sizeof(KEYBOARD_MAP[0]))

// 3. HID 转 VK 码
uint16_t hid_to_vk(uint8_t hid_code) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].hid0 == hid_code) {
            return KEYBOARD_MAP[i].vk0;
        }
    }
    return 0;
}

// 4. VK 转 HID 码
uint8_t vk_to_hid(uint16_t vk_code) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].vk0 == vk_code) {
            return KEYBOARD_MAP[i].hid0;
        }
    }
    return 0;
}

const char *get_keyname_vk(uint16_t vk_code)
{
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].vk0 == vk_code) {
            return KEYBOARD_MAP[i].name;
        }
    }
    return "";
}

const char *get_keyname_hid(uint16_t hid_code)
{
    for (size_t i = 0; i < MAP_SIZE; i++) {
        if (KEYBOARD_MAP[i].hid0 == hid_code) {
            return KEYBOARD_MAP[i].name;
        }
    }
    return "";
}
)");

DialogHidReview::DialogHidReview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogHidReview)
{
    ui->setupUi(this);
    setWindowFlags((windowFlags()|Qt::MSWindowsFixedSizeDialogHint)  & ~Qt::WindowContextHelpButtonHint);
    ui->textEdit->setText(strPage0);

    QTimer::singleShot(100,this,[=]{ui->radioButton_0->click();});
    connect(ui->radioButton_0,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage0);
    });

    connect(ui->radioButton_1,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage1);
    });

    connect(ui->radioButton_2,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage2);
    });

    connect(ui->radioButton_3,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage3);
    });

    connect(ui->radioButton_4,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage4);
    });

    connect(ui->radioButton_5,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strPage5 + QString("\n\n\n") + strUrl+ QString("\n\n\n") + strDefine);
    });

    connect(ui->radioButton_6,&QRadioButton::clicked,this,[=]{
        ui->textEdit->setText(strCode.replace(")\\",")"));
    });
    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        hide();
    });

//     QStringList lines = strPage6.split('\n');
//     foreach(const QString&line,lines)
//     {
//         QString strLine = line.trimmed();
//         if(strLine.isEmpty())
//             continue;
//         QStringList subs = strLine.split('\t');
//         if(subs.count()<3)
//             continue;

//         //qDebug().noquote() << subs;
//         QString strTmp0 = subs[0].trimmed().right(4);
//         QString strTmp1 = subs[0].trimmed().replace(strTmp0,"").trimmed();
//         qDebug().noquote().nospace() <<"{"<< strTmp0.trimmed()<< "," << QString::asprintf("%3d",subs[1].trimmed().toInt()) <<  ","<< subs[2].trimmed() <<  ","<< QString::asprintf("%3d",subs[3].trimmed().toInt()) << ",\"" << strTmp1.trimmed() << "\"},";
//     }
// QStringList lines = strDefine.split('\n');
// foreach(const QString&line,lines)
// {
//     QString strLine = line.trimmed();
//     if(strLine.isEmpty())
//         continue;
//     QStringList subs = strLine.split(' ');
//     if(subs.count()<2)
//         continue;

//     quint8 val = subs[2].toLatin1().toUInt(nullptr,16);

//     qDebug().noquote() << QString::asprintf("#define %-30s 0x%02X  //%3d",subs[1].toStdString().c_str(),val,val);
// }
}

DialogHidReview::~DialogHidReview()
{
    delete ui;
}
