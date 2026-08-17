#ifndef DIALOGLABELEDIT_H
#define DIALOGLABELEDIT_H

#include "FrameLabelView.h"

#include <QDialog>
#include <QStandardItemModel>


#include <QPainter>
#include <QTableView>

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QStyledItemDelegate>


class UppercaseHexSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit UppercaseHexSpinBox(QWidget *parent = nullptr) : QSpinBox(parent)
    {
        // 设置为16进制显示
        //setDisplayIntegerBase(16);
    }

protected:
    QString textFromValue(int value) const override
    {
        if(displayIntegerBase() == 16)
            return QString("%1").arg(value, 0, 16).toUpper();

        return QString("%1").arg(value, 0, 10).toUpper();
    }
};

class DateTimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DateTimeDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent)
    {
    }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(option)
        Q_UNUSED(index)
        QDateTimeEdit *pDTEdit = new QDateTimeEdit(parent);
        pDTEdit->setDisplayFormat("yyyy-MM-dd");
        return pDTEdit;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString strValue = index.model()->data(index, Qt::EditRole).toString();
        QDateTimeEdit *pDTEdit = static_cast<QDateTimeEdit*>(editor);
        QLocale enUs(QLocale::English, QLocale::UnitedStates);
        QDate dt0 = enUs.toDate(strValue, "yyyy-MM-dd");;
        pDTEdit->setDate(dt0);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QDateTimeEdit *pDTEdit = static_cast<QDateTimeEdit*>(editor);
        QString value = pDTEdit->date().toString("yyyy-MM-dd");
        model->setData(index, value, Qt::EditRole);
    }
};

class ComboBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ComboBoxDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent)
    {
    }

    void setItems(const QStringList&strItems){_strItems=strItems;}
    QStringList _strItems{"PASS","NG","签章"};
protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(option)
        Q_UNUSED(index)
        QComboBox *pComb = new QComboBox(parent);
        pComb->addItems(_strItems);
        connect(pComb,&QComboBox::textActivated,this,[=](const QString&text){
            QStandardItemModel *pModel = (QStandardItemModel *)index.model();
            QStandardItem *item = pModel->item(index.row(),index.column());
            item->setData(text,Qt::EditRole);
            pComb->hide();
        });
<<<<<<< HEAD
        QTimer::singleShot(20,this,[=]{pComb->showPopup();});
=======
>>>>>>> 6173a2adc7931979bf1b1a8c327d53f1b50ea3f1

        return pComb;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString strValue = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *pComb = static_cast<QComboBox*>(editor);
        int idx = pComb->findText(strValue);
        if (idx >= 0)
        {
            pComb->setCurrentIndex(idx);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QComboBox *pComb = static_cast<QComboBox*>(editor);
        QString value = pComb->currentText();
        model->setData(index, value, Qt::EditRole);
    }
};

class SideValueDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    SideValueDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {
        connect(this,&SideValueDelegate::onSaveOldText,this,[=](int nRow,const QString&text){
            setText(nRow,text);
        });
    }
    void setText(int nRow,const QString&text){
        _texts[nRow] = text;
        if(m_bFloatMode)
        {
            QString val = QString::asprintf("%.2f",text.toFloat());
            _texts[nRow] = val;
        }
    }
    void setTableView(QTableView *pView){ tableView = pView;}
    void setHexMode(bool set=true){ m_bHexMode = set ;}
    void setIntStep(int step){ m_nStep = step ;}
    void setFloatMode(bool set,float step=0.05){m_bFloatMode=set; m_fStep = step;};
    void resetText(){_texts.clear();}

protected:
    QMap<int,QString> _texts;
    QTableView *tableView = nullptr;
    bool m_bHexMode = false;
    bool m_bFloatMode = false;
    float m_fStep  = 0.1;
    int  m_nStep  = 1;

signals:
    void onSaveOldText(int nRow,const QString&text) const;

protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        Q_UNUSED(option)
        Q_UNUSED(index)

        QStandardItemModel *pModel = (QStandardItemModel *)index.model() ;
        QString strText = pModel->item(index.row(),index.column())->text() ;
        emit onSaveOldText(index.row(),strText);

        if(m_bFloatMode)
        {
            QDoubleSpinBox *pFBox = new QDoubleSpinBox(parent);
            pFBox->setRange(0.05,10000);
            pFBox->setSingleStep(m_fStep);

            connect(pFBox,&QDoubleSpinBox::textChanged,this,[=](const QString & text){
                QStandardItemModel *pModel = (QStandardItemModel *)index.model();
                QStandardItem *item = pModel->item(index.row(),index.column());
                item->setData(text,Qt::EditRole);
            }, Qt::UniqueConnection);
            return pFBox;
        }

        UppercaseHexSpinBox *pBox = new UppercaseHexSpinBox(parent) ;
        pBox->setRange(-1000000,1000000);
        pBox->setSingleStep(m_nStep);
        if(m_bHexMode)
        {
            pBox->setPrefix("0x");
            pBox->setDisplayIntegerBase(16);
        }

        connect(pBox,&UppercaseHexSpinBox::textChanged,this,[=](const QString & text){
            QStandardItemModel *pModel = (QStandardItemModel *)index.model();
            QStandardItem *item = pModel->item(index.row(),index.column());
            item->setData(text,Qt::EditRole);
        }, Qt::UniqueConnection);

        return pBox;
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::paint(painter,option,index) ;

        painter->save();

        QRect rect = option.rect;
        rect.adjust(4,4,-4,-4);

        QPen pen(Qt::darkGray);
        painter->setPen(pen);
        painter->drawText(rect,Qt::AlignRight|Qt::AlignVCenter,_texts[index.row()]);

        painter->restore();
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString strValue = index.model()->data(index, Qt::EditRole).toString();
        if(m_bFloatMode)
        {
            QDoubleSpinBox *pFBox = static_cast<QDoubleSpinBox*>(editor);
            pFBox->setValue(strValue.toDouble());
        }
        else
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            int value = m_bHexMode ? strValue.toInt(nullptr,16) : strValue.toInt();
            spinBox->setValue(value);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {

        if(m_bFloatMode)
        {
            QDoubleSpinBox *pFBox = static_cast<QDoubleSpinBox*>(editor);
            float value = pFBox->value();
            model->setData(index, value, Qt::EditRole);
        }
        else
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->interpretText();

            if(m_bHexMode)
            {
                QString value = spinBox->text();
                value.replace(spinBox->prefix(),"");
                model->setData(index, value.toUpper(), Qt::EditRole);
            }
            else
            {
                int value = spinBox->value();
                model->setData(index, value, Qt::EditRole);
            }
        }
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override
    {
        if(event->type() == QEvent::MouseButtonRelease && tableView)
        {
            QRect cellRect = tableView->visualRect(index);

            QPoint clp = tableView->viewport()->mapFromGlobal(QCursor::pos());

            int nx = clp.x();
            int ny = clp.y();

            for(int i=0; i<index.column(); i++)
                nx -= tableView->columnWidth(i);
            for(int i=0; i<index.row(); i++)
                ny -= tableView->rowHeight(i);
            if(nx * 10 / cellRect.width() > 6)
            {
                QStandardItemModel *pModel = (QStandardItemModel *)index.model();
                QStandardItem *item = pModel->item(index.row(),index.column());
                if(item && !_texts[index.row()].isEmpty()) item->setText(_texts[index.row()]);
            }
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;
        opt.rect.adjust(0,0,opt.rect.width()/2 * -1 + 20 ,0);
        QStyledItemDelegate::updateEditorGeometry(editor,opt,index);
    }
};


namespace Ui {
class DialogLabelEdit;
}

class FrameLabelView;

class DialogLabelEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLabelEdit(QWidget *parent = nullptr);
    ~DialogLabelEdit();
    void BindLabelView(FrameLabelView *pView);
private:
    Ui::DialogLabelEdit *ui;
    QStandardItemModel *m_pModel = nullptr;
    FrameLabelView *m_pView=nullptr;
    int m_nSelected = -1;
    bool m_bLoading=false;
    void AppendRow(QGraphicsItem *item);
    void LoadLabels();

    SideValueDelegate *pDele0;
    SideValueDelegate *pDele1;
    SideValueDelegate *pDele2;
    SideValueDelegate *pDele3;
    SideValueDelegate *pDele4;
};

#endif // DIALOGLABELEDIT_H
