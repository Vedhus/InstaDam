#include "instadam.h"
#include "ui_instadam.h"
#include "pixmapops.h"
#include "filtercontrols.h"
#include <string>
#include <iostream>





InstaDam::InstaDam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstaDam)
{
    ui->setupUi(this);
    filterControl = new filterControls();
    connectFilters();
    ui->IdmPhotoViewer->setFilterControls(filterControl);
    ui->IdmMaskViewer->setFilterControls(filterControl);


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


    clearLayout(ui->labelClassLayout);
    for(int i=0; i<currentProject.numLabels(); i++){
          QPushButton *button = new QPushButton();
          button->setText(currentProject.getLabel(i).getText());
          QPalette pal = button->palette();
          pal.setColor(QPalette::ButtonText, currentProject.getLabel(i).getColor());
          pal.setColor(QPalette::Button, currentProject.getLabel(i).getColor());
          button->setAutoFillBackground(true);
          button->setPalette(pal);
          button->update();
          ui->labelClassLayout->addWidget(button);
          qInfo("Button Added!");


    }
   return currentProject;
}

void InstaDam::clearLayout(QLayout * layout) {
    if (! layout)
       return;
    while (auto item = layout->takeAt(0)) {
       delete item->widget();
       clearLayout(item->layout());
    }
 }

Project InstaDam::on_actionOpen_triggered()
{

    QString doc;
    int i = 0;
    // Reading and Loading
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
    if (fileName.isEmpty()){
            return currentProject; // remove that part and add an alert
    }
    else
    {
       this->currentProject = Project();

       QFile file(fileName);
       QStringList list;
       file.open(QIODevice::ReadWrite);
       QTextStream fileStream(&file);

       while(!fileStream.atEnd()){
           QString line = file.readLine(); // to be updated to read all the labels and the colors
           QTextStream(stdout) << line;

           list = line.split("~%");

           QTextStream(stdout) << list.size() << endl;

           Label lb;
           lb.setText(list[0]);
           lb.setColor(QColor(list[1]));
           QTextStream(stdout) <<lb.getColor().name() << list[1] << lb.getColor().name() <<endl;
           currentProject.addLabel(lb);


           i++;

      }
    }

    i = 0;

    clearLayout(ui->labelClassLayout);
    QTextStream(stdout) <<currentProject.numLabels();
    for(int i=0; i<currentProject.numLabels(); i++)
    {
          QPushButton *button = new QPushButton();

          button->setText(currentProject.getLabel(i).getText());
          QPalette pal = button->palette();

          QTextStream(stdout) << currentProject.getLabel(i).getColor().name() << endl;


          pal.setColor(QPalette::ButtonText, currentProject.getLabel(i).getColor());
          pal.setColor(QPalette::Button, currentProject.getLabel(i).getColor());
          button->setAutoFillBackground(true);
          button->setPalette(pal);
          button->update();
          ui->labelClassLayout->addWidget(button);

    }

    return currentProject;
}




//Project InstaDam::on_actionNew_triggered()
//{
//    // find a way to either get a fixed number of labels from the user at the beginning
//    // or to create a flexible array where labels can get added

//    // int const maxNumLabels = 3; // defined in the headers file as static const.

//    while(currentProject.numLabels() < 3){
//        // label names is working
//        QString text = QInputDialog::getText(this, tr("Add a new label"),
//                                             tr("New Label"), QLineEdit::Normal, "cracks");
//        // Color picking is working
//        QColor color = QColorDialog::getColor(Qt::black, this, "Pick a color",  QColorDialog::DontUseNativeDialog);

//        Label lb;
//        lb.setText(text);
//        lb.setColor(color);

//        currentProject.addLabel(lb);
//        // printing out to the console
//        QTextStream(stdout) << text << endl;
//        QTextStream(stdout) << color.name() << endl;

//    }

//    ////////////////////////////////////////////////////////////////////
//        // Saving the file
//        QString outFileName = QFileDialog::getSaveFileName(this,
//               tr("save project"), "../", tr("instadam files (.idpro));; All Files (*)"));

//    //    if (outFileName.isEmpty()){
//    //            return currentProject;
//    //    }
//    //    else {
//        QFile outFile(outFileName);
//        outFile.open(QIODevice::ReadWrite);
//        for(int i=0; i<currentProject.numLabels(); i++)
//        {
//            Label lb = currentProject.getLabel(i);
//            QTextStream(&outFile) << lb.getText();
//            QTextStream(&outFile) << "~%";
//            QTextStream(&outFile) << lb.getColor().name() << endl;
//        }

//   return currentProject;
//}


//Project InstaDam::on_actionOpen_triggered()
//{
//    QString doc;
//    int i = 0;
//    // Reading and Loading
//    QString fileName = QFileDialog::getOpenFileName(this,
//        tr("open project"), "../", tr("instadam files (.idpro));; All Files (*)"));
//    if (fileName.isEmpty()){
//            return currentProject; // remove that part and add an alert
//    }
//    else
//    {
//       QFile file(fileName);
//       QStringList list;
//       file.open(QIODevice::ReadWrite);
//       QTextStream fileStream(&file);

//       while(!fileStream.atEnd()){
//           QString line = file.readLine(); // to be updated to read all the labels and the colors
//           QTextStream(stdout) << line;

//           list = line.split("~%");

//           QTextStream(stdout) << list.size() << endl;
//           if(list.size()==2){
//               Label lb;
//               lb.setText(list[0]);
//               lb.setColor(QColor(list[1]));
//               QTextStream(stdout) << list[0] << endl;
//               QTextStream(stdout) << list[1] << endl;

//               currentProject.addLabel(lb);
//               i++;
//          }
//      }
//    }

//    return currentProject;
//}




void InstaDam::connectFilters()
{
    connect(ui->IdmPhotoViewer, SIGNAL(changedMask(maskTypes)), ui->IdmMaskViewer, SLOT(setImMask(maskTypes)));

    connect(ui->blur_label, SIGNAL(checked(maskTypes)), ui->canny_label, SLOT(otherBoxChecked(maskTypes)));
    connect(ui->blur_label, SIGNAL(checked(maskTypes)), ui->threshold_label, SLOT(otherBoxChecked(maskTypes)));
    ui->blur_label->setMaskType(BLUR);

    connect(ui->blur_label, SIGNAL(checked(maskTypes)), ui->IdmPhotoViewer, SLOT(setImMask(maskTypes)));


    connect(ui->canny_label, SIGNAL(checked(maskTypes)), ui->blur_label, SLOT(otherBoxChecked(maskTypes)));
    connect(ui->canny_label, SIGNAL(checked(maskTypes)), ui->threshold_label, SLOT(otherBoxChecked(maskTypes)));
    ui->canny_label->setMaskType(CANNY);
    ui->canny_label->manualSelect();
    connect(ui->canny_label, SIGNAL(checked(maskTypes)), ui->IdmPhotoViewer, SLOT(setImMask(maskTypes)));


    connect(ui->threshold_label, SIGNAL(checked(maskTypes)), ui->canny_label, SLOT(otherBoxChecked(maskTypes)));
    connect(ui->threshold_label, SIGNAL(checked(maskTypes)), ui->blur_label, SLOT(otherBoxChecked(maskTypes)));
    ui->threshold_label->setMaskType(THRESHOLD);

    connect(ui->threshold_label, SIGNAL(checked(maskTypes)), ui->IdmPhotoViewer, SLOT(setImMask(maskTypes)));

    connect(ui->IdmPhotoViewer, SIGNAL(zoomed(int, float, QPointF)), ui->IdmMaskViewer, SLOT(zoomedInADifferentView(int, float, QPointF)));
    connect(ui->IdmMaskViewer, SIGNAL(zoomed(int, float, QPointF)), ui->IdmPhotoViewer, SLOT(zoomedInADifferentView(int, float, QPointF)));

    connect(ui->IdmPhotoViewer, SIGNAL(loadedPhoto()), this, SLOT(resetPixmapButtons()));
}
void InstaDam::resetPixmapButtons()
{
    cv::Mat thumbnail = ui->IdmPhotoViewer->cvThumb;
    ui->blur_label->resetPixmaps(filterControl->thumb2pixmap(thumbnail, BLUR));
    qInfo("1");
    ui->canny_label->resetPixmaps(filterControl->thumb2pixmap(thumbnail, CANNY));
    qInfo("2");
    ui->threshold_label->resetPixmaps(filterControl->thumb2pixmap( thumbnail, THRESHOLD));
    qInfo("3");
}

void InstaDam::on_actionSave_triggered()
{
// Saving the file
    QString outFileName = QFileDialog::getSaveFileName(this,
           tr("Save Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
    if (QFileInfo(outFileName).suffix() != QString("idpro"))
        outFileName = outFileName +QString(".idpro");

    QFile outFile(outFileName);
    outFile.open(QIODevice::ReadWrite);
    for(int i=0; i<currentProject.numLabels(); i++)
    {
        Label lb = currentProject.getLabel(i);
        QTextStream(&outFile) << lb.getText();
        QTextStream(&outFile) << "~%";
        QTextStream(&outFile) << lb.getColor().name() << "~%"<<endl;
    }

}


// File open opens a dialog window and allows a user to pick an image file to open. Subsequently,
// a list of images in the directory is established and the image fileId in the list is established
// The function then calls open file and labels

void InstaDam::on_actionOpen_File_triggered()
{
    QString filename_temp = QFileDialog::getOpenFileName(this, tr("Open Image"), "../", tr("Image Files (*.jpg *.png *.JPG *PNG *jpeg *JPEG );; All Files (*)"));
    QString ext = QFileInfo(filename_temp).suffix();
    if(!ext.compare("png", Qt::CaseInsensitive) || !ext.compare("jpg", Qt::CaseInsensitive) || !ext.compare("jpeg", Qt::CaseInsensitive))
    {
        this->filename = filename_temp;
        this->file = QFileInfo(filename);
        this->path = file.dir();
        this->imagesList = path.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.JPEG", QDir::Files);

        if (imagesList.empty())
                assertError("That doesn't seem to be a valid image file.");
        else
        {
            int counter = 0;
            foreach(QString filename, imagesList) {
               if (file.completeBaseName()==filename)
                   break;
                counter++;
            }
            fileId = counter;
            openFile_and_labels();
        }
    }
    else {
       assertError("That doesn't seem to be a valid image file.");
    }


}

// Generates the file name for the next file in the folder

void InstaDam::on_saveAndNext_clicked()
{

    if (imagesList.empty())
            assertError("No file loaded! Please go to File->Open File and select an image to open");
    else
    {
        saveFile();
        int newId = (fileId+1)%imagesList.size();
        if (newId)
        {
            fileId = newId;
            this->filename = path.absolutePath()+"/"+imagesList[fileId];
            this->file = QFileInfo(this->filename);
            openFile_and_labels();

        }

    }

}

void InstaDam::saveFile()
{

    QFile file(labelFile);
    file.open(QIODevice::WriteOnly);
    ui->IdmPhotoViewer->labels->pixmap().save(&file, "PNG");
}



void InstaDam::assertError(std::string errorMessage)
{
    QMessageBox *messageBox = new QMessageBox;
    messageBox->critical(0,"Error",QString::fromStdString(errorMessage));
    messageBox->setFixedSize(500,200);

}

// Uses name of current file and generates name of label image.
void InstaDam::generateLabelFileName()
{

    QString baseName = this->file.baseName();
    QString labelName = baseName+"_label.png";
    QString labelPath = this->path.absolutePath()+"/labels/";
    if (!QDir(labelPath).exists())
    {
        QDir().mkdir(labelPath);
    }

    this->labelFile = labelPath+labelName;
    this->path = file.dir();

}

// This function uses the defined QStringList of images in the path as well as the id of the current file
// and opens the file. If labels exist, the labels are opened
void InstaDam::openFile_and_labels()
{

    QString ext = file.suffix();
    QString labelNameTemp = QString::null;

    //Open labels
    generateLabelFileName();
    if (QFileInfo(labelFile).isFile())
    {
        labelNameTemp = labelFile;
        qInfo(labelNameTemp.toUtf8().constData());
        qInfo("I will open the labels!");
    }
    ui->IdmPhotoViewer->setPhotoFromFile(filename, labelNameTemp);
    ui->IdmMaskViewer->LinkToPhotoViewer(ui->IdmPhotoViewer);
}

//    else if(!ext.compare("idam"))
//    {
//        QTextStream(stdout) << "idam attempted to be opened";
//        QFile file(filename);
//        file.open(QIODevice::ReadOnly);
//        QDataStream in(&file);
//        QPixmap photoPixmap, labelPixmap;
//        in >> photoPixmap >> labelPixmap;
//        ui->IdmPhotoViewer->setPhotoFromPixmap(photoPixmap, labelPixmap);
//    }
//    else{
//        QTextStream(stdout) << "something other than an image or idam being opened, need to throw error to user here";
//    }





void InstaDam::on_panButton_clicked()
{
    ui->IdmPhotoViewer->setPanMode();
    ui->IdmMaskViewer->setPanMode();
}

void InstaDam::on_roundBrush_clicked()
{
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
    ui->IdmMaskViewer->setBrushMode(Qt::RoundCap);
}

void InstaDam::on_squareBrush_clicked()
{
    ui->IdmPhotoViewer->setBrushMode(Qt::SquareCap);
    ui->IdmMaskViewer->setBrushMode(Qt::SquareCap);
}

void InstaDam::on_pushButton_14_clicked()
{

    filterDialog* dialogs = new filterDialog(ui->IdmPhotoViewer->selectedMask, filterControl, ui->IdmPhotoViewer);
    dialogs->show();
}

void InstaDam::on_actionSave_File_triggered()
{
    saveFile();
//    QTextStream(stdout) << "saving current canvas as idamfile format";
//    QString outFileName = QFileDialog::getSaveFileName(this, tr("save images"), "../", tr("instadam files (.idam));; All Files (*)"));
//    QFile outFile(outFileName);
//    outFile.open(QIODevice::WriteOnly);
//    //actually write to the file here
//    QDataStream out(&outFile);
//    out << ui->IdmPhotoViewer->photo->pixmap() << ui->IdmPhotoViewer->labels->pixmap();
//    outFile.close();
}



