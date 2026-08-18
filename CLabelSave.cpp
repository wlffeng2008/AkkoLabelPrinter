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

bool CLabelSave::saveSceneWithImages(CustomScene *scene, const QString &filePath, int paperWidth, int paperHeight)
{
    QJsonArray itemsArray;
    QJsonObject paperObj;
    paperObj["name"]="paperset";
    paperObj["type"]="paperset";
    paperObj["width"]=paperWidth;
    paperObj["height"]=paperHeight;
    itemsArray.append(paperObj);

    for (auto item : scene->items())
    {
        if (auto textItem = dynamic_cast<CustomTextItem*>(item))
        {
            QJsonObject textObj;
            textObj["name"] = textItem->getName();
            textObj["type"] = "text";
            textObj["text"] = textItem->toPlainText();

            QFont tf = textItem->font();
            textObj["font"] = tf.family();
            textObj["style"] = tf.styleName();
            textObj["bold"] = tf.bold();
            textObj["size"] = tf.pointSize();

            textObj["x"] = textItem->pos().x();
            textObj["y"] = textItem->pos().y();
            textObj["w"] = textItem->getItemRect().width();
            textObj["h"] = textItem->getItemRect().height();
            textObj["scale"] = textItem->scale();
            textObj["zvalue"]   = textItem->zValue();
            textObj["type_in0"] = textItem->data( 0 ).toInt();
            textObj["type_in1"] = textItem->data( 1 ).toInt();
            textObj["type_in2"] = textItem->data( 2 ).toInt();
            textObj["type_in3"] = textItem->data( 3 ).toInt();
            itemsArray.append(textObj);
        }
        else if (auto pixmapItem = dynamic_cast<CustomPixmapItem*>(item))
        {
            QJsonObject pixmapObj;
            pixmapObj["name"] = pixmapItem->getName();
            pixmapObj["type"] = "pixmap";
            pixmapObj["x"] = pixmapItem->pos().x();
            pixmapObj["y"] = pixmapItem->pos().y();
            pixmapObj["w"] = pixmapItem->getItemRect().width();
            pixmapObj["h"] = pixmapItem->getItemRect().height();
            pixmapObj["scale"] = pixmapItem->scale();
            pixmapObj["zvalue"]   = pixmapItem->zValue();
            pixmapObj["type_in0"] = pixmapItem->data( 0 ).toInt();
            pixmapObj["type_in1"] = pixmapItem->data( 1 ).toInt();
            pixmapObj["type_in2"] = pixmapItem->data( 2 ).toInt();
            pixmapObj["type_in3"] = pixmapItem->data( 3 ).toInt();

            QImage image = pixmapItem->pixmap().toImage();
            QByteArray imageData = encodeImage(image);
            pixmapObj["imageData"] = QString::fromLatin1(imageData);

            itemsArray.append(pixmapObj);
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

bool CLabelSave::loadSceneWithImages(CustomScene *scene, const QString &filePath, int &paperWidth, int &paperHeight)
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

    paperWidth =100;
    paperHeight=100;

    for( const QJsonValue &value : std::as_const(itemsArray) )
    {
        QJsonObject obj = value.toObject();
        QString type = obj["type"].toString();
        QString name = obj["name"].toString();

        if (type == "paperset")
        {
            paperWidth = obj["width"].toInt();
            paperHeight = obj["height"].toInt();
        }

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
