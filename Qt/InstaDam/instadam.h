#ifndef INSTADAM_H
#define INSTADAM_H

#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "project.h"
#include <QDialog>
#include <QGraphicsItem>
#include <QObject>
#include <QMenuBar>
#include <QMenu>
#include <QColorDialog>
#include <QInputDialog>
#include <QFrame>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QtCore>
#include <QBitmap>
#include <QPainter>

#include <iostream>
#include <string>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2/imgproc.hpp"

class filterControls;

enum maskTypes{CANNY, THRESHOLD, BLUR, OTHER};
enum threshold_or_filter{THRESH, FILTER};

namespace Ui {
class InstaDam;
}

class InstaDam : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstaDam(QWidget *parent = nullptr);
    ~InstaDam();
    void fileOpen();
    void connectFilters();
    filterControls * filterControl;

private slots:
    void on_actionOpen_File_triggered();

private slots:
    Project on_actionNew_triggered();

    Project on_actionOpen_triggered();

    void on_actionSave_triggered();



    void on_panButton_clicked();

    void on_roundBrush_clicked();

    void on_squareBrush_clicked();

    void on_pushButton_14_clicked();

public slots:
    void resetPixmapButtons();


private:
    Ui::InstaDam *ui;
    Project currentProject;
    QGraphicsScene *scene;
};




#endif // INSTADAM_H
