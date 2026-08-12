#ifndef DIALOGREJECT_H
#define DIALOGREJECT_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DialogReject;
}

class DialogReject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReject(QWidget *parent = nullptr);
    ~DialogReject();

    QList<int>getPrintItems();
    QStringList getItemData(int item);

private:
    Ui::DialogReject *ui;
    QStandardItemModel *m_model=nullptr;

    QString m_strFile;
    void saveLoadData(bool save=true);
    int  m_nCurItem=-1;
    bool m_bLoading=false;
    void AddRow(const QString&val0,const QString&val1,const QString&val2,const QString&val3,const QString&val4,const QString&val5,bool toPrint=true);
};

#endif // DIALOGREJECT_H
