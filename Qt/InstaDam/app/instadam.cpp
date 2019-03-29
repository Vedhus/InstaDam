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
    mainUndoStack = new QUndoStack(this);
    tempUndoStack = new QUndoStack(this);
    undoGroup = new QUndoGroup(this);
    undoGroup->addStack(mainUndoStack);
    undoGroup->addStack(tempUndoStack);
    undoGroup->setActiveStack(mainUndoStack);
    currentSelectType = SelectItem::Ellipse;
    scene = ui->IdmPhotoViewer->scene;
    maskScene = ui->IdmMaskViewer->scene;
    currentItem = nullptr;
    currentLabel = nullptr;
    connect(scene, SIGNAL(pointClicked(PhotoScene::viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)), this,
            SLOT(processPointClicked(PhotoScene::viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)));
    connect(scene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(scene, SIGNAL(mouseReleased(PhotoScene::viewerTypes, QPointF, QPointF, const Qt::MouseButton)), this,
            SLOT(processMouseReleased(PhotoScene::viewerTypes, QPointF, QPointF, const Qt::MouseButton)));
    connect(scene, SIGNAL(keyPressed(PhotoScene::viewerTypes, const int)), this,
            SLOT(processKeyPressed(PhotoScene::viewerTypes, const int)));
    connect(maskScene, SIGNAL(pointClicked(PhotoScene::viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)), this,
            SLOT(processPointClicked(PhotoScene::viewerTypes, SelectItem*, QPointF, const Qt::MouseButton)));
    connect(maskScene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(maskScene, SIGNAL(mouseReleased(PhotoScene::viewerTypes, QPointF, QPointF, const Qt::MouseButton)), this,
            SLOT(processMouseReleased(PhotoScene::viewerTypes, QPointF, QPointF, const Qt::MouseButton)));
    connect(maskScene, SIGNAL(keyPressed(PhotoScene::viewerTypes, const int)), this,
            SLOT(processKeyPressed(PhotoScene::viewerTypes, const int)));

    connect(ui->addSelectionButton, SIGNAL(clicked()), this, SLOT(addCurrentSelection()));
    connect(ui->cancelSelectionButton, SIGNAL(clicked()), this, SLOT(cancelCurrentSelection()));
    undoAction = undoGroup->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    redoAction = undoGroup->createRedoAction(this, tr("&Redo"));
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
    connect(ui->panButton, SIGNAL(clicked()), this, SLOT(panButton_clicked()));
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
    labelButtons.clear();
    for(int i=0; i<currentProject.numLabels(); i++){
        QSharedPointer<Label> label = currentProject.getLabel(i);
        LabelButton *button = new LabelButton(label);
        button->setText(label->getText());
        QPalette pal = button->palette();
        pal.setColor(QPalette::ButtonText, Qt::black);
        pal.setColor(QPalette::Button, label->getColor());
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), this, SLOT(setCurrentLabel(QSharedPointer<Label>)));
        labelButtons.push_back(button);
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

void InstaDam::setCurrentLabel(QSharedPointer<Label> label){
    if(currentLabel == label)
        return;
    for(int i = 0; i < labelButtons.size(); i++){
        if(label != labelButtons[i]->myLabel){
            labelButtons[i]->setChecked(false);
        }
        else{
            labelButtons[i]->setChecked(true);
        }
    }
    currentLabel = label;
    cancelCurrentSelection();
}

void InstaDam::clearLayout(QLayout * layout) {
    if (! layout)
       return;
    while (auto item = layout->takeAt(0)) {
        if(item){
            delete item->widget();
            clearLayout(item->layout());
        }
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

        loadLabelFile(myfileName, PROJECT);
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

void InstaDam::on_actionSave_Annotation_triggered()
{
    // Saving the file
    #ifdef WASM_BUILD
        QByteArray outFile;
    #else
        QString outFileName = this->annotationPath;

        QFile outFile(outFileName);
        outFile.open(QIODevice::WriteOnly);
    #endif
        QJsonObject json;
        write(json, ANNOTATION);
        QJsonDocument saveDoc(json);
    #ifdef WASM_BUILD
        QString strJson(saveDoc.toJson(QJsonDocument::Compact));
        outFile.append(strJson);
        QHtml5File::save(outFile, "myproject.idantn");

    #else
        outFile.write(saveDoc.toJson());
    #endif
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
        write(json, PROJECT);
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
    QTextStream(stdout)<<currentProject.numLabels()<<"\n";
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
            QTextStream(stdout)<<currentProject.numLabels()<<"\n";
            foreach(QString tempFilename, imagesList) {
               QFileInfo tempInfo = QFileInfo(tempFilename);

               if (file.completeBaseName()==tempInfo.completeBaseName())
               {

                   break;
               }
              counter++;

            }
            fileId = counter;
            QTextStream(stdout)<<currentProject.numLabels()<<"\n";
            openFile_and_labels();
            QTextStream(stdout)<<currentProject.numLabels()<<"\n";
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
        qInfo("Going to save idantn");
        on_actionSave_Annotation_triggered() ; //exportImages();
        qInfo("saved idantn");
        int newId = (fileId+1)%imagesList.size();
        QTextStream(stdout)<<"NewId = "<<newId<<"\n";

        fileId = newId;
        this->filename = path.absolutePath()+"/"+imagesList[fileId];
        this->file = QFileInfo(this->filename);
        openFile_and_labels();

        qInfo("File opened");

    }

}


void InstaDam::on_saveAndBack_clicked()
{
    if (imagesList.empty())
            assertError("No file loaded! Please go to File->Open File and select an image to open");
    else
    {
        qInfo("Going to save idantn");
        on_actionSave_Annotation_triggered() ; //exportImages();
        qInfo("saved idantn");
        int newId = ((fileId-1)%imagesList.size()+imagesList.size())%imagesList.size();

        fileId = newId;
        this->filename = path.absolutePath()+"/"+imagesList[fileId];
        this->file = QFileInfo(this->filename);
        openFile_and_labels();

        qInfo("File opened");

    }

}

void InstaDam::exportImages(bool asBuffers)
{
    QString baseName = this->filename;
    for(int i = 0; i < currentProject.numLabels(); i++){
        QSharedPointer<Label> label = currentProject.getLabel(i);
        QString filename = baseName + "_" + label->getText() + ".png";
        if(asBuffers){
            //cout << "F " << filename.toStdString() << endl;
            QByteArray *bytes = new QByteArray();// = QSharedPointer<QByteArray>::create();
            QBuffer *buffer = new QBuffer(bytes);//QSharedPointer<QBuffer>::create(bytes.data());
            label->exportLabel(SelectItem::myBounds).save(buffer, "PNG");
            exportFiles.insert(filename, buffer);
            //cout << buffer->size() << endl;
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
    QString aPath = this->path.absolutePath()+"/annotations/"+baseName+"/";
    QString exPath = this->path.absolutePath()+"/exports/"+baseName+"/";
    if (!QDir(aPath).exists())
    {
        //qInfo("Creating paths %s", labelPath.toUtf8().constData());
        QDir().mkpath(aPath);
    }
    if (!QDir(exPath).exists())
    {
        //qInfo("Creating paths %s", labelPath.toUtf8().constData());
        QDir().mkpath(exPath);
    }
    labelPaths.clear();
    this->annotationPath = aPath+baseName+".idantn";
    for(int i=0; i<currentProject.numLabels(); i++){
        QString labfilePrefix = QString("%1").arg(i, 5, 10, QChar('0'));
        this->labelPaths.append(exPath+labfilePrefix+"_label.png");
    }

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
    QTextStream(stdout)<<currentProject.numLabels()<<"\n";
    scene->clearItems();
    maskScene->clearItems();
    generateLabelFileName();
    QTextStream(stdout)<<currentProject.numLabels()<<"\n";
    if (QFileInfo(this->annotationPath).isFile())
    {
        QTextStream(stdout) <<"Loading labels\n"<<this->annotationPath;
        QTextStream(stdout) <<"\n"<<this->file.baseName();
        loadLabelFile(this->annotationPath, ANNOTATION);
        QTextStream(stdout) <<"Loaded labels";
    }
    else {
        for(int i=0; i<currentProject.numLabels(); i++)
        {
           currentProject.getLabel(i)->clear();
        }
    }
    QTextStream(stdout) <<"Loading photo";
    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromFile(filename, labelPaths[0]);
    qInfo("my bounds set");
#endif
    ui->IdmMaskViewer->LinkToPhotoViewer(ui->IdmPhotoViewer);
    qInfo("photo viewer linked!");
    scene->update();
    maskScene->update();
}

void InstaDam::loadLabelFile(QString filename, fileTypes fileType){

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
    scene->clearItems();
    maskScene->clearItems();
    QTextStream(stdout) <<"Loaded file";

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    if (fileType == PROJECT)
        {
            this->currentProject = Project();
            read(loadDoc.object(), fileType);
            clearLayout(ui->labelClassLayout);
            labelButtons.clear();
            for(int i=0; i<currentProject.numLabels(); i++)
            {

                QSharedPointer<Label> label = currentProject.getLabel(i);
                LabelButton *button = new LabelButton(label);

                button->setText(label->getText());
                QPalette pal = button->palette();
                QTextStream(stdout) << label->getColor().name() << endl;
                pal.setColor(QPalette::ButtonText, Qt::black);
                pal.setColor(QPalette::Button, label->getColor());
                button->setAutoFillBackground(true);
                button->setPalette(pal);
                button->update();
                connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), this, SLOT(setCurrentLabel(QSharedPointer<Label>)));
                //        connect(button, SIGNAL(clicked()), this, SLOT(setCurrentLabel(button.myLabel)));
                labelButtons.push_back(button);
                ui->labelClassLayout->addWidget(button);
            }
        }

    else {

        for(int i=0; i<currentProject.numLabels(); i++)
            {
            read(loadDoc.object(), fileType);
            QSharedPointer<Label> label = currentProject.getLabel(i);

            if(!label->rectangleObjects.isEmpty()){
                QHashIterator<int, RectangleSelect*> rit(label->rectangleObjects);
                while(rit.hasNext()){
                    rit.next();
                    RectangleSelect *mirror = new RectangleSelect();
                    rit.value()->setLabel(label);
                    mirror->setLabel(label);
                    mirror->updatePen(mirror->myPen);
                    rit.value()->setMirror(mirror);
                    mirror->setMirror(rit.value());
                    mirror->setRectUnchecked(rit.value()->getRect());
                    rit.value()->rotateMirror();
                    scene->addItem(rit.value());
                    maskScene->addItem(mirror);
                    rit.value()->itemWasAdded();
                    mirror->itemWasAdded();
                }
            }
            if(!label->ellipseObjects.isEmpty()){
                QHashIterator<int, EllipseSelect*> eit(label->ellipseObjects);
                while(eit.hasNext()){
                    eit.next();
                    EllipseSelect *mirror = new EllipseSelect();
                    eit.value()->setLabel(label);
                    mirror->setLabel(label);
                    mirror->updatePen(mirror->myPen);
                    eit.value()->setMirror(mirror);
                    mirror->setMirror(eit.value());
                    mirror->setRectUnchecked(eit.value()->getRect());
                    eit.value()->rotateMirror();
                    scene->addItem(eit.value());
                    maskScene->addItem(mirror);
                    eit.value()->itemWasAdded();
                    mirror->itemWasAdded();
                }
            }
            if(!label->polygonObjects.isEmpty()){
                QHashIterator<int, PolygonSelect*> pit(label->polygonObjects);
                while(pit.hasNext()){
                    pit.next();
                    PolygonSelect *mirror = new PolygonSelect();
                    pit.value()->setLabel(label);
                    mirror->setLabel(label);
                    mirror->updatePen(mirror->myPen);
                    pit.value()->setMirror(mirror);
                    mirror->setMirror(pit.value());
                    pit.value()->setMirrorPolygon(SelectItem::UNSELECTED);
                    scene->addItem(pit.value());
                    maskScene->addItem(mirror);
                    pit.value()->itemWasAdded();
                    mirror->itemWasAdded();
                }
            }
            if(!label->freeDrawObjects.isEmpty()){
                FreeDrawSelect *item = label->freeDrawObjects.values()[0];
                FreeDrawSelect *mirror = new FreeDrawSelect();
                item->setLabel(label);
                mirror->setLabel(label);
                mirror->updatePen(mirror->myPen);
                item->setMirror(mirror);
                mirror->setMirror(item);
                item->setMirrorMap();
                scene->addItem(item);
                maskScene->addItem(mirror);
                item->itemWasAdded();
                mirror->itemWasAdded();
            }
        }
    }
    scene->inactiveAll();
    mainUndoStack->clear();
    tempUndoStack->clear();
    undoGroup->setActiveStack(mainUndoStack);
    scene->update();
    maskScene->update();
}


void InstaDam::panButton_clicked()
{
    panning = !panning; //!ui->panButton->isChecked();
    ui->panButton->setChecked(panning);
    ui->IdmPhotoViewer->setPanMode(panning);
    ui->IdmMaskViewer->setPanMode(panning);
}

void InstaDam::on_rectangleSelectButton_clicked(){
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
    switch(currentSelectType){
        case SelectItem::Polygon:
            if(ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")){
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            ui->polygonSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            if(ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")){
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            ui->freeSelectButton->setChecked(false);
            controlLayout->addWidget(blankWidget);
            break;
        case SelectItem::Rectangle:
            break;
        case SelectItem::Ellipse:
            ui->ellipseSelectButton->setChecked(false);
            break;
    }
    ui->rectangleSelectButton->setChecked(true);
    blankForm->blankText->setPlainText(rectBaseString);
    blankWidget->show();
    currentSelectType = SelectItem::Rectangle;
    scene->update();
    maskScene->update();
}

void InstaDam::on_ellipseSelectButton_clicked(){
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
    switch(currentSelectType){
        case SelectItem::Polygon:
            if(ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")){
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            ui->polygonSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            if(ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")){
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            ui->freeSelectButton->setChecked(false);
            break;
        case SelectItem::Rectangle:
            ui->rectangleSelectButton->setChecked(false);
            break;
        case SelectItem::Ellipse:
            break;
    }
    ui->ellipseSelectButton->setChecked(true);
    blankForm->blankText->setPlainText(ellipseBaseString);
    blankWidget->show();
    currentSelectType = SelectItem::Ellipse;
    scene->update();
    maskScene->update();
}

void InstaDam::on_polygonSelectButton_clicked(){
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
    switch(currentSelectType){
        case SelectItem::Ellipse:
        case SelectItem::Rectangle:
            if(ui->selectControlFrame->findChild<QWidget*>("blankForm")){
                controlLayout->removeWidget(blankWidget);
                blankWidget->hide();
            }
            controlLayout->addWidget(polygonSelectWidget);
            ui->ellipseSelectButton->setChecked(false);
            ui->rectangleSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            if(ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")){
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            controlLayout->addWidget(polygonSelectWidget);
            ui->freeSelectButton->setChecked(false);
            break;
        case SelectItem::Polygon:
            break;
    }
    ui->polygonSelectButton->setChecked(true);
    polygonSelectWidget->show();
    polygonSelectForm->polygonMessageBox->setPlainText(polygonBaseInstruction);
    currentSelectType = SelectItem::Polygon;
    scene->update();
}

void InstaDam::on_freeSelectButton_clicked(){
    scene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
    switch(currentSelectType){
        case SelectItem::Ellipse:
        case SelectItem::Rectangle:
            if(ui->selectControlFrame->findChild<QWidget*>("blankForm")){
                controlLayout->removeWidget(blankWidget);
                blankWidget->hide();
            }
            controlLayout->addWidget(freeSelectWidget);
            ui->ellipseSelectButton->setChecked(false);
            ui->rectangleSelectButton->setChecked(false);
            break;
        case SelectItem::Polygon:
            if(ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")){
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(freeSelectWidget);
            ui->polygonSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            break;
    }
    ui->freeSelectButton->setChecked(true);
    freeSelectWidget->show();
    currentSelectType = SelectItem::Freedraw;
    scene->update();
    maskScene->update();
}

void InstaDam::roundBrushButtonClicked()
{
    brushMode = Qt::RoundCap;
    freeSelectForm->squareBrushButton->setChecked(false);
    freeSelectForm->roundBrushButton->setChecked(true);
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
    ui->IdmMaskViewer->setBrushMode(Qt::RoundCap);
}

void InstaDam::squareBrushButtonClicked()
{
    brushMode = Qt::SquareCap;
    freeSelectForm->squareBrushButton->setChecked(true);
    freeSelectForm->roundBrushButton->setChecked(false);
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

void InstaDam::processPointClicked(PhotoScene::viewerTypes type, SelectItem *item, QPointF pos, const Qt::MouseButton button){
    //cout << "CLICK " << type<< endl;
    currentButton = button;
    if(!item){
        //cout << "QQ" << endl;
        //if(currentLabel == nullptr){
        //    cout << "NL" << endl;
        //}
        //cout << canDrawOnPhoto << currentItem << endl;
        if(!canDrawOnPhoto && (!currentItem || (currentItem && currentItem->type() != SelectItem::Polygon))){
            scene->inactiveAll();
            maskScene->inactiveAll();
            scene->update();
            maskScene->update();
            return;
        }
        if(type == PhotoScene::MASK_VIEWER_TYPE && currentSelectType != SelectItem::Freedraw){
            canDrawOnPhoto = false;
            undoGroup->setActiveStack(tempUndoStack);
            ui->addSelectionButton->setEnabled(true);
            ui->cancelSelectionButton->setEnabled(true);
        }

        if(!currentLabel || button == Qt::RightButton){
            //cout << "NO LABEL" << endl;
            return;
        }
        //cout << "CL" << currentLabel->getText().toStdString() << "__" << endl;

        if(currentItem && currentItem->type() == SelectItem::Polygon){
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
        //cout << "IAA" << endl;
        scene->inactiveAll();
        maskScene->inactiveAll();
        switch(currentSelectType){
            case SelectItem::Rectangle:
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
            case SelectItem::Ellipse:
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
            case SelectItem::Freeerase:
            case SelectItem::Freedraw:
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
            case SelectItem::Polygon:
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
        if((currentSelectType != SelectItem::Freedraw && currentSelectType != SelectItem::Freeerase) || drawing){
            mirrorItem->setLabel(currentLabel);
            mirrorItem->updatePen(mirrorItem->myPen);
            scene->addItem(tempItem);
            maskScene->addItem(mirrorItem);
            switch(type){
                case PhotoScene::PHOTO_VIEWER_TYPE:
                    currentItem = tempItem;
                    break;
                case PhotoScene::MASK_VIEWER_TYPE:
                    currentItem = mirrorItem;
                    break;
            }
            if(!canDrawOnPhoto)
                maskItem = currentItem;
        }
        scene->update();
        maskScene->update();
    }
    else{
        if(item->type() != currentSelectType){
            if(!canDrawOnPhoto)
                return;
            switch(item->type()){
                case SelectItem::Freedraw:
                    on_freeSelectButton_clicked();
                    break;
                case SelectItem::Polygon:
                    on_polygonSelectButton_clicked();
                    break;
                case SelectItem::Rectangle:
                    on_rectangleSelectButton_clicked();
                    break;
                case SelectItem::Ellipse:
                    on_ellipseSelectButton_clicked();
                    break;
            }
        }
        currentItem = item;
        if(!canDrawOnPhoto)
            maskItem = currentItem;
        currentLabel = currentItem->getLabel();
        //cout << "LABL" << endl;
        scene->inactiveAll();
        maskScene->inactiveAll();
        currentItem->clickPoint(pos);
        currentItem->setItemActive();
        if(currentItem->type() == SelectItem::Polygon && insertVertex){
            int vert = currentItem->getActiveVertex();
            //cout << vert << endl;
            if(vert != SelectItem::UNSELECTED){
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

void InstaDam::processMouseReleased(PhotoScene::viewerTypes type, QPointF oldPos, QPointF newPos, const Qt::MouseButton button)
{
    UNUSED(button);
    //cout << currentItem->type() << "  " << currentItem->isItemAdded() << endl;
    if(currentItem && !currentItem->isItemAdded()){
        if(currentItem->type() == SelectItem::Freedraw && type == PhotoScene::MASK_VIEWER_TYPE){
            FreeDrawSelect fds(maskSelection(currentItem));
            FreeDrawSelect *temp = dynamic_cast<FreeDrawSelect*>(currentItem);
            temp->setPointsUnchecked(fds.getMap());
        }
        if(currentItem->type() == SelectItem::Freeerase){
            QUndoCommand *eraseCommand = new ErasePointsCommand(myErase, scene, maskScene);
            undoGroup->activeStack()->push(eraseCommand);
        }
        else{
            QUndoCommand *addCommand = new AddCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), scene);
            undoGroup->activeStack()->push(addCommand);
        }
        currentItem->resetState();
        if(currentItem->type() != SelectItem::Polygon){
            //if(currentItem->type() == SelectItem::Freedraw){
            //    FreeDrawSelect *fd = dynamic_cast<FreeDrawSelect*>(currentItem);
            //    fd->print();
            //}
            currentItem = nullptr;
        }
        else{
            currentItem->setActiveVertex(SelectItem::UNSELECTED);
        }
    }
    else if(currentItem && (currentItem->wasMoved() || currentItem->wasResized() || currentItem->wasRotated())){
        if(currentItem->wasMoved()){
            //cout << "MOVED" << endl;
            QUndoCommand *moveCommand = new MoveCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), oldPos, newPos);
            undoGroup->activeStack()->push(moveCommand);
        }
        else if(currentItem->wasResized()){
            QUndoCommand *resizeCommand = new MoveVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), oldPos, newPos, currentItem->getActiveVertex());
            undoGroup->activeStack()->push(resizeCommand);
        }
        else{
            QUndoCommand *rotateCommand = new RotateCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), oldPos, newPos);
            undoGroup->activeStack()->push(rotateCommand);
        }
        currentItem->resetState();
    }
    else if(currentItem && currentItem->type() == SelectItem::Polygon && !currentItem->wasPointAdded()){
        QUndoCommand *addVertexCommand = new AddVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), newPos);
        undoGroup->activeStack()->push(addVertexCommand);
        currentItem->setActiveVertex(SelectItem::UNSELECTED);
        currentItem->resetState();
    }
    currentButton = Qt::NoButton;
    scene->update();
    maskScene->update();
}

void InstaDam::finishPolygonButtonClicked(){
    if(currentItem)
        currentItem->setActiveVertex(SelectItem::UNSELECTED);
    currentItem = nullptr;
    scene->update();
    maskScene->update();
}
void InstaDam::processKeyPressed(PhotoScene::viewerTypes type, const int key){
    if(!currentItem){
        return;
    //    cout << "NO CURRENT" << endl;
    }
    else if(key == Qt::Key_Delete || key == Qt::Key_Backspace){
        if(currentItem->type() == SelectItem::Polygon && currentItem->getActiveVertex() != SelectItem::UNSELECTED){
            QUndoCommand *deleteVertexCommand = new DeleteVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror());
            undoGroup->activeStack()->push(deleteVertexCommand);
        }
        else{
            QUndoCommand *deleteCommand = new DeleteCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(), scene);
            undoGroup->activeStack()->push(deleteCommand);
        }
    }
    else if(key == Qt::Key_X || key == Qt::Key_X + Qt::Key_Shift){
        currentItem = nullptr;
    }

}

void InstaDam::read(const QJsonObject &json, fileTypes type = PROJECT){
    if(json.contains("image")){

    }
    if(json.contains("labels") && json["labels"].isArray()){
        QJsonArray labelArray = json["labels"].toArray();
        //labels.reserve(labelArray.size());

        for(int i = 0; i < labelArray.size(); i++){
            QJsonObject labelObject = labelArray[i].toObject();
            if (type == PROJECT)
            {

                QSharedPointer<Label> label = QSharedPointer<Label>::create(labelObject, i);
                currentProject.addLabel(label);
                QTextStream(stdout)<<currentProject.numLabels()<<"\n";
            }
            else {
                    QTextStream(stdout)<<currentProject.numLabels();
                    QTextStream(stdout)<<currentProject.getLabel(i)->getText();
                    currentProject.getLabel(i)->clear();
                    currentProject.getLabel(i)->readIdantn(labelObject);


            }


        }
    }
}

void InstaDam::write(QJsonObject &json, fileTypes type = PROJECT){

    QJsonArray labs;
    for(int i = 0; i < currentProject.numLabels(); i++){
        QJsonObject lab;
        if (type== PROJECT)
        {
            currentProject.getLabel(i)->write(lab);
        }
        else if (type == ANNOTATION)
        {
            currentProject.getLabel(i)->writeIdantn(lab);
        }
        labs.append(lab);
    }
    json["labels"] = labs;



}

void InstaDam::addCurrentSelection(){
    tempUndoStack->clear();
    //cout << 12 << endl;
    undoGroup->setActiveStack(mainUndoStack);
    //cout << 13 << endl;
    FreeDrawSelect *fds = new FreeDrawSelect(maskSelection(maskItem), currentLabel);
    //cout << 14 << endl;
    scene->addItem(fds);
    //cout << 15 << endl;
    QUndoCommand *addCommand = new AddCommand(fds, scene);
    //cout << 16 << endl;
    mainUndoStack->push(addCommand);
    //cout << 17 << endl;
    scene->removeItem(maskItem->getMirror());
    maskScene->removeItem(maskItem->getMirror());
    scene->removeItem(maskItem);
    //cout << 18 << endl;
    maskScene->removeItem(maskItem);
    //cout << 19 << endl;
    //delete maskItem->getMirror();
    //cout << 20 << endl;
    //delete maskItem;
    //cout << 21 << endl;
    maskItem = nullptr;
    //cout << 22 << endl;
    ui->addSelectionButton->setDisabled(true);
    //cout << 23 << endl;
    ui->cancelSelectionButton->setDisabled(true);
    //cout << 24 << endl;
    canDrawOnPhoto = true;
}

void InstaDam::cancelCurrentSelection(){
    //cout << 1 << endl;
    undoGroup->setActiveStack(mainUndoStack);
    //cout << 2 << endl;
    canDrawOnPhoto = true;
    if(!ui->addSelectionButton->isEnabled()){
        //cout << 3 << endl;
        return;
    }
    //cout << 4 << endl;
    tempUndoStack->clear();
    //cout << "QQ" << endl;
    //cout << maskItem << endl;
    //cout << 5 << endl;
    scene->removeItem(maskItem->getMirror());
    maskScene->removeItem(maskItem->getMirror());
    scene->removeItem(maskItem);
    //cout << 6 << endl;
    maskScene->removeItem(maskItem);
    //cout << 7 << endl;
    //delete maskItem->getMirror();
    //cout << 8 << endl;
    //delete maskItem;
    //cout << 9 << endl;
    maskItem = nullptr;
    //cout << 10 << endl;
    ui->addSelectionButton->setDisabled(true);
    //cout << 11 << endl;
    ui->cancelSelectionButton->setDisabled(true);
}

QPixmap InstaDam::maskSelection(SelectItem *item){
    QPixmap map(SelectItem::myBounds);
    //cout << 2 << endl;
    map.fill(Qt::transparent);
    //cout << 3 << endl;
    QPainter *paint = new QPainter(&map);
    //cout << 4 << endl;
    QBrush brush(currentLabel->getColor());
    //cout << 5 << endl;
    paint->setPen(currentLabel->getColor());
    //cout << 6 << endl;
    paint->setBrush(brush);
    //cout << 7 << endl;
    paint->setCompositionMode(QPainter::CompositionMode_SourceOver);
    //cout << 8 << endl;
    item->toPixmap(paint);
    //cout << 9 << endl;
    paint->end();
    //cout << 10 << endl;
    map = joinPixmaps(ui->IdmMaskViewer->photo->pixmap(), map, QPainter::CompositionMode_DestinationIn);
    return map;
}

void InstaDam::on_addSelectionButton_clicked()
{

}



