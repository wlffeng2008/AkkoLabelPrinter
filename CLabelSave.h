#ifndef CLABELSAVE_H
#define CLABELSAVE_H

#include <QImage>
#include <QByteArray>
#include <QString>
#include <QGraphicsScene>

class CustomScene;

class CLabelSave
{
public:
    explicit CLabelSave();
    virtual ~CLabelSave();

    static QImage decodeImage( const QString &imageData );
    static QByteArray encodeImage( const QImage &image );
    static bool saveSceneWithImages( CustomScene *scene, const QString &filePath);
    static bool loadSceneWithImages( CustomScene *scene, const QString &filePath);
};

#endif // CLABELSAVE_H
