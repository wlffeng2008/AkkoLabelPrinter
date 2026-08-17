QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LabelPrinter
#程序图标
RC_ICONS = AkkoLabelPrinter.ico

#程序版本
VERSION = 2026.05.20


QMAKE_TARGET_COMPANY = "Home Office: Levi.Wei"
#产品名称
QMAKE_TARGET_PRODUCT = LabelPrinter
#版权所有
QMAKE_TARGET_COPYRIGHT = "微信: 13537556177"
#文件说明
QMAKE_TARGET_DESCRIPTION = "LabelPrinter By QT5.15.2"

RC_LANG = 0x0004

QMAKE_PROJECT_DEPTH = 0

CONFIG += c++17

LIBS += -L$$PWD -lzint
#LIBS += $$PWD/libzint-static.a

DESTDIR = ../../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CLabelSave.cpp \
    CustomItems.cpp \
    DialogFieldPickup.cpp \
    DialogLabelEdit.cpp \
    DialogRecList.cpp \
    DialogReferId.cpp \
    DialogReject.cpp \
    DialogSell.cpp \
    DialogSet.cpp \
    FrameLabelView.cpp \
    FramePrintControl.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CLabelSave.h \
    CustomItems.h \
    DialogFieldPickup.h \
    DialogLabelEdit.h \
    DialogRecList.h \
    DialogReferId.h \
    DialogReject.h \
    DialogSell.h \
    DialogSet.h \
    FrameLabelView.h \
    FramePrintControl.h \
    mainwindow.h

FORMS += \
    DialogFieldPickup.ui \
    DialogLabelEdit.ui \
    DialogRecList.ui \
    DialogReferId.ui \
    DialogReject.ui \
    DialogSell.ui \
    DialogSet.ui \
    FrameLabelView.ui \
    FramePrintControl.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
