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
#include <QPrintPreviewDialog>

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

void drawTextAutoWrapAndScale(QPainter* painter, const QRect& rect, const QString& text, QFont baseFont, Qt::Alignment alignment = Qt::AlignCenter)
{
    if (!painter || rect.isEmpty() || text.isEmpty())
        return;

    const int minFontSize = 1;
    const int maxFontSize = 18;
    int bestFontSize = minFontSize;

    // 二分搜索合适字号
    int low = minFontSize;
    int high = maxFontSize;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        baseFont.setPointSize(mid);

        QTextDocument doc;
        doc.setDefaultFont(baseFont);
        doc.setPlainText(text);
        doc.setTextWidth(rect.width()); // 设置换行宽度

        QSizeF textSize = doc.size();
        if (textSize.height() <= rect.height())
        {
            bestFontSize = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    baseFont.setPointSize(bestFontSize);

    QTextDocument doc;
    doc.setDefaultFont(baseFont);
    doc.setPlainText(text);
    doc.setTextWidth(rect.width());

    // 设置文本对齐
    QTextOption option;
    option.setAlignment(alignment);
    doc.setDefaultTextOption(option);

    painter->save();
    painter->translate(rect.topLeft());
    doc.drawContents(painter, QRectF(0,0, rect.width(), rect.height()));
    painter->restore();
}

FramePrintControl::FramePrintControl(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FramePrintControl)
{
    ui->setupUi(this);
    QString strCfgPath = QApplication::applicationDirPath() + "/config";
    QDir D(strCfgPath);
    if(!D.exists())
        D.mkdir(strCfgPath);

    m_pSet = new QSettings(strCfgPath + "/global.ini",QSettings::IniFormat);

    int nFunc = 2;
    ui->stackedWidget->setCurrentIndex(nFunc);

    QString strTemp = QString("/default%1.tem").arg(nFunc);

    m_strTemplFile = strCfgPath + strTemp;//m_pSet->value("lastTemplFile", strCfgPath + strTemp).toString();

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

    m_recList   = new DialogRecList(this);
    m_referId   = new DialogReferId(this);
    m_fieldList = new DialogFieldPickup(this);
    m_labelEdit = new DialogLabelEdit(this);

    ui->lineEditUrl->hide();

    ui->lineEditLimit->setText(m_pSet->value("limitCount","40").toString());
    ui->lineEditCode0->setText(m_pSet->value("Code0","BJ").toString());
    ui->lineEditCode1->setText(m_pSet->value("Code1","20260507").toString());
    ui->lineEditCode2->setText(m_pSet->value("Code2","A").toString());
    ui->lineEditCode3->setText(m_pSet->value("Code3","0001").toString());

    ui->lineEditLenght->setText(m_pSet->value("askLen","16").toString());
    ui->checkBoxCheckLen->setChecked(m_pSet->value("checkLen",true).toBool());
    ui->checkBoxGen128->setChecked(m_pSet->value("gen128",true).toBool());

    ui->lineEdit300->setText(m_pSet->value("text300","16").toString());
    ui->lineEdit301->setText(m_pSet->value("text301","16").toString());
    ui->textEdit302->setText(m_pSet->value("text302","16").toString());
    ui->lineEdit303->setText(m_pSet->value("text303","16").toString());
    ui->spinBoxCount->setValue(m_pSet->value("count304","1").toInt());
    ui->spinBoxUnit->setValue(m_pSet->value("text308","1").toInt());
    ui->dateTimeEdit0->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit1->setDateTime(QDateTime::currentDateTime());

    ui->label300->setText(m_pSet->value("name300","厂商名称").toString().replace("：","").trimmed() + "：");
    ui->label301->setText(m_pSet->value("name301","采购订单号").toString().replace("：","").trimmed() + "：");
    ui->label302->setText(m_pSet->value("name302","品名/规格").toString().replace("：","").trimmed() + "：");
    ui->label303->setText(m_pSet->value("name303","物料编码").toString().replace("：","").trimmed() + "：");
    ui->label304->setText(m_pSet->value("name304","数　　量").toString().replace("：","").trimmed() + "：");
    ui->label305->setText(m_pSet->value("name305","生产日期").toString().replace("：","").trimmed() + "：");
    ui->label306->setText(m_pSet->value("name306","交货日期").toString().replace("：","").trimmed() + "：");
    ui->label307->setText(m_pSet->value("name307","检验结果").toString().replace("：","").trimmed() + "：");

    ui->lineEditPaperW->setText(m_pSet->value("PaperW","60").toString());
    ui->lineEditPaperH->setText(m_pSet->value("PaperH","40").toString());
    ui->spinBoxPrintCol->setValue(m_pSet->value("PrintCol",1).toInt());
    ui->spinBoxPrintRow->setValue(m_pSet->value("PrintRow",1).toInt());

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
    connect(ui->pushButtonLabelMng,&QPushButton::clicked,this,[=]{
        m_labelEdit->show();
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
        connect(ui->checkBoxAutoGet,&QCheckBox::toggled,this,[=](bool checked){
            pTMGet->stop();
            ui->progressBarGet->setRange(0,ui->lineEditIntrval->text().toInt());
            autoGet = 0;
            if(checked) pTMGet->start(1000);
        });

        connect(pTMGet,&QTimer::timeout,this,[=]{
            int interval = ui->lineEditIntrval->text().toInt();
            if(interval<5) interval=5;
            //ui->progressBarGet->setRange(0,interval);
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
            m_nSelected = -1;
            pTMOut->stop();
            pTMOut->start(2500);
            if(ui->checkBoxAutoOut->isChecked())
            {
                int count = m_pModel->rowCount();
                for(int i=0; i<count; i++)
                {
                    QStandardItem *item0 = m_pModel->item(i,0);
                    if(item0->data().toInt() == 0)
                    {
                        m_nSelected = i;
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

            if(m_nSelected >= 0)
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
            m_nSelected = -1;

            QTimer::singleShot(300,this,[=]{
                if(ui->checkBoxAutoPrint->isChecked() || ui->checkBoxAutoOut->isChecked())
                {
                    ui->pushButtonPrint->click();
                }
            });
        });

        connect(ui->pushButtonToday0,&QPushButton::clicked,this,[=]{
            ui->dateTimeEdit0->setDateTime(QDateTime::currentDateTime());
            ui->pushButtonGenLabel->click();
        });
        connect(ui->pushButtonToday1,&QPushButton::clicked,this,[=]{
            ui->dateTimeEdit1->setDateTime(QDateTime::currentDateTime());
            ui->pushButtonGenLabel->click();
        });

        connect(ui->lineEdit300,&QLineEdit::textChanged,this,[=]{
            ui->pushButtonGenLabel->click();
        });
        connect(ui->lineEdit301,&QLineEdit::textChanged,this,[=]{
            ui->pushButtonGenLabel->click();
        });
        connect(ui->textEdit302,&QTextEdit::textChanged,this,[=]{
            ui->pushButtonGenLabel->click();
        });
        connect(ui->lineEdit303,&QLineEdit::textChanged,this,[=]{
            ui->pushButtonGenLabel->click();
        });
        connect(ui->radioButton0,&QRadioButton::clicked,this,[=]{
            ui->pushButtonGenLabel->click();
        });
        connect(ui->radioButton1,&QRadioButton::clicked,this,[=]{
            ui->pushButtonGenLabel->click();
        });
        connect(ui->radioButton2,&QRadioButton::clicked,this,[=]{
            ui->pushButtonGenLabel->click();
        });

        connect(ui->pushButtonGenLabel,&QPushButton::clicked,this,[=]{

            QString strName300 = ui->label300->text().trimmed();
            QString strText300 = ui->lineEdit300->text().trimmed();

            QString strName301 = ui->label301->text().trimmed();
            QString strText301 = ui->lineEdit301->text().trimmed();
            QString strName302 = ui->label302->text().trimmed();
            QString strText302 = ui->textEdit302->toPlainText().trimmed();
            QString strName303 = ui->label303->text().trimmed();
            QString strText303 = ui->lineEdit303->text().trimmed();

            QString strName304 = ui->label304->text().trimmed();
            QString strText304 = QString("%1").arg(ui->spinBoxCount->value());

            QString strName305 = ui->label305->text().trimmed();
            QString strText305 = QString("%1").arg(ui->dateTimeEdit0->dateTime().toString("yyyy-MM-dd"));//  HH:mm:ss
            QString strName306 = ui->label306->text().trimmed();
            QString strText306 = QString("%1").arg(ui->dateTimeEdit1->dateTime().toString("yyyy-MM-dd"));
            QString strName307 = ui->label307->text().trimmed();
            QString strText307 = QString("%1").arg(ui->radioButton0->isChecked()?"PASS":"  NG  ");
            if(ui->radioButton2->isChecked()) strText307 = "签章";

            QString strText308 = QString("%1").arg(ui->spinBoxUnit->value());

            strName300.replace("：","　　");
            strName301.replace("：","　　");
            strName302.replace("：","　　");
            strName303.replace("：","　　");
            strName304.replace("：","　　");
            strName305.replace("：","　　");
            strName306.replace("：","　　");
            strName307.replace("：","　　");

            m_pLabelView->AddText(strName300 + strText300, "value300");
            m_pLabelView->AddText(strName301 + strText301, "value301");
            m_pLabelView->AddText(strName303 + strText303, "value303");
            m_pLabelView->AddText(strName304 + strText304, "value304");
            m_pLabelView->AddText(strName305 + strText305, "value305");
            m_pLabelView->AddText(strName306 + strText306, "value306");
            m_pLabelView->AddText(strName307.trimmed() , "value307");            
            m_pLabelView->AddText(strText307, "Qpass");
            m_pLabelView->AddImageQR(strText303,"QrCode303");
            if(strText302.length()>24)
            {
                m_pLabelView->AddText(strName302.trimmed() , "value302");
<<<<<<< HEAD

                QImage textImg(400,120,QImage::Format_ARGB32);
=======
                QImage textImg(400,120,QImage::Format_RGB32);
>>>>>>> 7da5d8ae6359f4962182853428be9a70c8395930
                QPainter painter(&textImg);
                painter.fillRect(textImg.rect(),Qt::NoBrush);

                QFont font("Microsoft YaHei");
                painter.setPen(Qt::black);
<<<<<<< HEAD
                QString str = strText302;
                drawTextAutoWrapAndScale(&painter,textImg.rect(),str,font);
=======
                painter.fillRect(QRect(0,0,400,120),Qt::white);
                QString str = strText302;
                drawTextAutoWrapAndScale(&painter,QRect(0,0,400,120),str,font);
>>>>>>> 7da5d8ae6359f4962182853428be9a70c8395930
                m_pLabelView->AddImage(textImg,"longText");
                m_pLabelView->SetItemScale("longText",1);
            }
            else
            {
                m_pLabelView->AddText(strName302 + strText302, "value302");
<<<<<<< HEAD
                QImage textImg(4,4,QImage::Format_ARGB32);
=======
                QImage textImg(4,4,QImage::Format_RGB32);
>>>>>>> 7da5d8ae6359f4962182853428be9a70c8395930
                QPainter painter(&textImg);
                painter.fillRect(QRect(0,0,4,4),Qt::white);
                m_pLabelView->AddImage(textImg,"longText");
                m_pLabelView->SetItemScale("longText",0.1);
            }

            QString strExt = " ";
            if(ui->radioButton2->isChecked()) strExt = "(不可遮盖二维码)";
            m_pLabelView->AddText(strExt , "value308");

            m_pLabelView->AddLine(true,"LineH0");
            m_pLabelView->AddLine(true,"LineH1");
            m_pLabelView->AddLine(true,"LineH2");
            m_pLabelView->AddLine(true,"LineH3");
            m_pLabelView->AddLine(true,"LineH4");
            m_pLabelView->AddLine(true,"LineH5");
            m_pLabelView->AddLine(true,"LineH6");
            m_pLabelView->AddLine(true,"LineH7");
            m_pLabelView->AddLine(true,"LineH8");

            m_pLabelView->AddLine(false,"LineV0");
            m_pLabelView->AddLine(false,"LineV1");
            m_pLabelView->AddLine(false,"LineV2");

            m_pSet->setValue("name300",strName300);
            m_pSet->setValue("name301",strName301);
            m_pSet->setValue("name302",strName302);
            m_pSet->setValue("name303",strName303);
            m_pSet->setValue("name304",strName304);
            m_pSet->setValue("name305",strName305);
            m_pSet->setValue("name306",strName306);
            m_pSet->setValue("name307",strName307);

            m_pSet->setValue("text300",strText300);
            m_pSet->setValue("text301",strText301);
            m_pSet->setValue("text302",strText302);
            m_pSet->setValue("text303",strText303);
            m_pSet->setValue("text308",strText308);

            QTimer::singleShot(300,this,[=]{
                if(ui->checkBoxAutoPrint->isChecked() || ui->checkBoxAutoOut->isChecked())
                {
                    ui->pushButtonPrint->click();
                }
            });
        });

        connect(ui->spinBoxCount,&QSpinBox::textChanged,this,[=](const QString&){
            Calculate();
            ui->pushButtonGenLabel->click();
        });

        connect(ui->spinBoxUnit,&QSpinBox::textChanged,this,[=](const QString&){
            Calculate();
            ui->pushButtonGenLabel->click();
        });

        connect(ui->spinBoxPrintCol,&QSpinBox::textChanged,this,[=](const QString&){
            Calculate();
        });
        connect(ui->spinBoxPrintRow,&QSpinBox::textChanged,this,[=](const QString&){
            Calculate();
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

                    QString strOutId  = jData["outerOiId"].toString();
                    QString strStatus = jData["bizStatus"].toString();

                    QStandardItem *item0 = new QStandardItem(strOutId);
                    QStandardItem *item1 = new QStandardItem("未打印");

                    item0->setData(0);
                    if(strStatus== "1")
                    {
                        item0->setData(1);
                        item1->setText("已打印");
                    }

                    item0->setEditable(false);
                    item1->setEditable(false);
                    m_pModel->appendRow({item0,item1});
                }
                QDateTime NOW  = QDateTime::currentDateTime();
                QString strInfo= QString("%1 [%2]").arg(count).arg(NOW.toString("yyyy-MM-dd hh:mm:ss"));

                ui->labelOrderCount->setText(strInfo);
                pTMOut->start(500);
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

            QString strUrl = ui->lineEditUrl->text().trimmed();
            QString strRefer = m_referId->getIds();
            if(!strRefer.isEmpty())
                strUrl += QString("?referrerId=") + strRefer;

            m_http->get(strUrl);
        });

        connect(ui->pushButtonHistory,&QPushButton::clicked,this,[=]{
            pTMOut->stop();
            m_pModel->setRowCount(0);
            ui->checkBoxAutoGet->setChecked(false);
            m_getType = 0;
            ui->labelOrderCount->setText("0 [0000-00-00 00:00:00]");

            QString strUrl = "http://113.98.191.209:8085/sales/erpOrderItem/history";
            QString strRefer = m_referId->getIds();
            if(!strRefer.isEmpty())
                strUrl += QString("?referrerId=") + strRefer;

            m_http->get(strUrl);
        });

        connect(ui->pushButtonQuery,&QPushButton::clicked,this,[=]{
            QString strOrderId = ui->lineEditOrderId->text().trimmed();
            if(strOrderId.length() < 5)
            {
                QMessageBox::warning(this,"提示","请输入订单号！");
                return;
            }
            pTMOut->stop();
            m_pModel->setRowCount(0);
            ui->checkBoxAutoGet->setChecked(false);
            m_getType = 0;
            ui->labelOrderCount->setText("0 [0000-00-00 00:00:00]");

            QString strUrl = QString("http://113.98.191.209:8085/sales/erpOrderItem/getByOuterOiId?outerOiId=") + strOrderId;
            qDebug().noquote() << strUrl;
            m_http->get(strUrl);
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

void FramePrintControl::Calculate()
{
<<<<<<< HEAD
    int nCols = ui->spinBoxPrintCol->value();
    int nRows = ui->spinBoxPrintRow->value();
    int nUnit = ui->spinBoxUnit->value() * nCols * nRows;
=======
    int nCol = ui->spinBoxPrintCol->value();
    int nRow = ui->spinBoxPrintRow->value();
    int nUnit = ui->spinBoxUnit->value() * nCol * nRow;
>>>>>>> 7da5d8ae6359f4962182853428be9a70c8395930
    int nCount = ui->spinBoxCount->value();
    if(nUnit <= 0) return;
    int nPrintCount = nCount / nUnit;
    if(nCount % nUnit) nPrintCount ++;

    ui->spinBoxPrintCount->setValue(nPrintCount);

<<<<<<< HEAD
    m_pSet->setValue("PrintCol",nCols);
    m_pSet->setValue("PrintRow",nRows);
=======
    m_pSet->setValue("PrintCol",nCol);
    m_pSet->setValue("PrintRow",nRow);
>>>>>>> 7da5d8ae6359f4962182853428be9a70c8395930
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
    ui->pushButtonSetPaper->click();
    QTimer::singleShot(100,this,[=]{
        LoadTemplate(m_strTemplFile);
    });
}

void FramePrintControl::on_pushButtonPreview_clicked()
{
<<<<<<< HEAD
    int nCols = ui->spinBoxPrintCol->value();
    int nRows = ui->spinBoxPrintRow->value();
    if(nCols < 1) nCols = 1;
    if(nRows < 1) nRows = 1;
    m_pLabelView->SetPrintMatrix(nRows,nCols);

    if(ui->stackedWidget->currentIndex() == 2)
    {
        int nUnit = ui->spinBoxUnit->value();
        int nProCount = ui->spinBoxCount->value();
        int nLabelCount = nProCount/ nUnit;
        int nLast = nProCount % nUnit;
        if(nLast) nLabelCount++;
        if(nLast == 0) nLast = nUnit;

        int index = 0;
        bool finish = false;
        int nPages = nLabelCount/(nCols * nRows);
        if(nLabelCount % (nCols * nRows))
            nPages++;

        int nW = m_pLabelView->size().width();
        int nH = m_pLabelView->size().height();
        QPixmap pixmap2(nW * nCols, nH * nRows);
        pixmap2.fill(Qt::white);
        for(int page=0; page<nPages; page++)
        {
            if(finish) break;
            pixmap2.fill(Qt::white);
            QPainter painter2(&pixmap2);
            for(int row=0; row<nRows; row++)
            {
                if(finish) break;

                for(int col=0; col<nCols; col++)
                {
                    index++;
                    if(index>nLabelCount)
                    {
                        finish = true;
                        break;
                    }

                    QString strName304 = ui->label304->text().trimmed();
                    QString strText304 = QString("%1").arg(ui->spinBoxCount->value());
                    strName304.replace("：","");
                    int ship = nUnit;
                    if(index == nLabelCount) ship = nLast;
                    QString strCount = QString("%1　　%2 (本箱：%3, 第 %4 箱, 共 %5 箱)").arg(strName304).arg(strText304).arg(ship).arg(index).arg(nLabelCount);
                    //qDebug() << strCount;

                    m_pLabelView->AddText(strCount, "value304");
                    m_pLabelView->update();

                    QPixmap pixmap = m_pLabelView->toPixmap();
                    painter2.drawPixmap(col*nW,row*nH,nW,nH,pixmap);
                }
            }
        }

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::NativeFormat);

        QPrintPreviewDialog previewDialog(&printer, this);
        connect(&previewDialog, &QPrintPreviewDialog::paintRequested, this, [=](QPrinter *printer) {
            QPainter painter(printer);
            QRect rect = painter.viewport();
            QSize size = pixmap2.size();
            size.scale(rect.size(), Qt::KeepAspectRatio);  // 保持比例缩放
            painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
            painter.setWindow(pixmap2.rect());
            painter.drawPixmap(0, 0, pixmap2);
        });

        previewDialog.exec();

        return;
    }

    m_pLabelView->Preview();    
}

void FramePrintControl::on_pushButtonPrint_clicked()
{
    int nCols = ui->spinBoxPrintCol->value();
    int nRows = ui->spinBoxPrintRow->value();
    m_pLabelView->SetPrintMatrix(nRows,nCols);
    if(ui->stackedWidget->currentIndex() == 2)
    {
        //int nPrintCount = ui->spinBoxPrintCount->value();
        //for(int p=0; p<nPrintCount; p++)
        {
            int nUnit = ui->spinBoxUnit->value();
            int nProCount = ui->spinBoxCount->value();
            int nLabelCount = nProCount/ nUnit;
            int nLast = nProCount % nUnit;
            if(nLast) nLabelCount++;
            if(nLast == 0) nLast = nUnit;

            int index = 0;
            bool finish = false;
            int nPages = nLabelCount/(nCols * nRows);
            if(nLabelCount % (nCols * nRows))
                nPages++;

            int nW = m_pLabelView->size().width();
            int nH = m_pLabelView->size().height();
            QPixmap pixmap2(nW * nCols, nH * nRows);
            pixmap2.fill(Qt::white);
            for(int page=0; page<nPages; page++)
            {
                if(finish) break;
                pixmap2.fill(Qt::white);
                QPainter painter2(&pixmap2);
                for(int row=0; row<nRows; row++)
                {
                    if(finish) break;

                    for(int col=0; col<nCols; col++)
                    {
                        index++;
                        if(index > nLabelCount)
                        {
                            finish = true;
                            break;
                        }

                        QString strName304 = ui->label304->text().trimmed();
                        QString strText304 = QString("%1").arg(ui->spinBoxCount->value());
                        strName304.replace("：","");
                        int ship = nUnit;
                        if(index == nLabelCount) ship = nLast;
                        QString strCount = QString("%1　　%2 (本箱：%3, 第 %4 箱, 共 %5 箱)").arg(strName304).arg(strText304).arg(ship).arg(index).arg(nLabelCount);
                        //qDebug() << strCount;

                        m_pLabelView->AddText(strCount, "value304");
                        m_pLabelView->update();

                        QPixmap pixmap = m_pLabelView->toPixmap();
                        painter2.drawPixmap(col*nW,row*nH,nW,nH,pixmap);
                    }
                }
            }

            {
                QPrinter printer(QPrinter::HighResolution);
                printer.setOutputFormat(QPrinter::NativeFormat);

                QPainter painter(&printer);
                QRect rect = painter.viewport();
                QSize size = pixmap2.size();
                size.scale(rect.size(), Qt::KeepAspectRatio);
                painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                painter.setWindow(pixmap2.rect());
                painter.drawPixmap(0, 0, pixmap2);
            }
        }
        return;
    }

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

