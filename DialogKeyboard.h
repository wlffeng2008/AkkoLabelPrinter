#ifndef DIALOGKEYBOARD_H
#define DIALOGKEYBOARD_H

#include <QDialog>
#include <QStandardItemModel>
#include <QGraphicsScene>

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QPainter>

#include "DialogLabelEdit.h"

namespace Ui {
class DialogKeyboard;
}

class DialogKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKeyboard(QWidget *parent = nullptr);
    ~DialogKeyboard();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::DialogKeyboard *ui;

    bool m_bShiftPressed = false;
    bool m_bCtrlPressed = false;
    bool m_bAltPressed = false;

    CustomScene *m_sence=nullptr;
    QStandardItemModel *m_pModel = nullptr;
    QStandardItemModel *m_pModel2 = nullptr;
    int m_nSelected=-1;
    SideValueDelegate *pDele0;
    SideValueDelegate *pDele1;
    SideValueDelegate *pDele2;
    SideValueDelegate *pDele3;
    SideValueDelegate *pDele4;
    SideValueDelegate *pDele5;
    void UpdateRow(QGraphicsItem *item);
    void MoveGroup(int type,int step=0);
};

#endif // DIALOGKEYBOARD_H
