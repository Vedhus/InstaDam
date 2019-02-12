#ifndef MASKVIEWER_H
#define MASKVIEWER_H


#include <QGraphicsView>
#include <QMainWindow>
#include "photoviewer.h"

namespace Ui {
class MaskViewer;
}

class MaskViewer : public PhotoViewer
{
    Q_OBJECT

public:
    MaskViewer(QWidget *parent = 0);

private:
    Ui::MaskViewer *ui;
};

#endif // MASKVIEWER_H


