#ifndef DIALOGHIDREVIEW_H
#define DIALOGHIDREVIEW_H

//https://onlinedocs.microchip.com/oxy/GUID-49CD424A-D8EB-4F60-95E5-12E07036AA34-en-US-4/GUID-70C4159D-8412-4C45-A6F8-9824A327EF6E.html
#define HID_KEYID_NOT_FOUND            0xFF  //255
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



#include <QDialog>

namespace Ui {
class DialogHidReview;
}

class DialogHidReview : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHidReview(QWidget *parent = nullptr);
    ~DialogHidReview();

private:
    Ui::DialogHidReview *ui;
};

#endif // DIALOGHIDREVIEW_H
