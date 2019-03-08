#include "instadam.h"
#include "ui_instadam.h"
#include "pixmapops.h"
#include "filtercontrols.h"
#include "labelButton.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include "Selector/selectItem.h"
#include "Selector/ellipseSelect.h"
#include "Selector/rectangleSelect.h"
#include "Selector/polygonSelect.h"
#include "Selector/freeDrawSelect.h"
#include "Selector/freeDrawErase.h"
#include "Selector/commands.h"
#ifdef WASM_BUILD
#include "htmlFileHandler/qhtml5file.h"
#endif

int SelectItem::ID = 0;
QSize SelectItem::myBounds = QSize(0,0);
InstaDam::InstaDam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstaDam)
{
    ui->setupUi(this);
    filterControl = new filterControls();
    connectFilters();
    ui->IdmPhotoViewer->setFilterControls(filterControl);
    ui->IdmMaskViewer->setFilterControls(filterControl);
    undoStack = new QUndoStack(this);
    currentSelectType = EllipseObj;
    scene = ui->IdmPhotoViewer->scene;
    maskScene = ui->IdmMaskViewer->scene;
    currentItem = nullptr;
    currentLabel = nullptr;
    connect(scene, SIGNAL(pointClicked(viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)), this,
            SLOT(processPointClicked(viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)));
    connect(scene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(scene, SIGNAL(mouseReleased(viewerTypes, QPointF, QPointF, const Qt::MouseButton)), this,
            SLOT(processMouseReleased(viewerTypes, QPointF, QPointF, const Qt::MouseButton)));
    connect(scene, SIGNAL(keyPressed(viewerTypes, const int)), this,
            SLOT(processKeyPressed(viewerTypes, const int)));
    connect(maskScene, SIGNAL(pointClicked(viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)), this,
            SLOT(processPointClicked(viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)));
    connect(maskScene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(maskScene, SIGNAL(mouseReleased(viewerTypes, QPointF, QPointF, const Qt::MouseButton)), this,
            SLOT(processMouseReleased(viewerTypes, QPointF, QPointF, const Qt::MouseButton)));
    connect(maskScene, SIGNAL(keyPressed(viewerTypes, const int)), this,
            SLOT(processKeyPressed(viewerTypes, const int)));

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui->menuEdit->addAction(undoAction);
    ui->menuEdit->addAction(redoAction);
    blankForm = new Ui::blankForm;
    freeSelectForm = new Ui::freeSelectForm;
    polygonSelectForm = new Ui::polygonSelectForm;
    controlLayout = new QGridLayout(ui->selectControlFrame);
    blankWidget = new QWidget(ui->selectControlFrame);
    blankForm->setupUi(blankWidget);
    freeSelectWidget = new QWidget(ui->selectControlFrame);
    freeSelectForm->setupUi(freeSelectWidget);
    connect(freeSelectForm->roundBrushButton, SIGNAL(clicked()), this,
            SLOT(roundBrushButtonClicked()));
    connect(freeSelectForm->squareBrushButton, SIGNAL(clicked()), this,
            SLOT(squareBrushButtonClicked()));
    connect(freeSelectForm->drawButton, SIGNAL(clicked()), this,
            SLOT(toggleDrawing()));
    connect(freeSelectForm->eraseButton, SIGNAL(clicked()), this,
            SLOT(toggleErasing()));
    connect(freeSelectForm->brushSizeSlider, SIGNAL(valueChanged(int)), freeSelectForm->brushSizeSpinner,
            SLOT(setValue(int)));
    connect(freeSelectForm->brushSizeSpinner, SIGNAL(valueChanged(int)), freeSelectForm->brushSizeSlider,
            SLOT(setValue(int)));
    connect(freeSelectForm->brushSizeSlider, SIGNAL(valueChanged(int)), this,
            SLOT(setCurrentBrushSize(int)));
    freeSelectForm->brushSizeSlider->setValue(currentBrushSize);
    freeSelectWidget->hide();
    polygonSelectWidget = new QWidget(ui->selectControlFrame);
    polygonSelectForm->setupUi(polygonSelectWidget);
    connect(polygonSelectForm->finishPolygonButton, SIGNAL(clicked()), this,
            SLOT(finishPolygonButtonClicked()));
    connect(polygonSelectForm->insertPointButton, SIGNAL(clicked()), this,
            SLOT(setInsert()));
    polygonSelectWidget->hide();
    controlLayout->addWidget(blankWidget);

#ifdef WASM_BUILD
    addConnector("Load File", [&]() {
            QHtml5File::load("*", [&](const QByteArray &content, const QString &fileName){
                fileContent = content;
                filename = fileName;
                imagesList = (QStringList() << filename);

                openFile_and_labels();
            });
        });
#endif
}

#ifdef WASM_BUILD
void InstaDam::addConnector(QString text, std::function<void ()> onActivate){
    openFile = new MyConnector;
    openFile->onActivate = onActivate;
}

#endif
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
        Label *label = currentProject.getLabel(i);
        LabelButton *button = new LabelButton(label);
        button->setText(label->getText());
        QPalette pal = button->palette();
        pal.setColor(QPalette::ButtonText, Qt::black);
        pal.setColor(QPalette::Button, label->getColor());
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        connect(button, SIGNAL(cclicked(Label*)), this, SLOT(setCurrentLabel(Label*)));
        ui->labelClassLayout->addWidget(button);
        qInfo("Button Added!");
    }
   return currentProject;
}

void InstaDam::setCurrentLabel(LabelButton *button){
    currentLabel = button->myLabel;
}
void InstaDam::setCurrentLabel(Label *label){
    //cout << "SET LABEL" << endl;
    currentLabel = label;
    //cout << label->getText().toStdString() << endl;
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
        Label *label = currentProject.getLabel(i);
        LabelButton *button = new LabelButton(label);

        button->setText(label->getText());
        QPalette pal = button->palette();

        QTextStream(stdout) << label->getColor().name() << endl;


        pal.setColor(QPalette::ButtonText, Qt::black);
        pal.setColor(QPalette::Button, label->getColor());
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        connect(button, SIGNAL(cclicked(Label*)), this, SLOT(setCurrentLabel(Label*)));
//        connect(button, SIGNAL(clicked()), this, SLOT(setCurrentLabel(button.myLabel)));
        ui->labelClassLayout->addWidget(button);

    }

    return currentProject;
}

void InstaDam::toggleDrawing(){
    drawing = true;
    freeSelectForm->eraseButton->setChecked(false);
    freeSelectForm->drawButton->setChecked(true);
}
void InstaDam::toggleErasing(){
    drawing = false;
    freeSelectForm->eraseButton->setChecked(true);
    freeSelectForm->drawButton->setChecked(false);
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

void InstaDam::setCurrentBrushSize(int size){
    currentBrushSize = size;
}

void InstaDam::on_actionSave_triggered()
{
// Saving the file
#ifdef WASM_BUILD
    QByteArray outFile;
#else
    QString outFileName = QFileDialog::getSaveFileName(this,
           tr("Save Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
    if (QFileInfo(outFileName).suffix() != QString("idpro"))
        outFileName = outFileName +QString(".idpro");

    QFile outFile(outFileName);
    outFile.open(QIODevice::ReadWrite);
#endif
    for(int i=0; i<currentProject.numLabels(); i++)
    {
        Label lb = (*currentProject.getLabel(i));
        QTextStream(&outFile) << lb.getText();
        QTextStream(&outFile) << "~%";
        QTextStream(&outFile) << lb.getColor().name() << "~%"<<endl;
    }
#ifdef WASM_BUILD
    QHtml5File::save(outFile, "myproject.idpro");
#endif
}


void InstaDam::on_actionOpen_File_triggered()
{
#ifdef WASM_BUILD
    openFile->onActivate();
#else
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
#endif
}

#ifdef WASM_BUILD
void InstaDam::loadRawImage(){


    openFile_and_labels();
}
#endif
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
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    ui->IdmPhotoViewer->labels->pixmap().save(&buffer, "PNG");

    QFile file(labelFile);
    file.open(QIODevice::WriteOnly);
    ui->IdmPhotoViewer->labels->pixmap().save(&file, "PNG");
}



void InstaDam::assertError(std::string errorMessage)
{
    QMessageBox *messageBox = new QMessageBox;
    messageBox->critical(nullptr,"Error",QString::fromStdString(errorMessage));
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

#ifdef WASM_BUILD
    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromByteArray(fileContent,labelNameTemp);
#else
    //Open labels
    generateLabelFileName();
    if (QFileInfo(labelFile).isFile())
    {
        labelNameTemp = labelFile;
        qInfo(labelNameTemp.toUtf8().constData());
        qInfo("I will open the labels!");
    }
    scene->update();
    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromFile(filename, labelNameTemp);
#endif
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

void InstaDam::on_rectangleSelectButton_clicked(){
    scene->inactiveAll();
    maskScene->inactiveAll();
    currentItem = nullptr;
    switch(currentSelectType){
        case PolygonObj:
            if(ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")){
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            break;
        case FreedrawObj:
            if(ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")){
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            break;
        case FreeeraseObj:
        case RectangleObj:
        case EllipseObj:
            break;
    }
    blankWidget->show();
    currentSelectType = RectangleObj;
    scene->update();
    maskScene->update();
}

void InstaDam::on_ellipseSelectButton_clicked(){
    scene->inactiveAll();
    maskScene->inactiveAll();
    currentItem = nullptr;
    switch(currentSelectType){
        case PolygonObj:
            if(ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")){
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            break;
        case FreedrawObj:
            if(ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")){
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            break;
        case FreeeraseObj:
        case RectangleObj:
        case EllipseObj:
            break;
    }
    blankWidget->show();
    currentSelectType = EllipseObj;
    scene->update();
    maskScene->update();
}

void InstaDam::on_polygonSelectButton_clicked(){
    scene->inactiveAll();
    maskScene->inactiveAll();
    currentItem = nullptr;
    switch(currentSelectType){
        case EllipseObj:
        case RectangleObj:
            if(ui->selectControlFrame->findChild<QWidget*>("blankForm")){
                controlLayout->removeWidget(blankWidget);
                blankWidget->hide();
            }
            controlLayout->addWidget(polygonSelectWidget);
            break;
        case FreedrawObj:
            if(ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")){
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            controlLayout->addWidget(polygonSelectWidget);
            break;
        case FreeeraseObj:
        case PolygonObj:
            break;
    }
    polygonSelectWidget->show();
    polygonSelectForm->polygonMessageBox->setPlainText(polygonBaseInstruction);
    currentSelectType = PolygonObj;
    scene->update();
}

void InstaDam::on_freeSelectButton_clicked(){
    scene->inactiveAll();
    currentItem = nullptr;
    switch(currentSelectType){
        case EllipseObj:
        case RectangleObj:
            if(ui->selectControlFrame->findChild<QWidget*>("blankForm")){
                controlLayout->removeWidget(blankWidget);
                blankWidget->hide();
            }
            controlLayout->addWidget(freeSelectWidget);
            break;
        case PolygonObj:
            if(ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")){
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(freeSelectWidget);
            break;
        case FreeeraseObj:
        case FreedrawObj:
            break;
    }
    freeSelectWidget->show();
    currentSelectType = FreedrawObj;
    scene->update();
    maskScene->update();
}

void InstaDam::roundBrushButtonClicked()
{
    brushMode = Qt::RoundCap;
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
    ui->IdmMaskViewer->setBrushMode(Qt::RoundCap);
}

void InstaDam::squareBrushButtonClicked()
{
    brushMode = Qt::SquareCap;
    ui->IdmPhotoViewer->setBrushMode(Qt::SquareCap);
    ui->IdmMaskViewer->setBrushMode(Qt::SquareCap);
}

void InstaDam::on_pushButton_14_clicked()
{

    filterDialog* dialogs = new filterDialog(ui->IdmPhotoViewer->selectedMask, filterControl, ui->IdmPhotoViewer);
    dialogs->show();
}

void InstaDam::setInsert(){
    insertVertex = true;
    vertex1 = -1;
    vertex2 = -1;
    polygonSelectForm->polygonMessageBox->setPlainText("Click the vertices between which you want to insert a point.");
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


void InstaDam::processPointClicked(viewerTypes type, SelectItem *item, QPointF pos, const Qt::MouseButton button){
    //cout << "CLICK " << type<< endl;
    currentButton = button;
    if(!item){
        //cout << "QQ" << endl;
        //if(currentLabel == nullptr){
        //    cout << "NL" << endl;
        //}
        if(!currentLabel || button == Qt::RightButton){
            //cout << "NO LABEL" << endl;
            return;
        }
        //cout << "CL" << currentLabel->getText().toStdString() << "__" << endl;

        if(currentItem && currentItem->type() == PolygonObj){
            //cout << "AA" << endl;
            if(insertVertex && vertex1 >= 0 && vertex2 >= 0){
                //cout << "BB" << endl;
                if(abs(vertex2 - vertex1) == 1){
                    //cout << "CC" << endl;
                    currentItem->insertVertex(min(vertex1, vertex2), pos);
                }
                else{
                    //cout << "DD" << endl;
                    currentItem->insertVertex(max(vertex1, vertex2), pos);
                }
                //cout << "EE" << endl;
                vertex1 = -1;
                vertex2 = -1;
                insertVertex = false;
                polygonSelectForm->polygonMessageBox->setPlainText(currentItem->baseInstructions());
            }
            else{
                //cout << "FF" << endl;
                currentItem->addPoint(pos);
            }
            scene->update();
            maskScene->update();
            return;
        }
        scene->inactiveAll();
        maskScene->inactiveAll();
        switch(currentSelectType){
            case RectangleObj:
            {
                RectangleSelect *temp = new RectangleSelect(pos, currentLabel);
                RectangleSelect *mirror = new RectangleSelect(pos);
                tempItem = temp;
                mirrorItem = mirror;
                mirrorItem->setLabel(currentLabel);
                mirrorItem->updatePen(mirrorItem->myPen);
                tempItem->setMirror(mirrorItem);
                mirrorItem->setMirror(tempItem);
            }
                break;
            case EllipseObj:
            {
                EllipseSelect *temp = new EllipseSelect(pos, currentLabel);
                EllipseSelect *mirror = new EllipseSelect(pos);
                tempItem = temp;
                mirrorItem = mirror;
                mirrorItem->setLabel(currentLabel);
                mirrorItem->updatePen(mirrorItem->myPen);
                tempItem->setMirror(mirrorItem);
                mirrorItem->setMirror(tempItem);
            }
                break;
            case FreeeraseObj:
            case FreedrawObj:
            {
                if(drawing){
                    FreeDrawSelect *temp = new FreeDrawSelect(pos, currentBrushSize, brushMode, currentLabel);
                    FreeDrawSelect *mirror = new FreeDrawSelect(pos, currentBrushSize, brushMode);
                    tempItem = temp;
                    mirrorItem = mirror;
                    mirrorItem->setLabel(currentLabel);
                    mirrorItem->updatePen(mirrorItem->myPen);
                    tempItem->setMirror(mirrorItem);
                    mirrorItem->setMirror(tempItem);
                }
                else{
                    myErase = new FreeDrawErase(pos, currentBrushSize, brushMode, currentLabel);
                    currentItem = myErase;
                }
            }
                break;
            case PolygonObj:
            {
                PolygonSelect *temp = new PolygonSelect(pos, currentLabel);
                PolygonSelect *mirror = new PolygonSelect(pos);
                tempItem = temp;
                mirrorItem = mirror;
                mirrorItem->setLabel(currentLabel);
                mirrorItem->updatePen(mirrorItem->myPen);
                tempItem->setMirror(mirrorItem);
                mirrorItem->setMirror(tempItem);
            }
                break;
        }
        if((currentSelectType != FreedrawObj && currentSelectType != FreeeraseObj) || drawing){
            mirrorItem->setLabel(currentLabel);
            mirrorItem->updatePen(mirrorItem->myPen);
            scene->addItem(tempItem);
            maskScene->addItem(mirrorItem);
            switch(type){
                case PHOTO_VIEWER_TYPE:
                    currentItem = tempItem;
                    break;
                case MASK_VIEWER_TYPE:
                    currentItem = mirrorItem;
                    break;
            }
        }
        scene->update();
        maskScene->update();
    }
    else{
        if(item->type() != currentSelectType){

            switch(item->type()){
                case FreedrawObj:
                    on_freeSelectButton_clicked();
                    break;
                case PolygonObj:
                    on_polygonSelectButton_clicked();
                    break;
                case RectangleObj:
                    on_rectangleSelectButton_clicked();
                    break;
                case EllipseObj:
                    on_ellipseSelectButton_clicked();
                    break;
            }
        }

        currentItem = item;
        scene->inactiveAll();
        maskScene->inactiveAll();
        currentItem->clickPoint(pos);

        if(currentItem->type() == PolygonObj && insertVertex){
            int vert = currentItem->getActiveVertex();
            //cout << vert << endl;
            if(vert != UNSELECTED){
                if(vertex1 <0){
                    vertex1 = vert;
                    polygonSelectForm->polygonMessageBox->appendPlainText("First vertex selected.");
                }
                else if(vertex2 < 0){
                    if(abs(vert - vertex1) != 1 && abs(vert - vertex1) != (currentItem->numberOfVertices() - 1)){
                        polygonSelectForm->polygonMessageBox->appendPlainText("Invalid second vertex, it must be adjacent to the first vertex.");
                    }
                    else{
                        vertex2 = vert;
                        polygonSelectForm->polygonMessageBox->setPlainText("Click on the point where you want to insert a new vertex. (must be outside the current polygon, but can be dragged anywhere)");
                    }
                }
            }
        }
        scene->update();
        maskScene->update();
    }
}

void InstaDam::processMouseMoved(QPointF fromPos, QPointF toPos){
    if(currentItem){
        if(currentButton == Qt::LeftButton){
            //cout << "MOVED" << endl;
            currentItem->moveItem(fromPos, toPos);
        }
        else{
            //cout << "ROTATE" << endl;
            currentItem->rotate(fromPos, toPos);
        }
        scene->update();
        maskScene->update();
    }
}

void InstaDam::processMouseReleased(viewerTypes type, QPointF oldPos, QPointF newPos, const Qt::MouseButton button)
{
    if(currentItem && !currentItem->isItemAdded()){
        if(currentItem->type() == FreeeraseObj){
            QUndoCommand *eraseCommand = new ErasePointsCommand(myErase, scene, maskScene);
            undoStack->push(eraseCommand);
        }else{
            QUndoCommand *addCommand = new AddCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), scene);
            undoStack->push(addCommand);
        }
        currentItem->resetState();
        if(currentItem->type() != PolygonObj){
            currentItem = nullptr;
        }
        else{
            currentItem->setActiveVertex(UNSELECTED);
        }
    }
    else if(currentItem && (currentItem->wasMoved() || currentItem->wasResized() || currentItem->wasRotated())){
        if(currentItem->wasMoved()){
            //cout << "MOVED" << endl;
            QUndoCommand *moveCommand = new MoveCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), oldPos, newPos);
            undoStack->push(moveCommand);
        }
        else if(currentItem->wasResized()){
            QUndoCommand *resizeCommand = new MoveVertexCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), oldPos, newPos, currentItem->getActiveVertex());
            undoStack->push(resizeCommand);
        }
        else{
            QUndoCommand *rotateCommand = new RotateCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), oldPos, newPos);
            undoStack->push(rotateCommand);
        }
        currentItem->resetState();
    }
    else if(currentItem && currentItem->type() == PolygonObj && !currentItem->wasPointAdded()){
        QUndoCommand *addVertexCommand = new AddVertexCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), newPos);
        undoStack->push(addVertexCommand);
        currentItem->setActiveVertex(UNSELECTED);
        currentItem->resetState();
    }
    currentButton = Qt::NoButton;
}

void InstaDam::finishPolygonButtonClicked(){
    if(currentItem)
        currentItem->setActiveVertex(UNSELECTED);
    currentItem = nullptr;
    scene->update();
    maskScene->update();
}
void InstaDam::processKeyPressed(viewerTypes type, const int key){
    if(!currentItem){

    }
    else if(key == Qt::Key_Delete || key == Qt::Key_Backspace){
        if(currentItem->type() == PolygonObj && currentItem->getActiveVertex() != UNSELECTED){
            QUndoCommand *deleteVertexCommand = new DeleteVertexCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror());
            undoStack->push(deleteVertexCommand);
        }
        else{
            QUndoCommand *deleteCommand = new DeleteCommand((type == PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), scene);
            undoStack->push(deleteCommand);
        }
    }
    else if(key == Qt::Key_X || key == Qt::Key_X + Qt::Key_Shift){
        currentItem = nullptr;
    }

}
