#ifndef FRAMEPRINTCONTROL_H
#define FRAMEPRINTCONTROL_H

#include "DialogRecList.h"
#include "DialogReferId.h"
#include "DialogFieldPickup.h"

#include <QFrame>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QJsonArray>

#include <QSettings>
#include <QTimer>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

class HttpHandler : public QObject
{
    Q_OBJECT
public:
    explicit HttpHandler(QObject *parent = nullptr) : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
    }

    void get(const QString &url)
    {
        QNetworkRequest request((QUrl(url)));
        QNetworkReply *reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [=]{
            handleReply(reply);
        });
    }

    void post(const QString &url, const QByteArray &data)
    {
        QNetworkRequest request((QUrl(url)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray("application/json"));
        QNetworkReply *reply = manager->post(request, data);
        connect(reply, &QNetworkReply::finished, this, [=]{
            handleReply(reply);
        });
    }

private:
    void handleReply(QNetworkReply *reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            QString error = reply->errorString();
            qDebug() << "HttpHandler Error:" << error;
        }
        else
        {
            int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QString text = reply->readAll();
            onHttpReturn(text,code);
        }
        reply->deleteLater();
    }

signals:
    void onHttpReturn(const QString&text,int code);
    void onHttpWrror(const QString&error);

private:
    QNetworkAccessManager *manager=nullptr;
};

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
    DialogRecList *m_recList = nullptr;
    DialogReferId *m_referId = nullptr;
    DialogFieldPickup *m_fieldList = nullptr;
    void ShowSN();

    QString m_strTemplFile;
    void LoadTemplate(const QString&strFile);

    FrameLabelView *m_pLabelView = nullptr;

    int m_getType = 0;
    HttpHandler *m_http = nullptr;

    QJsonArray m_jArr;
    int m_nSelected = -1;
    QStandardItemModel *m_pModel = nullptr;
};

#endif // FRAMEPRINTCONTROL_H
