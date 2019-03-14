#include "instadam.h"
#include "ui_instadam.h"
#include "pixmapops.h"
#include "filtercontrols.h"
#include "labelButton.h"
#include <string>
#include <QByteArray>
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
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#ifdef WASM_BUILD
#include "htmlFileHandler/qhtml5file.h"
#endif

InstaDam::InstaDam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstaDam)
{
    ui->setupUi(this);
    filterControl = new filterControls();
    connectFilters();
    ui->IdmPhotoViewer->setFilterControls(filterControl);
    ui->IdmMaskViewer->setFilterControls(filterControl);
#ifdef WASM_BUILD
    ui->actionExport->setVisible(false);
#endif
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
    addImageConnector("Load File", [&]() {
            QHtml5File::load(".jpg, .png", [&](const QByteArray &content, const QString &fileName){
                imageFileContent = content;
                this->filename = fileName;
                imagesList = (QStringList() << filename);

                openFile_and_labels();
            });
        });

    addIdproConnector("Load File", [&]() {
            QHtml5File::load(".idpro", [&](const QByteArray &content, const QString &fileName){
                idproFileContent = content;
                idproFileName = fileName;
                //imagesList = (QStringList() << filename);

                loadLabelFile(idproFileName);
            });
        });

#endif
}

#ifdef WASM_BUILD
void InstaDam::addImageConnector(QString text, std::function<void ()> onActivate){
    openImageConnector = new MyConnector;
    openImageConnector->onActivate = onActivate;
}

void InstaDam::addIdproConnector(QString text, std::function<void ()> onActivate){
    openIdproConnector = new MyConnector;
    openIdproConnector->onActivate = onActivate;
}

#endif
InstaDam::~InstaDam()
{
    delete ui;
}

void InstaDam::setNewProject(){
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

}

void InstaDam::on_actionNew_triggered()
{
    currentProject.resetLabels();
    newProject = new newproject(this);
    newProject->setModal(true);
    connect(newProject, SIGNAL(accepted()), this, SLOT(setNewProject()));
    newProject->exec();
    setNewProject();
}

void InstaDam::setCurrentLabel(LabelButton *button){
    currentLabel = button->myLabel;
}

void InstaDam::setCurrentLabel(Label *label){
    currentLabel = label;
}

void InstaDam::clearLayout(QLayout * layout) {
    if (! layout)
       return;
    while (auto item = layout->takeAt(0)) {
       delete item->widget();
       clearLayout(item->layout());
    }
 }

void InstaDam::on_actionOpen_triggered()
{
#ifdef WASM_BUILD
      openIdproConnector->onActivate();
#else
      //QString doc;
    // Reading and Loading
    QString myfileName = QFileDialog::getOpenFileName(this,
        tr("Open Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
    if (myfileName.isEmpty()){
            return; // remove that part and add an alert
    }
    else
    {
        loadLabelFile(myfileName);
    }
#endif
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
    outFile.open(QIODevice::WriteOnly);
#endif
    QJsonObject json;
    write(json);
    QJsonDocument saveDoc(json);
#ifdef WASM_BUILD
    QString strJson(saveDoc.toJson(QJsonDocument::Compact));
    outFile.append(strJson);
    QHtml5File::save(outFile, "myproject.idpro");

#else
    outFile.write(saveDoc.toJson());
#endif
}


void InstaDam::on_actionOpen_File_triggered()
{
#ifdef WASM_BUILD
    openImageConnector->onActivate();
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
        exportImages();
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

void InstaDam::exportImages(bool asBuffers)
{
    QString baseName = this->filename;
    for(int i = 0; i < currentProject.numLabels(); i++){
        Label *label = currentProject.getLabel(i);
        QString filename = baseName + "_" + label->getText() + ".png";
        if(asBuffers){
            cout << "F " << filename.toStdString() << endl;
            QByteArray *bytes;// = QSharedPointer<QByteArray>::create();
            QBuffer *buffer = new QBuffer(bytes);//QSharedPointer<QBuffer>::create(bytes.data());
            label->exportLabel(SelectItem::myBounds).save(buffer, "PNG");
            exportFiles.insert(filename, buffer);
            cout << buffer->size() << endl;
        }
        else{
            label->exportLabel(SelectItem::myBounds).save(filename, "PNG");
        }
    }
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
    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromByteArray(imageFileContent,labelNameTemp);
#else
    //Open labels
    generateLabelFileName();
    if (QFileInfo(labelFile).isFile())
    {
        loadLabelFile(labelFile);
    }
    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromFile(filename, labelNameTemp);
#endif
    ui->IdmMaskViewer->LinkToPhotoViewer(ui->IdmPhotoViewer);
    scene->update();
    maskScene->update();
}


void InstaDam::loadLabelFile(QString filename){
    this->currentProject = Project();

#ifdef WASM_BUILD
    QByteArray saveData = idproFileContent;
#else
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QByteArray saveData = loadFile.readAll();
#endif
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());

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
        if(!label->rectangleObjects.isEmpty()){
            QHashIterator<int, RectangleSelect*> rit(label->rectangleObjects);
            while(rit.hasNext()){
                rit.next();
                RectangleSelect *mirror = new RectangleSelect();
                mirror->setLabel(label);
                mirror->updatePen(mirror->myPen);
                rit.value()->setMirror(mirror);
                mirror->setMirror(rit.value());
                mirror->setRectUnchecked(rit.value()->getRect());
                rit.value()->rotateMirror();
                scene->addItem(rit.value());
                maskScene->addItem(mirror);
            }
        }
        if(!label->ellipseObjects.isEmpty()){
            QHashIterator<int, EllipseSelect*> eit(label->ellipseObjects);
            while(eit.hasNext()){
                eit.next();
                EllipseSelect *mirror = new EllipseSelect();
                mirror->setLabel(label);
                mirror->updatePen(mirror->myPen);
                eit.value()->setMirror(mirror);
                mirror->setMirror(eit.value());
                mirror->setRectUnchecked(eit.value()->getRect());
                eit.value()->rotateMirror();
                scene->addItem(eit.value());
                maskScene->addItem(mirror);
            }
        }
        if(!label->polygonObjects.isEmpty()){
            QHashIterator<int, PolygonSelect*> pit(label->polygonObjects);
            while(pit.hasNext()){
                pit.next();
                PolygonSelect *mirror = new PolygonSelect();
                mirror->setLabel(label);
                mirror->updatePen(mirror->myPen);
                pit.value()->setMirror(mirror);
                mirror->setMirror(pit.value());
                pit.value()->setMirrorPolygon(UNSELECTED);
                scene->addItem(pit.value());
                maskScene->addItem(mirror);
            }
        }
        if(!label->freeDrawObjects.isEmpty()){
            FreeDrawSelect *item = label->freeDrawObjects.values()[0];
            FreeDrawSelect *mirror = new FreeDrawSelect();
            mirror->setLabel(label);
            mirror->updatePen(mirror->myPen);
            item->setMirror(mirror);
            mirror->setMirror(item);
            item->setMirrorMap();
            scene->addItem(item);
            maskScene->addItem(mirror);
        }
    }
    scene->update();
    maskScene->update();
}


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
void InstaDam::on_actionExport_triggered()
{
    exportImages();
}

void InstaDam::on_actionExport_zip_triggered(){
    exportImages(true);
#ifdef WASM_BUILD
    QByteArray *outbytes;

    QBuffer *outbuffer = new QBuffer(outbytes);
    QuaZip zip(outbuffer);
#else
    QuaZip zip(this->filename + "_idpro.zip");
#endif
    if (!zip.open(QuaZip::mdCreate)) {
        //myMessageOutput(true, QtDebugMsg, QString("testCreate(): zip.open(): %1").arg(zip.getZipError()));
        return;
    }

    QuaZipFile outFile(&zip);
    QHashIterator<QString, QBuffer*> it(exportFiles);
    while(it.hasNext()){
        it.next();
        QString filename = it.key();
        QBuffer *buffer = it.value();

        if(!(*buffer).open(QIODevice::ReadOnly)) {
            qInfo("Could not access buffer file.");
            return;
        }

        if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(filename, filename))) {
            qInfo("Error opening zip file.");
            return;
        }
        for (qint64 pos = 0, len = (*buffer).size(); pos < len; ) {
            char buf[4096];
            qint64 readSize = qMin(static_cast<qint64>(4096), len - pos);
            qint64 l;
            if ((l = (*buffer).read(buf, readSize)) != readSize){
                qInfo("Read failure");
}
            qDebug("Reading %ld bytes from %s at %ld returned %ld",
                   static_cast<long>(readSize),
                   filename.toUtf8().constData(),
                   static_cast<long>(pos),
                   static_cast<long>(l));
            outFile.write(buf, readSize);
            pos += readSize;
        }
        if (outFile.getZipError() != UNZ_OK) {
            qInfo("Zip file error");
            return;
        }
        outFile.close();

        if (outFile.getZipError() != UNZ_OK) {
            qInfo("Error closeing zip file.");
            return;
        }

        (*buffer).close();
    }

    zip.close();

    if (zip.getZipError() != 0) {
        qInfo("Error.");
        return;
    }
#ifdef WASM_BUILD
    QHtml5File::save(outbuffer->data(), "myproject_idpro.zip");
#endif
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
            //if(currentItem->type() == FreedrawObj){
            //    FreeDrawSelect *fd = dynamic_cast<FreeDrawSelect*>(currentItem);
            //    fd->print();
            //}
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

void InstaDam::read(const QJsonObject &json){
    if(json.contains("image")){

    }
    if(json.contains("labels") && json["labels"].isArray()){
        QJsonArray labelArray = json["labels"].toArray();
        //labels.reserve(labelArray.size());
        for(int i = 0; i < labelArray.size(); i++){
            QJsonObject labelObject = labelArray[i].toObject();
            cout << "LABEL" << endl;
            Label label = Label(labelObject);
            currentProject.addLabel(label);
        }
    }
}

void InstaDam::write(QJsonObject &json){
    QJsonArray labs;
    for(int i = 0; i < currentProject.numLabels(); i++){
        QJsonObject lab;
        currentProject.getLabel(i)->write(lab);
        labs.append(lab);
    }
    json["labels"] = labs;
}
