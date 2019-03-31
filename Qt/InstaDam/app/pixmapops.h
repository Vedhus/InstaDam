#ifndef PIXMAPOPS_H
#define PIXMAPOPS_H
#include "photoviewer.h"


QPixmap testPixmap(int, int, int, int);
QPixmap maskPixmaps(QPixmap, QPixmap, QPixmap, brushTypes );
QPixmap directPixmaps(QPixmap, QPixmap, brushTypes);
QPixmap joinPixmaps(QPixmap, QPixmap, QPainter::CompositionMode);

cv::Mat QImageToCvMat(QImage, bool);

cv::Mat QPixmapToCvMat(QPixmap, bool);


#endif // PIXMAPOPS_H
