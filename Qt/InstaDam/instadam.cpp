#include "instadam.h"
#include "ui_instadam.h"
#include "pixmapops.h"
#include "filtercontrols.h"

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

    ui->IdmPhotoViewer->setPhoto(filename);
    ui->IdmMaskViewer->LinkToPhotoViewer(ui->IdmPhotoViewer);



}

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
