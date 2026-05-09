#ifndef DIALOGFIELDPICKUP_H
#define DIALOGFIELDPICKUP_H

#include <QDialog>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>

namespace Ui {
class DialogFieldPickup;
}

class DialogFieldPickup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFieldPickup(QWidget *parent = nullptr);
    ~DialogFieldPickup();
    bool getChecked(int index=0);

private:
    Ui::DialogFieldPickup *ui;
    QStandardItemModel *m_pModel = nullptr;
};

#endif // DIALOGFIELDPICKUP_H
