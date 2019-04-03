#ifndef PIXMAPOPS_H
#define PIXMAPOPS_H
#include "photoviewer.h"

QPixmap testPixmap(int r, int g, int b, int a);
QPixmap maskPixmaps(QPixmap p1, QPixmap p2, QPixmap mask, brushTypes brushType);
QPixmap directPixmaps(QPixmap p1, QPixmap p2, brushTypes brushType);
QPixmap joinPixmaps(QPixmap p1, QPixmap p2,
                    QPainter::CompositionMode mode = QPainter::CompositionMode_SourceOver);

cv::Mat QImageToCvMat(QImage inImage, bool inCloneImageData = true);

//cv::Mat QPixmapToCvMat(QPixmap, bool);


#endif  // PIXMAPOPS_H
