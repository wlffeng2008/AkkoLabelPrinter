#ifndef DIALOGKEYBOARD_H
#define DIALOGKEYBOARD_H

#include <QDialog>
#include <QGraphicsScene>

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QPainter>

#include "FrameLabelView.h"

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
