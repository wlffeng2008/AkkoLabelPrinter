#ifndef DIALOGREFERID_H
#define DIALOGREFERID_H

#include <QDialog>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>

namespace Ui {
class DialogReferId;
}

class DialogReferId : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReferId(QWidget *parent = nullptr);
    ~DialogReferId();

private:
    Ui::DialogReferId *ui;

    QStandardItemModel *m_pModel = nullptr;
};

#endif // DIALOGREFERID_H
