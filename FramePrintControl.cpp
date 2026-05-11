#include "FramePrintControl.h"
#include "ui_FramePrintControl.h"

#include "FrameLabelView.h"
#include "DialogSet.h"

#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QDir>
#include <QFile>
#include <QPrinterInfo>
#include <QDateTime>
#include <QMessageBox>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// 检测是否存在【实体打印机】
bool hasRealPhysicalPrinter()
{
    // 获取系统所有打印机
    QList<QPrinterInfo> printers = QPrinterInfo::availablePrinters();

    // 虚拟打印机关键字（过滤掉）
    QStringList virtualKeywords = {
        "PDF", "XPS", "OneNote", "Microsoft Print", "Fax", "图片传真"
    };

    foreach (QPrinterInfo info, printers) {
        QString name = info.printerName();
        qDebug() << name ;

        // 排除虚拟打印机
        bool isVirtual = false;
        foreach (QString key, virtualKeywords) {
            if (name.contains(key, Qt::CaseInsensitive)) {
                isVirtual = true;
                break;
            }
        }
        if (isVirtual) continue;

        // 剩下的 = 实体打印机
        return true;
    }

    // 没找到实体打印机
    return false;
}

FramePrintControl::FramePrintControl(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FramePrintControl)
{
    ui->setupUi(this);

    m_pSet = new QSettings(QApplication::applicationDirPath() + "/config/global.ini",QSettings::IniFormat);

    m_strTemplFile = m_pSet->value("lastTemplFile", QApplication::applicationDirPath() + "/config/default.tem").toString();

    ui->checkBoxDouble->setChecked(true);


    bool hasPrinter = QPrinterInfo::availablePrinters().size() > 0;

    if (hasPrinter) {
        qDebug() << "系统已连接打印机";
    } else {
        qDebug() << "系统没有任何打印机";
    }
    qDebug() << QPrinterInfo::defaultPrinter();

    if (hasRealPhysicalPrinter()) {
        qDebug() << "✅ 找到实体打印机，可以打印";
    } else {
        qDebug() << "❌ 未连接实体打印机";
    }

    m_recList = new DialogRecList(this);
    m_referId = new DialogReferId(this);
    m_fieldList = new DialogFieldPickup(this);

    ui->lineEditLimit->setText(m_pSet->value("limitCount","40").toString());
    ui->lineEditCode0->setText(m_pSet->value("Code0","BJ").toString());
    ui->lineEditCode1->setText(m_pSet->value("Code1","20260507").toString());
    ui->lineEditCode2->setText(m_pSet->value("Code2","A").toString());
    ui->lineEditCode3->setText(m_pSet->value("Code3","0001").toString());
    ui->lineEditPaperW->setText(m_pSet->value("PaperW","60").toString());
    ui->lineEditPaperH->setText(m_pSet->value("PaperH","40").toString());

    ui->lineEditLenght->setText(m_pSet->value("askLen","16").toString());
    ui->checkBoxCheckLen->setChecked(m_pSet->value("checkLen",true).toBool());
    ui->checkBoxGen128->setChecked(m_pSet->value("gen128",true).toBool());

    connect(ui->pushButtonCreate,&QPushButton::clicked,this,[=]{
        QString strSN = ui->textEditAll->toPlainText().trimmed();
        //strSN.replace("\n\r\n","\n");
        strSN.replace("\n","\r\n");
        m_pLabelView->AddImageQR(strSN,"QRCode");

        QString strCode0 = ui->lineEditCode0->text().trimmed();
        QString strCode1 = ui->lineEditCode1->text().trimmed();
        QString strCode2 = ui->lineEditCode2->text().trimmed();
        QString strCode3 = ui->lineEditCode3->text().trimmed();
        QString strCode = strCode0 + strCode1 + strCode2 + strCode3;
        QString newValue = QString::asprintf("%04d",strCode3.toInt()+1);
        ui->lineEditCode3->setText(newValue);

        bool bGen128 = ui->checkBoxGen128->isChecked();
        if(bGen128)
        {
            m_pLabelView->AddImage128(strCode,"128Code");
            m_pLabelView->AddText(strCode,"128Text");
        }
        else
        {
            m_pLabelView->Remove("128Code");
            m_pLabelView->Remove("128Text");
        }

        m_pSet->setValue("Code0",strCode0);
        m_pSet->setValue("Code1",strCode1);
        m_pSet->setValue("Code2",strCode2);
        m_pSet->setValue("Code3",newValue);
        m_pSet->setValue("gen128",bGen128);
    });

    connect(ui->pushButtonClear,&QPushButton::clicked,this,[=]{
        m_recList->clear();
        ui->labelCurCount->setText("0");
        ShowSN();
    });
    connect(ui->pushButtonEdit,&QPushButton::clicked,this,[=]{
        m_recList->show();
    });
    connect(ui->pushButtonSet,&QPushButton::clicked,this,[=]{
        DialogSet dlg(this);
        dlg.m_strCount = ui->lineEditLimit->text();
        if(dlg.exec() == QDialog::Accepted)
        {
            ui->lineEditLimit->setText(dlg.m_strCount);
            m_pSet->setValue("limitCount",dlg.m_strCount);
        }
    });

    QTimer *pTMGet = new QTimer(this);
    connect(ui->lineEditQrInput,&QLineEdit::textChanged,this,[=]{
        pTMGet->stop();
        pTMGet->start(500);
    });

    connect(m_recList,&DialogRecList::onTextChanged,this,[=]{
        ShowSN();
    });

    connect(pTMGet,&QTimer::timeout,this,[=]{
        pTMGet->stop();
        int askLen = ui->lineEditLenght->text().toInt();
        m_pSet->setValue("askLen",askLen);
        m_pSet->setValue("checkLen",ui->checkBoxCheckLen->isChecked());
        if(!ui->checkBoxCheckLen->isChecked())
            askLen = 4;

        QString strSN = ui->lineEditQrInput->text().trimmed();
        if(strSN.length() >= askLen)
        {
            if(m_recList->contains(strSN))
            {
                QMessageBox::information(this,"温馨提示",QString("此条码 %1 已扫入!").arg(strSN));
                ui->lineEditQrInput->clear();
                ui->lineEditQrInput->setFocus();
                return;
            }

            m_recList->appendCode(strSN);

            m_nAllCount++;
            ui->labelAllCount->setText(QString("%1").arg(m_nAllCount));

            int nCount = m_recList->count();
            ui->labelCurCount->setText(QString("%1").arg(nCount));

            //ShowSN();

            QTimer::singleShot(1000,this,[=]{
                ui->lineEditQrInput->clear();
                ui->lineEditQrInput->setFocus();
                if(nCount >= ui->lineEditLimit->text().trimmed().toInt())
                {
                    if(ui->checkBoxAutoPrint->isChecked())
                    {
                        QTimer::singleShot(100,this,[=]{
                            ui->pushButtonPrint->click();
                        });
                    }

                    if(QMessageBox::question(this,"温馨提示","本次扫码数量已达上限,自动清零吗?") == QMessageBox::Yes)
                    {
                        ui->pushButtonClear->click();
                    }
                }
            });
        }
    });

    connect(ui->checkBoxDouble,&QCheckBox::clicked,this,[=]{
        ShowSN();
    });
    connect(ui->pushButtonSetPaper,&QPushButton::clicked,this,[=]{        
        QString strPaperW = ui->lineEditPaperW->text();
        QString strPaperH = ui->lineEditPaperH->text();
        m_pSet->setValue("PaperW",strPaperW);
        m_pSet->setValue("PaperH",strPaperH);
        m_pLabelView->SetPaperSize(strPaperW.toDouble(),strPaperH.toDouble());
    });

    {
        m_pModel = new QStandardItemModel(this);
        m_pModel->setHorizontalHeaderLabels(QString("扫码内容,状态").split(','));
        ui->tableView->setModel(m_pModel);
        QHeaderView *pHeader = ui->tableView->horizontalHeader();
        pHeader->setSectionResizeMode(QHeaderView::Stretch);
        pHeader->setSectionResizeMode(1,QHeaderView::Fixed);
        pHeader->resizeSection(1,60);

        connect(ui->tableView,&QTableView::clicked,this,[=](const QModelIndex &index){
            m_nSelected = index.row();
            QStandardItem *item0 = m_pModel->item(index.row(),0);
            ui->lineEditBarcode->setText(item0->text());
            ui->pushButtonBarcode->click();
        });

        static int autoGet = 0;
        QTimer *pTMGet = new QTimer(this);
        connect(ui->checkBoxAutoGet,&QCheckBox::checkStateChanged,this,[=](Qt::CheckState state){
            pTMGet->stop();
            autoGet = 0;
            bool checked = ui->checkBoxAutoGet->isChecked();
            if(checked) pTMGet->start(1000);
        });

        connect(pTMGet,&QTimer::timeout,this,[=]{
            int interval = ui->lineEditIntrval->text().toInt();
            if(interval<10) interval=10;
            ui->progressBarGet->setRange(0,interval);
            autoGet++;
            if(autoGet >= interval)
            {
                autoGet = 0;
                ui->pushButtonGet->click();
            }
            ui->progressBarGet->setValue(autoGet);
        });

        QTimer *pTMOut = new QTimer(this);
        connect(pTMOut,&QTimer::timeout,this,[=]{
            if(ui->checkBoxAutoOut->isChecked())
            {
                int count = m_pModel->rowCount();
                for(int i=0; i<count; i++)
                {
                    QStandardItem *item0 = m_pModel->item(i,0);
                    if(item0->data().toInt() == 0)
                    {
                        ui->tableView->setCurrentIndex(item0->index());
                        ui->lineEditBarcode->setText(item0->text());
                        ui->pushButtonBarcode->click();
                        break;
                    }
                }
            }
        });

        connect(ui->pushButtonFileld,&QPushButton::clicked,this,[=]{
            m_fieldList->show();
        });
        connect(ui->pushButtonReferId,&QPushButton::clicked,this,[=]{
            m_referId->show();
        });
        connect(ui->pushButtonBarcode,&QPushButton::clicked,this,[=]{

            QString strCode = ui->lineEditBarcode->text().trimmed();
            m_pLabelView->AddImage128(strCode,"128Code");
            m_pLabelView->AddText(strCode,"128Text");

            //if(!ui->checkBoxOnlyOrder->isChecked())
            {
                QJsonObject jData = m_jArr[m_nSelected].toObject();

                QString strName   = jData["name"].toString();
                QString strValue  = jData["propertiesValue"].toString();
                QString strAddr   = jData["receiverAddress"].toString();
                QString strDate   = jData["orderDate"].toString();
                QString strPDate  = jData["payDate"].toString();
                QString strShop   = jData["shopName"].toString();
                QString strShopId = jData["shopId"].toString();
                QString strInId   = jData["oid"].toString();

                if(m_fieldList->getChecked(0)) m_pLabelView->AddText(QString("店铺名称：")+ strShop,"shopName");         else m_pLabelView->Remove("shopName");
                if(m_fieldList->getChecked(1)) m_pLabelView->AddText(QString("产品名称：")+ strName,"name");             else m_pLabelView->Remove("name");
                if(m_fieldList->getChecked(2)) m_pLabelView->AddText(QString("产品属性：")+ strValue,"propertiesValue"); else m_pLabelView->Remove("propertiesValue");
                if(m_fieldList->getChecked(3)) m_pLabelView->AddText(QString("收货地址：")+ strAddr,"receiverAddress");  else m_pLabelView->Remove("receiverAddress");
                if(m_fieldList->getChecked(4)) m_pLabelView->AddText(QString("订单日期：")+ strDate,"orderDate");        else m_pLabelView->Remove("orderDate");
                if(m_fieldList->getChecked(5)) m_pLabelView->AddText(QString("支付日期：")+ strPDate,"payDate");         else m_pLabelView->Remove("payDate");
                //m_pLabelView->AddText( strShopId,"shopId");
                //m_pLabelView->AddText( strInId,"oid");
            }

            QTimer::singleShot(100,this,[=]{
                if(ui->checkBoxAutoPrint->isChecked() || ui->checkBoxAutoOut->isChecked())
                {
                    ui->pushButtonPrint->click();
                }
            });
        });

        m_http = new HttpHandler(this);
        connect(m_http,&HttpHandler::onHttpReturn,this,[=](const QString&text,int code){
            // qDebug().noquote() << text;
            QJsonDocument jDoc = QJsonDocument::fromJson(text.toUtf8());
            if(!jDoc.isObject())
            {
                qDebug() << "HTTP ERROR!";
                return;
            }
            QJsonObject jObj = jDoc.object();
            if(m_getType == 0)
            {
                QJsonArray jArr = jObj["data"].toArray();
                m_jArr = jArr;

                 m_pModel->setRowCount(0);
                int count = jArr.count();
                for(int i=0; i<count; i++)
                {
                    QJsonObject jData = jArr[i].toObject();

                    QString strOutId = jData["outerOiId"].toString();

                    QStandardItem *item0 = new QStandardItem(strOutId);
                    QStandardItem *item1 = new QStandardItem("未打印");
                    item0->setData(0);
                    item0->setEditable(false);
                    item1->setEditable(false);
                    m_pModel->appendRow({item0,item1});
                }
                QDateTime NOW = QDateTime::currentDateTime();
                QString strInfo=QString("%1 [%2]").arg(count).arg(NOW.toString("yyyy-MM-dd hh:mm:ss"));

                ui->labelOrderCount->setText(strInfo);
                pTMOut->start(1000);
            }
            else
            {
                qDebug().noquote() << text;
                if(jObj["code"].toInt() == 200)
                {
                }
            }
        });
        connect(ui->pushButtonGet,&QPushButton::clicked,this,[=]{
            pTMOut->stop();
            m_pModel->setRowCount(0);
            m_getType = 0;
            ui->labelOrderCount->setText("0 [0000-00-00 00:00:00]");
            QString text = ui->lineEditUrl->text().trimmed();
            m_http->get(text);
        });
        QTimer::singleShot(1000,this,[=]{
            if(ui->stackedWidget->currentIndex() == 1)
                ui->pushButtonGet->click();
        });
    }
    QTimer::singleShot(600,this,[=]{
        ui->pushButtonSetPaper->click();
    });
}

void FramePrintControl::ShowSN()
{
    int nCount = m_recList->count();
    ui->labelCurCount->setText(QString("%1").arg(nCount));
    ui->textEditAll->clear();
    int colCount = ui->checkBoxDouble->isChecked() ? 2 : 1;
    ui->textEditAll->append(m_recList->toText(colCount));

    if(nCount >= ui->lineEditLimit->text().trimmed().toInt())
        ui->pushButtonCreate->click();
}

FramePrintControl::~FramePrintControl()
{
    delete ui;
}

bool FramePrintControl::eventFilter(QObject *watched, QEvent *event)
{
    return QFrame::eventFilter(watched,event);
}

void FramePrintControl::LoadTemplate(const QString&strFile)
{
    m_pLabelView->Load(strFile);
}

void FramePrintControl::BindLabelView(FrameLabelView *pView)
{
    m_pLabelView = pView;
    LoadTemplate(m_strTemplFile);
}

void FramePrintControl::on_pushButtonPreview_clicked()
{
    m_pLabelView->Preview();
}

void FramePrintControl::on_pushButtonPrint_clicked()
{
    int nCount = ui->spinBoxPrintCount->value();
    for(int i=0; i<nCount; i++)
        m_pLabelView->Print();

    if(ui->stackedWidget->currentIndex() == 1)
    {
        int count = m_pModel->rowCount();
        for(int i=0; i<count; i++)
        {
            QStandardItem *item0 = m_pModel->item(i,0);
            QStandardItem *item1 = m_pModel->item(i,1);
            if(item0->text() == ui->lineEditBarcode->text())
            {
                m_getType = 1;
                QString strGet = QString("http://113.98.191.209:8085/sales/erpOrderItem/status?outerOiId=%1&bizStatus=1").arg(item0->text());
                m_http->get(strGet);

                m_nSelected = i;
                item0->setData(1);
                item1->setText("已打印");
                break;
            }
        }
    }
}

