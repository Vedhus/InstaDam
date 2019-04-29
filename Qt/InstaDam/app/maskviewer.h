#ifndef MASKVIEWER_H
#define MASKVIEWER_H

#include <QGraphicsView>
#include <QMainWindow>

#include "photoviewer.h"

namespace Ui {
class MaskViewer;
}

class MaskViewer : public PhotoViewer {
    Q_OBJECT

 public:
    explicit MaskViewer(QWidget *parent = nullptr);
    void LinkToPhotoViewer(PhotoViewer *viewer);

    //maskObjects *maskObject;
    QPixmap qImg;
    QPixmap qAlpha;
    PhotoViewer *photoViewer;
    //void setMaskPixmap();
 public slots:
    void setImMask(EnumConstants::maskTypes filterName);

 private:
    Ui::MaskViewer *ui;
};

#endif  // MASKVIEWER_H
