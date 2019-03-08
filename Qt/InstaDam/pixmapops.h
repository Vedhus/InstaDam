#ifndef PIXMAPOPS_H
#define PIXMAPOPS_H
#include "photoviewer.h"


QPixmap testPixmap(int, int, int, int);
QPixmap maskPixmaps(QPixmap, QPixmap, QPixmap, toolTypes );
QPixmap directPixmaps(QPixmap, QPixmap, toolTypes);
QPixmap joinPixmaps(QPixmap, QPixmap, QPainter::CompositionMode);






#endif // PIXMAPOPS_H
