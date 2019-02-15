#ifndef INSTADAM_H
#define INSTADAM_H

#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "project.h"
#include "newproject.h"
#include <QDialog>
#include <QGraphicsItem>
#include <QObject>
#include <QMenuBar>
#include <QMenu>


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

private slots:
    void on_actionOpen_File_triggered();

private slots:
    Project on_actionNew_triggered();

    Project on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_panButton_clicked();

    void on_roundBrush_clicked();

    void on_squareBrush_clicked();

    void on_pushButton_4_clicked();


private:
    Ui::InstaDam *ui;
    newproject *newProject;
    Project currentProject;
    QGraphicsScene *scene;
};




#endif // INSTADAM_H
