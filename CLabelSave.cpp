#include "CLabelSave.h"
#include "CustomItems.h"

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QBuffer>
#include <QImage>
#include <QGraphicsScene>
#include <QDebug>
#include "FrameLabelView.h"

CLabelSave::CLabelSave()
{

}

CLabelSave::~CLabelSave()
{

}


QImage CLabelSave::decodeImage(const QString &imageData)
{
    QByteArray byteArray = QByteArray::fromBase64(imageData.toLatin1());
    QImage image;
    image.loadFromData(byteArray, "PNG");  // 从 Base64 编码数据加载图片
    return image;
}

QByteArray CLabelSave::encodeImage( const QImage &image )
{
    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );
    image.save( &buffer, "PNG" );  // 将图片以 PNG 格式保存到缓冲区
    return byteArray.toBase64();   // 将图片数据进行 Base64 编码
}

bool CLabelSave::saveSceneWithImages(CustomScene *scene, const QString &filePath)
{
    QJsonArray itemsArray;

    for (auto item : scene->items())
    {
        if (auto textItem = dynamic_cast<CustomTextItem*>(item))
        {
            QJsonObject textObject;
            textObject["name"] = textItem->getName();
            textObject["type"] = "text";
            textObject["text"] = textItem->toPlainText();

            QFont tf = textItem->font();
            textObject["font"] = tf.family();
            textObject["style"] = tf.styleName();
            textObject["bold"] = tf.bold();
            textObject["size"] = tf.pointSize();

            textObject["x"] = textItem->pos().x();
            textObject["y"] = textItem->pos().y();
            textObject["w"] = textItem->getItemRect().width();
            textObject["h"] = textItem->getItemRect().height();
            textObject["scale"] = textItem->scale();
            textObject["zvalue"]   = textItem->zValue();
            textObject["type_in0"] = textItem->data( 0 ).toInt();
            textObject["type_in1"] = textItem->data( 1 ).toInt();
            textObject["type_in2"] = textItem->data( 2 ).toInt();
            textObject["type_in3"] = textItem->data( 3 ).toInt();
            itemsArray.append(textObject);
        }
        else if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
        {
            QJsonObject pixmapObject;
            pixmapObject["name"] = pixmapItem->getName();
            pixmapObject["type"] = "pixmap";
            pixmapObject["x"] = pixmapItem->pos().x();
            pixmapObject["y"] = pixmapItem->pos().y();
            pixmapObject["w"] = pixmapItem->getItemRect().width();
            pixmapObject["h"] = pixmapItem->getItemRect().height();
            pixmapObject["scale"] = pixmapItem->scale();
            pixmapObject["zvalue"]   = pixmapItem->zValue();
            pixmapObject["type_in0"] = pixmapItem->data( 0 ).toInt();
            pixmapObject["type_in1"] = pixmapItem->data( 1 ).toInt();
            pixmapObject["type_in2"] = pixmapItem->data( 2 ).toInt();
            pixmapObject["type_in3"] = pixmapItem->data( 3 ).toInt();

            QImage image = pixmapItem->pixmap().toImage();
            QByteArray imageData = encodeImage(image);
            pixmapObject["imageData"] = QString::fromLatin1(imageData);

            itemsArray.append(pixmapObject);
        }
    }

    // 保存到 JSON 文件
    QFile file( filePath );
    if( file.open( QIODevice::WriteOnly ) )
    {
        QJsonObject sceneObject;
        sceneObject["items"] = itemsArray;
        QJsonDocument doc( sceneObject );
        if( file.write( doc.toJson() ) < 0 )
        {
            qDebug() << "Label template data write error." << filePath;
            file.close();
            return false;
        }
        file.close();
        return true;
    }
     return false;
}

bool CLabelSave::loadSceneWithImages(CustomScene *scene, const QString &filePath)
{
    QFile file( filePath );
    if ( !file.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "File open error: " << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject sceneObject = doc.object();
    QJsonArray itemsArray = sceneObject["items"].toArray();

    scene->clear();

    int nW = scene->width();
    int nH = scene->height();

    for( const QJsonValue &value : std::as_const(itemsArray) )
    {
        QJsonObject obj = value.toObject();
        QString type = obj["type"].toString();
        QString name = obj["name"].toString();

        if (type == "text")
        {
            QString text = obj["text"].toString();
            float x = obj["x"].toDouble();
            float y = obj["y"].toDouble();
            float scale = obj["scale"].toDouble();
            int zvalue = obj["zvalue"].toInt();
            int type_in0 = obj["type_in0"].toInt();
            int type_in1 = obj["type_in1"].toInt();
            int type_in2 = obj["type_in2"].toInt();
            int type_in3 = obj["type_in3"].toInt();

            if(scale <= 0.05) scale = 0.5;
            if(x < 0) x = 0;
            if(y < 0) y = 0;
            if(x > nW-10) x = nW-10 ;
            if(y > nH-10) y = nH-10 ;

            text.replace("：","　　");

            CustomTextItem *textItem = new CustomTextItem( text );
            textItem->setPos(x, y);
            textItem->setScale( scale );
            textItem->setZValue( zvalue );
            textItem->setData( 0, type_in0 );
            textItem->setData( 1, type_in1 );
            textItem->setData( 2, type_in2 );
            textItem->setData( 3, type_in3 );
            textItem->setName( name );
            if(obj.contains("font"))
            {
                QFont font1;
                font1.setFamily(obj["font"].toString());
                //font1.setFamily("微软雅黑");
                font1.setStyleName(obj["style"].toString());
                font1.setBold( obj["bold"].toBool());
                font1.setPointSize(obj["size"].toInt());
                textItem->setFont(font1);
            }
            if(name == "QPass")
                textItem->m_bShowRect = true;
            scene->addItem(textItem);
            //textItem->setScale(1);
        }
        else if (type == "pixmap")
        {
            float x = obj["x"].toDouble();
            float y = obj["y"].toDouble();
            float scale = obj["scale"].toDouble();
            int zvalue = obj["zvalue"].toInt();
            int type_in0 = obj["type_in0"].toInt();
            int type_in1 = obj["type_in1"].toInt();
            int type_in2 = obj["type_in2"].toInt();
            int type_in3 = obj["type_in3"].toInt();
            QString imageData = obj["imageData"].toString();

            if(scale <= 0.05) scale = 0.5;
            if(x < 0) x = 0;
            if(y < 0) y = 0;
            if(x > nW-10) x = nW-10 ;
            if(y > nH-10) y = nH-10 ;

            QImage image = decodeImage(imageData);
            CustomPixmapItem *pixmapItem = new CustomPixmapItem(QPixmap::fromImage(image));
            pixmapItem->setPos(x, y);
            pixmapItem->setScale( scale );
            pixmapItem->setZValue( zvalue );
            pixmapItem->setData( 0, type_in0 );
            pixmapItem->setData( 1, type_in1 );
            pixmapItem->setData( 2, type_in2 );
            pixmapItem->setData( 3, type_in3 );
            pixmapItem->setName( name );

            scene->addItem(pixmapItem);
        }
    }

    return true;
}
