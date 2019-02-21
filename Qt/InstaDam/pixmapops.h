#ifndef PIXMAPOPS_H
#define PIXMAPOPS_H
#include "photoviewer.h"


QPixmap testPixmap(int, int, int, int);
QPixmap maskPixmaps(QPixmap, QPixmap, QPixmap, brushTypes );
QPixmap directPixmaps(QPixmap, QPixmap, brushTypes);
QPixmap joinPixmaps(QPixmap, QPixmap, QPainter::CompositionMode);






#endif // PIXMAPOPS_H
