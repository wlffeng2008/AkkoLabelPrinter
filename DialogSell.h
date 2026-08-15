#ifndef DIALOGSELL_H
#define DIALOGSELL_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
class DialogSell;
}

class DialogSell : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSell(QWidget *parent = nullptr);
    ~DialogSell();

    QStringList getItemData(int item);

signals:
    void itemChanged(int index);

private:
    Ui::DialogSell *ui;
    QStandardItemModel *m_model=nullptr;

    QString m_strFile;
    void saveLoadData(bool save=true);
    int  m_nCurItem=-1;
    bool m_bLoading=false;
    void AddRow(const QString&val0,const QString&val1,const QString&val2,const QString&val3,const QString&val4,const QString&val5,const QString&val6,const QString&val7,const QString&val8);
};

#endif // DIALOGSELL_H
