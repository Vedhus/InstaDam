#ifndef INSTADAM_H
#define INSTADAM_H

#include <QMainWindow>
#include <QDialog>
#include <QGraphicsItem>
#include <QObject>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>

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

private:
    Ui::InstaDam *ui;
    QGraphicsScene *scene;
};

#endif // INSTADAM_H
