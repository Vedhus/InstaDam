#include "pixmapops.h"


QPixmap testPixmap(int r = 255,int g = 0,int b = 0, int a = 255)
{
    QPixmap px = QPixmap(200,200);
    QColor color  = QColor(r,g,b,a);
    px.fill(color);
    return px;
}

QPixmap maskPixmaps(QPixmap p1, QPixmap p2, QPixmap mask, toolTypes brushType)
{
    QPixmap maskp2 = joinPixmaps(mask, p2, QPainter::CompositionMode_SourceIn);
    return directPixmaps(p1,maskp2, brushType);

}

QPixmap directPixmaps(QPixmap p1, QPixmap p2, toolTypes brushType)
{
    QPainter::CompositionMode mode;
    if (brushType == ERASER)
        mode = QPainter::CompositionMode_DestinationOut;
    else
        mode = QPainter::CompositionMode_SourceOver;
    return joinPixmaps(p1, p2, mode);
}

QPixmap joinPixmaps(QPixmap p1, QPixmap p2, QPainter::CompositionMode mode= QPainter::CompositionMode_SourceOver)
{

    QPixmap result =  QPixmap(p1.size().expandedTo(p2.size()));
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(QPoint(), p1);
    painter.setCompositionMode(mode);
    painter.drawPixmap(result.rect(), p2, p2.rect());
    painter.end();
    return result;
}
