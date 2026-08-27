#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication a(argc, argv);
    QString strStyle=R"(

        QPushButton:focus{background-color: #C0B0BF;}
        QPushButton:hover{background-color: #B0B0BF;}
        QPushButton {background-color: #3CBEEF; border-radius: 6px; color:white; border:1px solid gray;max-width: 360px; min-width: 52px; min-height: 26px; padding-left:2px;padding-right:2px;}
        QLineEdit {border: 1px solid gray; border-radius: 4px; }

        QTableView{ border: 1px solid gray; background-color: rgb(226, 240, 255) ; gridline-color: gray;}
        QTableView::Item{padding-left:2px; background-color: rgb(226, 240, 255) ; border-top: 0px solid gray; border-bottom: 1px solid transparent;border-right: 0px solid gray;}
        QTableView::Item::selected{ background-color: #a0bb9e ; color:white; }
        QTableView QTableCornerButton::section { background-color: skyblue ; min-width: 32px; border-top: 0px solid gray; border-bottom: 1px solid gray; border-left: 0px solid gray; border-right: 1px solid gray; }

        QHeaderView::section{ background-color:skyblue;}
        QHeaderView::section:horizontal{ padding-left: 2px; border-top: 0px solid gray; border-bottom: 1px solid gray; border-right: 1px solid gray; font-weight: bold;}
        QHeaderView::section:vertical{ padding-left: 2px; text-align: right; min-width: 36px; border-top: 0px solid gray; border-bottom: 1px solid gray; border-left: 0px solid gray; border-right: 1px solid gray;}

        QTableView::indicator { width: 18px; height: 18px; }
        QTableView::indicator:checked { image: url(:/images/BoxChecked.png); }
        QTableView::indicator:unchecked { image: url(:/images/BoxUncheck.png); }

        QMessageBox {min-width: 400px; min-height: 300px;}
        QMessageBox QLabel#qt_msgbox_label{max-width: 300px; min-width: 300px;min-height: 120px; qproperty-alignment: AlignLeft;}
        QMessageBox QLabel#qt_msgboxex_icon_label{ min-width: 50px; min-height: 80px; qproperty-alignment: AlignTop;}
        QMessageBox QPushButton {background-color: #303CCF; border-radius: 4px; color:white; min-width: 80px;min-height: 24px;}

    )";

    a.setStyleSheet(strStyle );

    QLocale::setDefault( QLocale(QLocale::Chinese, QLocale::China) );
    QTranslator translatorA;
    QTranslator translatorB;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    translatorA.load("qt_zh_CN.qm",     QLibraryInfo::path(QLibraryInfo::TranslationsPath));
    translatorB.load("qtbase_zh_CN.qm", QLibraryInfo::path(QLibraryInfo::TranslationsPath));
#else
    translatorA.load("qt_zh_TW.qm",     QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    translatorB.load("qtbase_zh_TW.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    a.installTranslator(&translatorA);
    a.installTranslator(&translatorB);
    MainWindow w;
    w.show();
    return a.exec();
}
