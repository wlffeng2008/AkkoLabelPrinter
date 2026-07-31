#ifndef DIALOGLABELEDIT_H
#define DIALOGLABELEDIT_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogLabelEdit;
}

class FrameLabelView;

class DialogLabelEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLabelEdit(QWidget *parent = nullptr);
    ~DialogLabelEdit();
    FrameLabelView *m_pView=nullptr;
private:
    Ui::DialogLabelEdit *ui;
    QStandardItemModel *m_pModel = nullptr;
};

#endif // DIALOGLABELEDIT_H
