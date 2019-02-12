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
    // find a way to either get a fixed number of labels from the user at the beginning
    // or to create a flexible array where labels can get added

    // int const maxNumLabels = 3; // defined in the headers file as static const.

    while(currentProject.numLabels() < 3){
        // label names is working
        QString text = QInputDialog::getText(this, tr("Add a new label"),
                                             tr("New Label"), QLineEdit::Normal, "cracks");
        // Color picking is working
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick a color",  QColorDialog::DontUseNativeDialog);

        Label lb;
        lb.setText(text);
        lb.setColor(color);

        currentProject.addLabel(lb);
        // printing out to the console
        QTextStream(stdout) << text << endl;
        QTextStream(stdout) << color.name() << endl;

    }

    ////////////////////////////////////////////////////////////////////
        // Saving the file
        QString outFileName = QFileDialog::getSaveFileName(this,
               tr("save project"), "../", tr("instadam files (.idpro));; All Files (*)"));

    //    if (outFileName.isEmpty()){
    //            return currentProject;
    //    }
    //    else {
        QFile outFile(outFileName);
        outFile.open(QIODevice::ReadWrite);
        for(int i=0; i<currentProject.numLabels(); i++)
        {
            Label lb = currentProject.getLabel(i);
            QTextStream(&outFile) << lb.getText();
            QTextStream(&outFile) << "~%";
            QTextStream(&outFile) << lb.getColor().name() << endl;
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

    Project currentProject;  // check how to feed the current project to this function
    // Saving a file
    QString outFileName = QFileDialog::getSaveFileName(this,
           tr("save project"), "../", tr("instadam files (.idpro));; All Files (*)"));

    if (outFileName.isEmpty()){
            return;
    }
    else {
        QFile outFile(outFileName);
        outFile.open(QIODevice::ReadWrite);
        for(int i=0; i<3; i++){
            Label lb = currentProject.getLabel(i);
            QTextStream(&outFile) << lb.getText();
            QTextStream(&outFile) << "~%";
            QTextStream(&outFile) << lb.getColor().name() << endl;
        }
     }
}

void InstaDam::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName();
    QPixmap im = QPixmap(filename);
    ui->IdmPhotoViewer->setPhoto(im);

}
