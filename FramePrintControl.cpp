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
#include <QMessageBox>

FramePrintControl::FramePrintControl(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FramePrintControl)
{
    ui->setupUi(this);

    m_pSet = new QSettings(QApplication::applicationDirPath() + "/config/global.ini",QSettings::IniFormat);

    m_strTemplFile = m_pSet->value("lastTemplFile", QApplication::applicationDirPath() + "/config/default.tem").toString();

    ui->checkBoxDouble->setChecked(true);

    m_recList = new DialogRecList(this);

    ui->lineEditLimit->setText(m_pSet->value("limitCount","40").toString());
    ui->lineEditCode0->setText(m_pSet->value("Code0","BJ").toString());
    ui->lineEditCode1->setText(m_pSet->value("Code1","20260507").toString());
    ui->lineEditCode2->setText(m_pSet->value("Code2","A").toString());
    ui->lineEditCode3->setText(m_pSet->value("Code3","0001").toString());

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
}

