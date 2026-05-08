#include "DialogSet.h"
#include "ui_DialogSet.h"

#include <QTimer>

DialogSet::DialogSet(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSet)
{
    ui->setupUi(this);

    connect(ui->pushButtonOK,&QPushButton::clicked,this,[=]{
        m_strCount = ui->lineEdit->text().trimmed();
        this->accept();
    });

    connect(ui->pushButtonCancel,&QPushButton::clicked,this,[=]{
        this->reject();
    });

    connect(ui->horizontalSlider,&QSlider::valueChanged,this,[=](int value){
        ui->lineEdit->setText(QString("%1").arg(value));
    });

    connect(ui->lineEdit,&QLineEdit::textChanged,this,[=](const QString&text){
        ui->horizontalSlider->setValue(text.toInt());
    });

    QTimer::singleShot(50,this,[=]{
        ui->lineEdit->setText(m_strCount);
    });
}

DialogSet::~DialogSet()
{
    delete ui;
}
