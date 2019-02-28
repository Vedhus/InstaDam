#ifndef MASKVIEWER_H
#define MASKVIEWER_H


#include <QGraphicsView>
#include <QMainWindow>
#include "photoviewer.h"
#include "filters.h"
#include "maskobjects.h"

namespace Ui {
class MaskViewer;
}

class MaskViewer : public PhotoViewer
{
    Q_OBJECT

public:
    MaskViewer(QWidget *parent = 0);
    void LinkToPhotoViewer(PhotoViewer *);

    maskObjects *maskObject;
    QPixmap qImg;
    QPixmap qAlpha;
    PhotoViewer * photoViewer;
    void setMaskPixmap();
public slots:
    void setImMask(maskTypes);

private:
    Ui::MaskViewer *ui;
};

#endif // MASKVIEWER_H


