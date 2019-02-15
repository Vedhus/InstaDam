#include "instadam.h"
#include "ui_instadam.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <iostream>
#include <string>
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

Project InstaDam::on_actionNew_triggered()
{
    currentProject.resetLabels();
    newProject = new newproject(this);
    newProject->setModal(true);
    newProject->exec();
    currentProject = newProject->newPr;
    for(int i=0; i<currentProject.numLabels(); i++){
        if(i==0){
          ui->pushButton_4->setText(currentProject.getLabel(i).getText());
          QPalette pal = ui->pushButton_4->palette();
          pal.setColor(QPalette::Button, currentProject.getLabel(i).getColor());
          ui->pushButton_4->setAutoFillBackground(true);
          ui->pushButton_4->setPalette(pal);
          ui->pushButton_4->update();
        }

        if(i==1){
          ui->pushButton_5->setText(currentProject.getLabel(i).getText());
          QPalette pal = ui->pushButton_5->palette();
          pal.setColor(QPalette::Button, currentProject.getLabel(i).getColor());
          ui->pushButton_5->setAutoFillBackground(true);
          ui->pushButton_5->setPalette(pal);
          ui->pushButton_5->update();
        }

        if(i==2){
          ui->pushButton_6->setText(currentProject.getLabel(i).getText());
          QPalette pal = ui->pushButton_6->palette();
          pal.setColor(QPalette::Button, currentProject.getLabel(i).getColor());
          ui->pushButton_6->setAutoFillBackground(true);
          ui->pushButton_6->setPalette(pal);
          ui->pushButton_6->update();
        }

        if(i==3){
          ui->pushButton_7->setText(currentProject.getLabel(i).getText());
          QPalette pal = ui->pushButton_7->palette();
          pal.setColor(QPalette::Button, currentProject.getLabel(i).getColor());
          ui->pushButton_7->setAutoFillBackground(true);
          ui->pushButton_7->setPalette(pal);
          ui->pushButton_7->update();
        }
    }

   return currentProject;
}



Project InstaDam::on_actionOpen_triggered()
{
    QString doc;
    int i = 0;
    // Reading and Loading
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("open project"), "../", tr("instadam files (.idpro));; All Files (*)"));
    if (fileName.isEmpty()){
            return currentProject; // remove that part and add an alert
    }
    else
    {
       QFile file(fileName);
       QStringList list;
       file.open(QIODevice::ReadWrite);
       QTextStream fileStream(&file);

       while(!fileStream.atEnd()){
           QString line = file.readLine(); // to be updated to read all the labels and the colors
           QTextStream(stdout) << line;

           list = line.split("~%");

           QTextStream(stdout) << list.size() << endl;
           if(list.size()==2){
               Label lb;
               lb.setText(list[0]);
               lb.setColor(QColor(list[1]));
               QTextStream(stdout) << list[0] << endl;
               QTextStream(stdout) << list[1] << endl;

               currentProject.addLabel(lb);
               i++;
          }
      }
    }

    return currentProject;
}


void InstaDam::on_actionSave_triggered()
{
// Saving the file
    QString outFileName = QFileDialog::getSaveFileName(this,
           tr("save project"), "../", tr("instadam files (.idpro));; All Files (*)"));

    QFile outFile(outFileName);
    outFile.open(QIODevice::ReadWrite);
    for(int i=0; i<currentProject.numLabels(); i++)
    {
        Label lb = currentProject.getLabel(i);
        QTextStream(&outFile) << lb.getText();
        QTextStream(&outFile) << "~%";
        QTextStream(&outFile) << lb.getColor().name() << endl;
    }

}

void InstaDam::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName();
    QPixmap im = QPixmap(filename);
    ui->IdmPhotoViewer->setPhoto(im);

}

void InstaDam::on_panButton_clicked()
{
    ui->IdmPhotoViewer->setPanMode();
}

void InstaDam::on_roundBrush_clicked()
{
    qInfo("Round");
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
}

void InstaDam::on_squareBrush_clicked()
{
    ui->IdmPhotoViewer->setBrushMode(Qt::SquareCap);
}

void InstaDam::on_pushButton_4_clicked()
{

}

