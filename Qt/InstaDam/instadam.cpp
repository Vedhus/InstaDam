#include "instadam.h"
#include "ui_instadam.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string
#include <stdio.h>



InstaDam::InstaDam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstaDam)
{
    ui->setupUi(this);
}

InstaDam::~InstaDam()
{
    delete ui;
}


void InstaDam::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName();
    QPixmap im = QPixmap(filename);
    ui->IdmPhotoViewer->setPhoto(im);

}
