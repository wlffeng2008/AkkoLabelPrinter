#ifndef DIALOGRECLIST_H
#define DIALOGRECLIST_H

#include <QDialog>
#include <QStandardItemModel>
#include <QHeaderView>
namespace Ui {
class DialogRecList;
}

class DialogRecList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRecList(QWidget *parent = nullptr);
    ~DialogRecList();
    void appendCode(const QString&text);
    QString toText(int colCount=2);
    bool contains(const QString&text);
    int count();
    void clear();

    void setPath(const QString&path);
    void setBoxnum(const QString&boxnum);

signals:
    void onTextChanged();
private:
    Ui::DialogRecList *ui;

    void saveToFile();

    QString m_strPath=("/output");
    QString m_strboxnum;
    QStringList m_saveList;

    int m_nSelected = -1;
    QStandardItemModel *m_pModel=nullptr;
};

#endif // DIALOGRECLIST_H
