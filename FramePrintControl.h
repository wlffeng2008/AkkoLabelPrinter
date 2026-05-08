#ifndef FRAMEPRINTCONTROL_H
#define FRAMEPRINTCONTROL_H

#include <QFrame>
#include <QSettings>
#include <QTimer>
#include <QMap>

#include "DialogRecList.h"

namespace Ui {
class FramePrintControl;
}

typedef struct
{
    QString strMac ;
    QString strDID ;
    QString strDate ;
    QString strSN ;
    QString strQPass ;
}DataItem;

class FrameLabelView ;

class FramePrintControl : public QFrame
{
    Q_OBJECT

public:
    explicit FramePrintControl(QWidget *parent = nullptr);
    ~FramePrintControl();
    void BindLabelView(FrameLabelView *pView);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_pushButtonPreview_clicked();
    void on_pushButtonPrint_clicked();

private:
    Ui::FramePrintControl *ui;
    QSettings *m_pSet = nullptr;

    int m_nAllCount = 0;
    //QStringList m_SNs;
    DialogRecList *m_recList = nullptr;
    void ShowSN();

    QString m_strTemplFile;
    void LoadTemplate(const QString&strFile);

    FrameLabelView *m_pLabelView = nullptr;
};

#endif // FRAMEPRINTCONTROL_H
