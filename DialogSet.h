#ifndef DIALOGSET_H
#define DIALOGSET_H

#include <QDialog>

namespace Ui {
class DialogSet;
}

class DialogSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSet(QWidget *parent = nullptr);
    ~DialogSet();
    QString m_strCount;
private:
    Ui::DialogSet *ui;
};

#endif // DIALOGSET_H
