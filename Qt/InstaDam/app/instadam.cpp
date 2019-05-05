#include "instadam.h"

#include <QByteArray>
#include <QDialog>
#include <QImageReader>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "ui_instadam.h"
#include "ui_projectDialog.h"
#include "pixmapops.h"
#include "filtercontrols.h"
#include "labelButton.h"
#include "Selector/selectItem.h"
#include "Selector/ellipseSelect.h"
#include "Selector/rectangleSelect.h"
#include "Selector/polygonSelect.h"
#include "Selector/freeDrawSelect.h"
#include "Selector/freeDrawErase.h"
#include "commands.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "imagelist.h"
#include "ui_addusertoproject.h"
#include "addusertoproject.h"
#include "ui_projectlist.h"
#include "projectlist.h"
#include "serverprojectname.h"
#include "ui_serverprojectname.h"
#ifdef WASM_BUILD
#include "htmlFileHandler/qhtml5file.h"
#endif



/*!
  \class InstaDam
  \ingroup app
  \inmodule InstaDam
  \inherits QMainWindow
  \brief The InstaDam class defined the main window for the app.

  InstaDam is the main window class for the InstaDam app. The class includes
  all the signals, initialization and slots of the various buttons and tools that can be
  selected from the main window.
*/

/*!
  Constructs an InstaDam window given QString \a databaseURL, Qstring \a token
  and QMainWindow \a parent, if any. The constructor connects all the buttons
  in the main window.
*/
InstaDam::InstaDam(QWidget *parent, QString databaseURL, QString token) :
    QMainWindow(parent), ui(new Ui::InstaDam) {
    hide();
    ui->setupUi(this);
    photoLoaded = false;
    filterControl = new filterControls();
    maskTypeList = { EnumConstants::BLUR, EnumConstants::CANNY,
                     EnumConstants::THRESHOLD, EnumConstants::LABELMASK, EnumConstants::COLORTHRESHOLD};
    maskButtonList = {ui->blur_label, ui->canny_label, ui->threshold_label,
                      ui->labelmask_label, ui->colorthreshold_label};
    connectFilters();
    qInfo("Connected Filters");
    ui->IdmPhotoViewer->setFilterControls(filterControl);
    ui->IdmMaskViewer->setFilterControls(filterControl);
    newProject = new newproject(this);

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

    this->databaseURL = databaseURL;
    this->accessToken = token;
    currentItem = nullptr;
    currentLabel = nullptr;
    currentProject = new Project();
    connect(scene, SIGNAL(pointClicked(PhotoScene::viewerTypes, SelectItem*,
                                       QPointF, const Qt::MouseButton,
                                       const Qt::KeyboardModifiers)), this,
            SLOT(processPointClicked(PhotoScene::viewerTypes, SelectItem*,
                                     QPointF, const Qt::MouseButton,
                                     const Qt::KeyboardModifiers)));
    connect(scene, SIGNAL(mouseMoved(QPointF, QPointF,
                                     const Qt::KeyboardModifiers)), this,
            SLOT(processMouseMoved(QPointF, QPointF,
                                   const Qt::KeyboardModifiers)));
    connect(scene, SIGNAL(mouseReleased(PhotoScene::viewerTypes, QPointF,
                                        QPointF, const Qt::MouseButton,
                                        const Qt::KeyboardModifiers)), this,
            SLOT(processMouseReleased(PhotoScene::viewerTypes, QPointF, QPointF,
                                      const Qt::MouseButton,
                                      const Qt::KeyboardModifiers)));
    connect(scene, SIGNAL(keyPressed(PhotoScene::viewerTypes, const int)), this,
            SLOT(processKeyPressed(PhotoScene::viewerTypes, const int)));
    connect(maskScene, SIGNAL(pointClicked(PhotoScene::viewerTypes, SelectItem*,
                                           QPointF, const Qt::MouseButton,
                                           const Qt::KeyboardModifiers)), this,
            SLOT(processPointClicked(PhotoScene::viewerTypes, SelectItem*,
                                     QPointF, const Qt::MouseButton,
                                     const Qt::KeyboardModifiers)));
    connect(maskScene, SIGNAL(mouseMoved(QPointF, QPointF,
                                         const Qt::KeyboardModifiers)), this,
            SLOT(processMouseMoved(QPointF, QPointF,
                                   const Qt::KeyboardModifiers)));
    connect(maskScene, SIGNAL(mouseReleased(PhotoScene::viewerTypes, QPointF,
                                            QPointF, const Qt::MouseButton,
                                            const Qt::KeyboardModifiers)), this,
            SLOT(processMouseReleased(PhotoScene::viewerTypes, QPointF, QPointF,
                                      const Qt::MouseButton,
                                      const Qt::KeyboardModifiers)));
    connect(maskScene, SIGNAL(keyPressed(PhotoScene::viewerTypes, const int)), this,
            SLOT(processKeyPressed(PhotoScene::viewerTypes, const int)));

    connect(ui->showMaskSelections, SIGNAL(stateChanged(int)), this,
            SLOT(processShowHide(int)));
    connect(ui->addSelectionButton, SIGNAL(clicked()), this,
            SLOT(addCurrentSelection()));
    connect(ui->cancelSelectionButton, SIGNAL(clicked()), this,
            SLOT(cancelCurrentSelection()));
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
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(0);
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
    connect(freeSelectForm->brushSizeSlider, SIGNAL(valueChanged(int)),
            freeSelectForm->brushSizeSpinner, SLOT(setValue(int)));
    connect(freeSelectForm->brushSizeSpinner, SIGNAL(valueChanged(int)),
            freeSelectForm->brushSizeSlider, SLOT(setValue(int)));
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

    setButtonsConfiguration();

#ifdef WASM_BUILD
    addImageConnector("Load File", [&]() {
            QHtml5File::load(".jpg, .png", [&](const QByteArray &content, const QString &fileName) {
                imageFileContent = content;
                this->filename = fileName;
                imagesList = (QStringList() << filename);
                openFile_and_labels();
            });
        });

    addIdproConnector("Load File", [&]() {
            QHtml5File::load(".idpro", [&](const QByteArray &content, const QString &fileName) {
                idproFileContent = content;
                idproFileName = fileName;
                loadLabelFile(idproFileName, PROJECT);
            });
        });

#endif
}

#ifdef WASM_BUILD
void InstaDam::addImageConnector(QString text, std::function<void ()> onActivate) {
    openImageConnector = new MyConnector;
    openImageConnector->onActivate = onActivate;
}

void InstaDam::addIdproConnector(QString text, std::function<void ()> onActivate) {
    openIdproConnector = new MyConnector;
    openIdproConnector->onActivate = onActivate;
}

#endif

/*!
  Destructor
*/
InstaDam::~InstaDam() {
    delete ui;
}


/*!
  Configures different buttons for local and server versions.
*/
void InstaDam::setButtonsConfiguration(){
    if(this->runningLocally){
        this->ui->menuUser->setEnabled(false);
        this->ui->actionDelete_2->setEnabled(false);
        this->ui->actionSave->setEnabled(true);
        this->ui->actionImport->setEnabled(true);
    }
    else {
        this->ui->actionImport->setEnabled(false);
        this->ui->actionSave->setEnabled(false);
    }
}
/*!
  Sets the current project to the newly created project.
*/
void InstaDam::setNewProject() {
    currentProject = newProject->newPr;
    setLabels();
    scene->clearItems();
    maskScene->clearItems();
    currentProjectLoaded = true;
    this->resetGUIclearLabels();
}

/*!
  Creates sets the label buttons to the labels of the current project.
*/
void InstaDam::setLabels() {
    clearLayout(ui->labelClassLayout);
    labelButtons.clear();
    for (int i = 0; i < currentProject->numLabels(); i++) {
        QSharedPointer<Label> label = currentProject->getLabel(i);
        LabelButton *button = new LabelButton(label);
        button->setText(label->getText());
        QPalette pal = button->palette();
        pal.setColor(QPalette::ButtonText, Qt::black);
        pal.setColor(QPalette::Button, label->getColor());
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        QHBoxLayout *hl = new QHBoxLayout();
        hl->addWidget(button->slider);
        hl->addWidget(button);
        ui->labelClassLayout->addLayout(hl);
        connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), this,
                SLOT(setCurrentLabel(QSharedPointer<Label>)));
        button->slider->setValue(50);
        connect(button, SIGNAL(opacity(QSharedPointer<Label>, int)), this,
                SLOT(setOpacity(QSharedPointer<Label>, int)));

        labelButtons.push_back(button);
    }
    setCurrentLabel(currentProject->getLabel(0));
}

/*!
 Creates a new project.
*/
void InstaDam::on_actionNew_triggered() {
    currentProject->resetLabels();
    newProject = new newproject(this);
    newProject->setModal(true);
    newProject->show();

#ifndef TEST
    if(this->runningLocally==false){
        connect(newProject, SIGNAL(sendProject()), this, SLOT(on_actionSave_triggered()));
    }
    else{
        connect(newProject, SIGNAL(sendProject()), this, SLOT(setNewProject()));
    }

#else
    QHashIterator<QString, QColor> it(labelHash);
    while (it.hasNext()) {
        it.next();
        newProject->setLabel(it.key(), it.value());
    }
#endif
}

/*!
 Sets the \a currentLabel to the \a myLabel of the LabelButton \a button
*/
void InstaDam::setCurrentLabel(LabelButton *button) {
    setCurrentLabel(button->myLabel);
}

/*!
 Sets the \a currentLabel to the Label \a label and cancels the current selection
*/
void InstaDam::setCurrentLabel(QSharedPointer<Label> label) {
    checkLabel(label);
    cancelCurrentSelection();
}

/*!
 Sets the \a currentLabel to the Label \a label
*/
void InstaDam::checkLabel(QSharedPointer<Label> label) {
    for (int i = 0; i < labelButtons.size(); i++) {
        if (label != labelButtons[i]->myLabel) {
            labelButtons[i]->setChecked(false);
        } else {
            labelButtons[i]->setChecked(true);
        }
    }
    currentLabel = label;
}

/*!
 Sets the opacity of the Label \a label to \a val.
*/
void InstaDam::setOpacity(QSharedPointer<Label> label, int val) {
    qInfo("Instadam Opacity");
    label->setOpacity(val);
    scene->update();
    maskScene->update();
}


/*!
 Removes all items from the QLayout \a layout.
*/
void InstaDam::clearLayout(QLayout *layout) {
    if (!layout)
       return;

    while (auto item = layout->takeAt(0)) {
        if (item) {
            delete item->widget();
            clearLayout(item->layout());
            layout->removeItem(item);
        }
    }
}


/*!
 Slot for opening a project file (.idpro).
*/
void InstaDam::on_actionOpen_triggered() {
    if(this->runningLocally){
#ifdef WASM_BUILD
        openIdproConnector->onActivate();
#else
        /* Reading and Loading */
        #ifndef TEST
            QString myfileName = QFileDialog::getOpenFileName(this,
                tr("Open Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
        #else
            QString myfileName = prjInName;

        #endif
        if (myfileName.isEmpty()) {
                assertError("Project file is invalid!");
                return;
        } else {
            loadLabelFile(myfileName, PROJECT);
            getReadyForNewProject();
            scene->update();
            maskScene->update();
        }
#endif
    }
    else{
        this->projecListUseCase = "OPEN";
        InstaDam::listProjects();
    }
}

/*!
    Reads label json information and updates the scene.
*/
void InstaDam::openFileFromJson(QJsonObject json){
    loadLabelJson(json, PROJECT);
    getReadyForNewProject();
    scene->update();
    maskScene->update();
}


/*!
 Gets Instadam Ready for opening a new project by clearing the undo stacks, setting the active stack and clearing scene.
*/
void InstaDam::getReadyForNewProject(){
    newProject = new newproject(this);
    mainUndoStack->clear();
    tempUndoStack->clear();
    undoGroup->setActiveStack(mainUndoStack);
    scene->clearItems();
    maskScene->clearItems();
}

/*!
 Toggles the mouse funciton for freeDrawSelect to drawing and not erasing.
*/
void InstaDam::toggleDrawing() {
    drawing = true;
    freeSelectForm->eraseButton->setChecked(false);
    freeSelectForm->drawButton->setChecked(true);
}

/*!
 Toggles the mouse funciton for freeDrawSelect to erasing and not drawing.
*/
void InstaDam::toggleErasing() {
    drawing = false;
    freeSelectForm->eraseButton->setChecked(true);
    freeSelectForm->drawButton->setChecked(false);
}

/*!
 Connects the filter buttons to the corresponding maskType.
*/
void InstaDam::connectFilters() {
    for (int i = 0; i < maskButtonList.size(); ++i) {
        for (int j = 0; j < maskButtonList.size(); ++j) {
            if (i == j) {
                maskButtonList[i]->setMaskType(maskTypeList[i]);
                connect(maskButtonList[i], SIGNAL(checked(EnumConstants::maskTypes)),
                        ui->IdmPhotoViewer, SLOT(setImMask(EnumConstants::maskTypes)));
            } else {
                connect(maskButtonList[i], SIGNAL(checked(EnumConstants::maskTypes)),
                        maskButtonList[j], SLOT(otherBoxChecked(EnumConstants::maskTypes)));
            }
        }
    }
    connect(ui->IdmPhotoViewer, SIGNAL(changedMask(EnumConstants::maskTypes)),
            ui->IdmMaskViewer, SLOT(setImMask(EnumConstants::maskTypes)));
    connect(ui->IdmPhotoViewer, SIGNAL(zoomed(int, float, QPointF)),
            ui->IdmMaskViewer, SLOT(zoomedInADifferentView(int, float, QPointF)));
    connect(ui->IdmMaskViewer, SIGNAL(zoomed(int, float, QPointF)),
            ui->IdmPhotoViewer, SLOT(zoomedInADifferentView(int, float, QPointF)));
    connect(ui->IdmPhotoViewer, SIGNAL(loadedPhoto()), this,
            SLOT(resetPixmapButtons()));
}

/*!
 Resets the pixmaps of the filter buttons.
*/
void InstaDam::resetPixmapButtons() {
    cv::Mat thumbnail = ui->IdmPhotoViewer->cvThumb;
    for (int i = 0; i < maskButtonList.size(); ++i) {
         maskButtonList[i]->resetPixmaps(filterControl->thumb2pixmap(thumbnail, maskTypeList[i]));
    }
}

/*!
 Sets the current freedrawselect brush size to \a size.
*/
void InstaDam::setCurrentBrushSize(int size) {
    currentBrushSize = size;
}

/*!
  Slot called when saving an annotation is complete.
*/
void InstaDam::saveAnnotationReplyFinished() {
    if (rep->error()) {
        qInfo() << rep->error();
        qInfo() << rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        return;
    }

    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);  // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject obj = jsonReply.object();
        qInfo() << obj;
    }
}

/*!
 Saves the current annotation to disk or the server.
*/
void InstaDam::on_actionSave_Annotation_triggered() {
    // Saving the file
    if (runningLocally) {
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
        outFile.write(saveDoc.toJson(QJsonDocument::Compact));
#endif
    } else {
        QJsonObject json;
        write(json, ANNOTATION);
        QString annotationsURL = this->databaseURL + "/" +
                InstaDamJson::ANNOTATION + "/";

        QUrl dabaseLink = QUrl(annotationsURL);

        QJsonObject js
        {
            {InstaDamJson::PROJECT_ID, this->currentProject->getId()},
            {InstaDamJson::IMAGE_ID, this->currentProject->getImageId()},
            {InstaDamJson::LABELS, json[InstaDamJson::LABELS]}
        };

        qInfo() << js;

        QJsonDocument doc(js);
        QByteArray bytes = doc.toJson();
        QNetworkRequest req = QNetworkRequest(dabaseLink);
        req.setHeader(QNetworkRequest::ContentTypeHeader,
                      InstaDamJson::APPLICATIONJSON);
        req.setRawHeader(InstaDamJson::AUTHORIZATION, InstaDamJson::BEARER + this->accessToken.toUtf8());

        rep = manager->post(req, bytes);

#ifdef WASM_BUILD
        connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(saveAnnotationReplyFin(QNetworkReply*)));
        connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

#endif
        connect(rep, &QNetworkReply::finished,
                        this, &InstaDam::saveAnnotationReplyFinished);
        QEventLoop loop;
        connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }
}

#ifdef WASM_BUILD
void InstaDam::saveAnnotationReplyFin(QNetworkReply* reply){
    rep = reply;
    saveAnnotationReplyFinished();
}
#endif

/*!
 Saves the current project to disk or the server.
*/
void InstaDam::on_actionSave_triggered() {
    if (this->runningLocally) {
    // Saving the file
    #ifdef WASM_BUILD
        QByteArray outFile;
    #else  // WASM_BUILD
    #ifndef TEST
        QString outFileName = QFileDialog::getSaveFileName(this,
           tr("Save Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
    #else  // TEST
        QString outFileName = prjOutName;
    #endif  // TEST
        if (QFileInfo(outFileName).suffix() != QString("idpro"))
            outFileName = outFileName +QString(".idpro");

        QFile outFile(outFileName);
        outFile.open(QIODevice::WriteOnly);
    #endif  // WASM_BUILD
        QJsonObject json;
        write(json, PROJECT);
        QJsonDocument saveDoc(json);
    #ifdef WASM_BUILD
        QString strJson(saveDoc.toJson(QJsonDocument::Compact));
        outFile.append(strJson);
        QHtml5File::save(outFile, "myproject.idpro");

    #else  // WASM_BUILD
        outFile.write(saveDoc.toJson());
    #endif  // WASM_BUILD
    } else {
        this->sProjectName = new serverProjectName();
//        this->sProjectName->setWindowFlags(Qt::WindowStaysOnTopHint);
        this->sProjectName->show();
        connect(this->sProjectName, SIGNAL(on_pushButton_clicked()), this, SLOT(saveToServer()));
        connect(this->sProjectName, SIGNAL(on_pushButton_clicked()), this, SLOT(setNewProject()));
    }
}

/*!
 Opens a file stored at Qstring \a path for the web-assembly version.
*/
void InstaDam::fileDownloaded(QString path) {
    this->oldFilename = this->filename;
    this->filename = path;
    this->oldFile = this->file;
    this->file = QFileInfo(filename);
    this->oldPath = this->path;
    this->path = file.dir();
    this->oldImagesList = this->imagesList;
    this->imagesList = this->path.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.JPEG", QDir::Files);
    if (imagesList.empty()) {
        assertError("That doesn't seem to be a valid image file.");
        revert();
    } else {
        int counter = 0;
        foreach(QString tempFilename, imagesList) {
           QFileInfo tempInfo = QFileInfo(tempFilename);
               if (file.completeBaseName() == tempInfo.completeBaseName()) {
                   break;
               }
              counter++;
            }
        fileId = counter;
        openFile_and_labels();
        QTextStream(stdout) << currentProject->numLabels() << "\n";
    }
}

/*!
 Waits for the reply for image reception.
*/
void InstaDam::imagesReplyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);  // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject obj = jsonReply.object();
        il = new ImageList(currentProject, nullptr, this->databaseURL, this->accessToken);
        il->show();
        il->addItems(obj);
        qInfo() << connect(il, &ImageList::fileDownloaded, this, &InstaDam::fileDownloaded);
        connect(il, &ImageList::clearGUI, this, &InstaDam::resetGUIclearLabels);
    }
}

/*!
 Opens an image and annotation file from the disk or server.
*/
void InstaDam::on_actionOpen_File_triggered() {
    if (runningLocally) {
        // QTextStream(stdout)<<currentProject->numLabels()<<"\n";
#ifdef WASM_BUILD
        if (currentProject->numLabels() == 0) {
            assertError("Please create or open a project first. Projects define the label classes and the color to annotate them. You can open or create a project from the Project menu.");
            return;
        }
        openImageConnector->onActivate();
#else

#ifndef TEST
        QString filename_temp = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "../", tr("Image Files (*.jpg *.png *.JPG *PNG *jpeg *JPEG );; All Files (*)"));
#else
        QString filename_temp = imgInName;
#endif  // TEST
        QString ext = QFileInfo(filename_temp).suffix();
        if (!ext.compare("png", Qt::CaseInsensitive) ||
            !ext.compare("jpg", Qt::CaseInsensitive) ||
            !ext.compare("jpeg", Qt::CaseInsensitive)) {
            this->oldFilename = this->filename;
            this->filename = filename_temp;
            this->oldFile = this->file;
            this->file = QFileInfo(filename);
            this->oldPath = this->path;
            this->path = file.dir();
            this->oldImagesList = this->imagesList;
            this->imagesList = path.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.JPEG", QDir::Files);
            if (imagesList.empty()) {
                assertError("That doesn't seem to be a valid image file.");
                revert();
            } else {
                int counter = 0;
                foreach(QString tempFilename, imagesList) {
                    QFileInfo tempInfo = QFileInfo(tempFilename);
                    if (file.completeBaseName() == tempInfo.completeBaseName()) {
                        break;
                    }
                    counter++;
                }

                fileId = counter;
                openFile_and_labels();
                QTextStream(stdout)<<currentProject->numLabels()<<"\n";
            }
        } else {
            assertError("That doesn't seem to be a valid image file.");
        }
#endif  // WASM_BUILD
    } else {
        if (currentProject->numLabels() == 0) {
            assertError("Please create or open a project first. Projects define the label classes and the color to annotate them. You can open or create a project from the Project menu.");
            return;
        }
        QString databaseImagesURL = this->databaseURL + "/" +
                InstaDamJson::PROJECTS + "/" +
                QString::number(currentProject->getId()) +
                "/" + InstaDamJson::IMAGES;
        QUrl dabaseLink = QUrl(databaseImagesURL);

        qInfo() << databaseImagesURL;

        QNetworkRequest req = QNetworkRequest(dabaseLink);
        req.setRawHeader(InstaDamJson::AUTHORIZATION,
                         InstaDamJson::BEARER + this->accessToken.toUtf8());
        rep = manager->get(req);
#ifdef WASM_BUILD
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(imagesReplyFin(QNetworkReply*)));
#else
        connect(rep, &QNetworkReply::finished,
                this, &InstaDam::imagesReplyFinished);
#endif
        qInfo() << "waiting for the reply...";
    }
}

#ifdef WASM_BUILD
void InstaDam::imagesReplyFin(QNetworkReply* reply){
    rep = reply;
    imagesReplyFinished();
}

void InstaDam::loadRawImage() {
    openFile_and_labels();
}
#endif
// Generates the file name for the next file in the folder

/*!
 * Waits for the file to be received.
*/
void InstaDam::fileReplyFinished() {
    qInfo() << "got a file";
    if (rep->error()) {
        qInfo() << "error getting file";
    }
    QUrl url = rep->url();
    qInfo() << url;
    QString path = QFileInfo(url.path()).fileName();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qInfo() << "could not save to disk";
    }

    file.write(rep->readAll());
    file.close();
    fileDownloaded(path);

    currentProject->setImageId(il->getIdList()[il->getSelectedIdIndex()]);
}

/*! Clears the guis and resets the labels()
 **/
void InstaDam::resetGUIclearLabels() {
    qInfo()<<"Reset!";
    scene->clearItems();
    maskScene->clearItems();
    currentProject->clearAllLabels();
    scene->update();
    maskScene->update();
}

/*!
 Saves the current annotation and opens the next image in the same folder that the current image
 is in.
*/
void InstaDam::on_saveAndNext_clicked() {
    saveAndProgress(1);
}

/*!
 Saves the current annotation and opens the previous image in the same folder that the current image
 is in.
*/
void InstaDam::on_saveAndBack_clicked() {
    saveAndProgress(-1);
}

/*!
  Save current annotations and continue. \a num is used to indicate the index of
  the current image.
*/
void InstaDam::saveAndProgress(int num)
{
    if (runningLocally and imagesList.empty()) {
            assertError("No file loaded! Please go to File->Open File and select an image to open");
    } else {
        on_actionSave_Annotation_triggered();

        if (runningLocally) {
            int newId = ((fileId+num)%imagesList.size()+imagesList.size())%imagesList.size();
            fileId = newId;
            this->filename = path.absolutePath()+"/"+imagesList[fileId];
            this->file = QFileInfo(this->filename);

            openFile_and_labels();
            qInfo("File opened");
        }
        else {
            if(currentProjectLoaded){
                if(this->currentProject->numLabels()!=0 and imagesList.isEmpty()==false){
                    int idIndex = il->getSelectedIdIndex();
                    il->setAnnotated();

                    QList<int> idList = il->getIdList();
                    int newId = ((idIndex+num)%idList.size()+idList.size())%idList.size();
                    QList<QString> pathList = il->getPathList();
                    QString filepath = this->databaseURL + "/" + pathList[newId];
                    il->setSelectedIdIndex(newId);
                    QNetworkRequest req = QNetworkRequest(filepath);
                    rep = manager->get(req);

#ifdef WASM_BUILD
                    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileReplyFin(QNetworkReply*)));

                    QEventLoop loop;
                    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
#else
                    connect(rep, &QNetworkReply::finished,
                            this, &InstaDam::fileReplyFinished);

                    QEventLoop loop;
                    connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
#endif
                    loop.exec();
                }
                else{
                    assertError("Please select an image first");
                }
            }
            else{
                assertError("Please create or open a project first. Projects define the label classes and the color to annotate them. You can open or create a project from the Project menu.");
            }
        }
    }
}

#ifdef WASM_BUILD
void InstaDam::fileReplyFin(QNetworkReply* reply){
    rep = reply;
    fileReplyFinished();
}
#endif

/*!
 Saves the current annotations as rasterized PNGs or as a buffer depending on the value of
 \a asBuffers
*/
void InstaDam::exportImages(bool asBuffers) {
    QString baseName = this->file.baseName();
    QString labelPath = this->path.absolutePath()+"/" + InstaDamJson::EXPORTS +
            "/" + baseName + "/";
    if (!QDir(labelPath).exists()) {
        QDir().mkpath(labelPath);
    }
    for (int i = 0; i < currentProject->numLabels(); i++) {
        QSharedPointer<Label> label = currentProject->getLabel(i);


        QString labfilePrefix = QString("%1").arg(i, 5, 10, QChar('0'));
        QString filename = labelPath + labfilePrefix + InstaDamJson::LABEL_PNG;
        qInfo()<<filename;
        if (asBuffers) {
            QByteArray *bytes = new QByteArray();
            QBuffer *buffer = new QBuffer(bytes);
            label->exportLabel(SelectItem::myBounds).save(buffer, "PNG");
            exportFiles.insert(filename, buffer);
        } else {
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            label->exportLabel(SelectItem::myBounds).save(&file, "PNG");
        }
    }
}

/*!
 Displays the std::string \a errorMessage as a QMessageBox.
*/
void InstaDam::assertError(std::string errorMessage) {
#ifndef TEST
    QMessageBox *messageBox = new QMessageBox;
    messageBox->critical(nullptr, "Error", QString::fromStdString(errorMessage));
    messageBox->setFixedSize(500, 200);
#else
    assertThrown = true;
    QTextStream(stdout) << "ERROR " << QString(errorMessage.c_str()) << endl;
#endif
}

/*!
 Generates the annotation and export file names to be stored locally based on the location
 and name of the image files.
*/
void InstaDam::generateLabelFileName() {
    QString baseName = this->file.baseName();
    ui->filelabel->setText(baseName);
    QString aPath = this->path.absolutePath()+"/annotations/"+baseName+"/";

    if (!QDir(aPath).exists()) {
        QDir().mkpath(aPath);
    }

    this->oldAnnotationPath = this->annotationPath;
    this->annotationPath = aPath+baseName+".idantn";

    this->path = file.dir();
}

/*!
 Uses  defined QStringList of images in the path as well as the id of the current file
 and opens the file. If annotations exist, the annotations are opened.
*/
void InstaDam::openFile_and_labels() {
    // Open labels
    generateLabelFileName();
    SelectItem::myBounds = QPixmap(filename).size();
    if (runningLocally == true) {
#ifdef WASM_BUILD
        SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromByteArray(imageFileContent);
#else

        if (QFileInfo(this->annotationPath).isFile()) {
            resetGUIclearLabels();
            if (!loadLabelFile(this->annotationPath, ANNOTATION)) {
                revert();
                return;
            }

            QTextStream(stdout) << "Loaded labels" << endl;;
        } else if (currentProject != nullptr && currentProject->numLabels() == 0) {
            assertError("Please create or open a project first. Projects define the label classes and the color to annotate them. You can open or create a project from the Project menu.");
            revert();
            return;
        }
        else {
           resetGUIclearLabels();
        }
#endif

    } else {
        resetGUIclearLabels();
        connect(il, &ImageList::allAnnotationsLoaded, this, &InstaDam::loadLabelJson);
        il->openAnnotation();
    }

    QTextStream(stdout) << "Loading photoX" << endl;;
    photoLoaded = true;

    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromFile(filename);
    qInfo("my bounds set");
    ui->IdmMaskViewer->LinkToPhotoViewer(ui->IdmPhotoViewer);

    qInfo("photo viewer linked!");
    populateSceneFromProjectLabels();
    scene->inactiveAll();
    mainUndoStack->clear();
    tempUndoStack->clear();
    undoGroup->setActiveStack(mainUndoStack);
    scene->update();
    maskScene->update();
}

/*!
 Populates the PhotoScene with the labels of the current project.
*/
void InstaDam::populateSceneFromProjectLabels() {
    for (int i = 0; i < currentProject->numLabels(); i++) {
        QSharedPointer<Label> label = currentProject->getLabel(i);

        if (!label->rectangleObjects.isEmpty()) {
            QHashIterator<int, RectangleSelect*> rit(label->rectangleObjects);
            while (rit.hasNext()) {
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
        if (!label->ellipseObjects.isEmpty()) {
            QHashIterator<int, EllipseSelect*> eit(label->ellipseObjects);
            while (eit.hasNext()) {
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
        if (!label->polygonObjects.isEmpty()) {
            QHashIterator<int, PolygonSelect*> pit(label->polygonObjects);
            while (pit.hasNext()) {
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
        if (!label->freeDrawObjects.isEmpty()) {
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
        label->setMaskState(ui->showMaskSelections->checkState());
    }
}

/*!
 InstaDam::loadLabelFile loads the Qstring \a filename of fileTypes
 \a fileType where \a fileType is either a PROJECT or ANNOTATION
*/
bool InstaDam::loadLabelFile(QString filename, fileTypes fileType) {
#ifdef WASM_BUILD
    QByteArray saveData = idproFileContent;
#else
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
#endif
    QTextStream(stdout) <<"Loaded file";

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    return loadLabelJson(loadDoc.object(), fileType);
}

/*!
 InstaDam::loadLabelJson loads the json object \a json of fileTypes
 \a fileType where \a fileType is either a PROJECT or ANNOTATION. Returns
 a \c bool indicating success (\c true) or failure (\c false) of the load.
*/
bool InstaDam::loadLabelJson(QJsonObject json, fileTypes fileType) {
    currentProject->resetLabels();
    clearLayout(ui->labelClassLayout);
    qInfo() << "inside loadLabelJson()";
    if (read(json, fileType)) {
        setLabels();
        scene->clearItems();
        maskScene->clearItems();
        currentProjectLoaded=true;
        return true;
    } else {
        return false;
    }
}

/*!
 * Toggles the pan mode.
*/
void InstaDam::panButton_clicked() {
    panning = !panning;  // !ui->panButton->isChecked();
    ui->panButton->setChecked(panning);
    ui->IdmPhotoViewer->setPanMode(panning);
    ui->IdmMaskViewer->setPanMode(panning);
}

/*!
 * Slot called when the Rectangle Select button is clicked.
*/
void InstaDam::on_rectangleSelectButton_clicked() {
    ui->rectangleSelectButton->setChecked(true);
    if (currentSelectType == SelectItem::Rectangle)
        return;
    inactivateSceneCancelSelection();
    switch (currentSelectType) {
        case SelectItem::Polygon:
            if (ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")) {
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            ui->polygonSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            if (ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")) {
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
    blankForm->blankText->setPlainText(rectBaseString);
    blankWidget->show();
    currentSelectType = SelectItem::Rectangle;
    scene->update();
    maskScene->update();
}

/*!
  Inactivates all SelectItems in the scene and calcels the current selection.
*/
void InstaDam::inactivateSceneCancelSelection() {
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
}

/*!
 * Slot called when the Ellispe Select button is clicked.
*/
void InstaDam::on_ellipseSelectButton_clicked() {
    ui->ellipseSelectButton->setChecked(true);
    if (currentSelectType == SelectItem::Ellipse)
        return;
    inactivateSceneCancelSelection();
    switch (currentSelectType) {
        case SelectItem::Polygon:
            if (ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")) {
                controlLayout->removeWidget(polygonSelectWidget);
                polygonSelectWidget->hide();
            }
            controlLayout->addWidget(blankWidget);
            ui->polygonSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            if (ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")) {
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
    blankForm->blankText->setPlainText(ellipseBaseString);
    blankWidget->show();
    currentSelectType = SelectItem::Ellipse;
    scene->update();
    maskScene->update();
}

/*!
 * Slot called when the Polygon Select button is clicked.
*/
void InstaDam::on_polygonSelectButton_clicked() {
    ui->polygonSelectButton->setChecked(true);
    if (currentSelectType == SelectItem::Polygon)
        return;
    inactivateSceneCancelSelection();
    switch (currentSelectType) {
        case SelectItem::Ellipse:
        case SelectItem::Rectangle:
            if (ui->selectControlFrame->findChild<QWidget*>("blankForm")) {
                controlLayout->removeWidget(blankWidget);
                blankWidget->hide();
            }
            controlLayout->addWidget(polygonSelectWidget);
            ui->ellipseSelectButton->setChecked(false);
            ui->rectangleSelectButton->setChecked(false);
            break;
        case SelectItem::Freeerase:
        case SelectItem::Freedraw:
            if (ui->selectControlFrame->findChild<QWidget*>("freeSelectForm")) {
                controlLayout->removeWidget(freeSelectWidget);
                freeSelectWidget->hide();
            }
            controlLayout->addWidget(polygonSelectWidget);
            ui->freeSelectButton->setChecked(false);
            break;
        case SelectItem::Polygon:
            break;
    }
    polygonSelectWidget->show();
    polygonSelectForm->polygonMessageBox->setPlainText(polygonBaseInstruction);
    currentSelectType = SelectItem::Polygon;
    scene->update();
}


/*!
 * Slot called when the Free Select button is clicked.
*/
void InstaDam::on_freeSelectButton_clicked() {
    ui->freeSelectButton->setChecked(true);
    if (currentSelectType == SelectItem::Freeerase ||
        currentSelectType == SelectItem::Freedraw)
        return;
    inactivateSceneCancelSelection();
    switch (currentSelectType) {
        case SelectItem::Ellipse:
        case SelectItem::Rectangle:
            if (ui->selectControlFrame->findChild<QWidget*>("blankForm")) {
                controlLayout->removeWidget(blankWidget);
                blankWidget->hide();
            }
            controlLayout->addWidget(freeSelectWidget);
            ui->ellipseSelectButton->setChecked(false);
            ui->rectangleSelectButton->setChecked(false);
            break;
        case SelectItem::Polygon:
            if (ui->selectControlFrame->findChild<QWidget*>("polygonSelectForm")) {
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
    freeSelectWidget->show();
    currentSelectType = SelectItem::Freedraw;
    scene->update();
    maskScene->update();
}


/*!
 * Slot called when the Free Select mode is changed to a round brush.
*/
void InstaDam::roundBrushButtonClicked() {
    brushMode = Qt::RoundCap;
    freeSelectForm->squareBrushButton->setChecked(false);
    freeSelectForm->roundBrushButton->setChecked(true);
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
    ui->IdmMaskViewer->setBrushMode(Qt::RoundCap);
}

/*!
 * Slot called when the Free Select mode is changed to a square brush.
*/
void InstaDam::squareBrushButtonClicked() {
    brushMode = Qt::SquareCap;
    freeSelectForm->squareBrushButton->setChecked(true);
    freeSelectForm->roundBrushButton->setChecked(false);
    ui->IdmPhotoViewer->setBrushMode(Qt::SquareCap);
    ui->IdmMaskViewer->setBrushMode(Qt::SquareCap);
}

/*!
 * Slot called when the filter options button is clicked.
*/
void InstaDam::on_filterOptions_clicked()
{
    if (currentProjectLoaded==false) {
        assertError("Please create or open a project first. Projects define the label classes and the color to annotate them. You can open or create a project from the Project menu.");
    } else {
        filterDialog* dialogs = new filterDialog(ui->IdmPhotoViewer->selectedMask,
                                                 filterControl, ui->IdmPhotoViewer,
                                                 currentProject);
        toggleFilterDialogOpen(0);
        connect(dialogs, SIGNAL(finished(int)), this, SLOT(toggleFilterDialogOpen(int)));
        connect(this, SIGNAL(colorChanged(cv::Scalar)), dialogs, SLOT(changeColor(cv::Scalar)));
        dialogs->show();
    }
}

/*!
  Toggles the filter dialog open window
  */
void InstaDam::toggleFilterDialogOpen(int r){
    qInfo()<<"Filter opened or closed!";
    filterOpenPickColor = !filterOpenPickColor;
}

/*!
 * Function to insert point between two points in the polygon.
*/
void InstaDam::setInsert() {
    currentItem->resetState();
    insertVertex = true;
    vertex1 = -1;
    vertex2 = -1;
    polygonSelectForm->polygonMessageBox->setPlainText("Click the vertices between which you want to insert a point.");
}

/*!
 * Slot called when the export annotations option is chosen.
*/
void InstaDam::on_actionExport_triggered() {
    exportImages();
}

/*!
 * Slot called when the export zip option is chosen.
*/
void InstaDam::on_actionExport_zip_triggered() {
    exportImages(true);
#ifdef WASM_BUILD
    QByteArray *outbytes;

    QBuffer *outbuffer = new QBuffer(outbytes);
    QuaZip zip(outbuffer);
#else
    QuaZip zip(this->filename + "_idpro.zip");
#endif
    if (!zip.open(QuaZip::mdCreate)) {
        return;
    }

    QuaZipFile outFile(&zip);
    QHashIterator<QString, QBuffer*> it(exportFiles);
    while (it.hasNext()) {
        it.next();
        QString filename = it.key();
        QBuffer *buffer = it.value();

        if (!(*buffer).open(QIODevice::ReadOnly)) {
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
            if ((l = (*buffer).read(buf, readSize)) != readSize) {
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

/*!
  Processes a point being clicked in the scene \a type, any SelectItem at \a pos
  is given by \a item, andthe button clicked is given as \a button.
*/
void InstaDam::processPointClicked(PhotoScene::viewerTypes type,
                                   SelectItem *item, QPointF pos,
                                   const Qt::MouseButton button,
                                   const Qt::KeyboardModifiers modifiers) {


    if (photoLoaded){
        currentButton = button;
        selectedViewer = type;
        if (button == Qt::LeftButton && QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true)
        {
            ctrlPanning = true;
            ui->panButton->setChecked(ctrlPanning);
            ui->IdmPhotoViewer->setPanMode(ctrlPanning);
            ui->IdmMaskViewer->setPanMode(ctrlPanning);
        }
        if (!panning && !ctrlPanning && !filterOpenPickColor){
            qInfo()<<"clicked!";
            if (!(modifiers & Qt::ShiftModifier)){
                annotationDraw(type, item, pos, button, modifiers);
            } else if((modifiers & Qt::ShiftModifier) && item != nullptr) {
               annotationTransform(type, item, pos, button, modifiers);
            } else {
                scene->inactiveAll();
                maskScene->inactiveAll();
                currentItem = nullptr;
            }
        }
        if (filterOpenPickColor){
            emit colorChanged(ui->IdmPhotoViewer->colorAtPoint(pos));
        }
    }
}

/*!
  Draws annotations on the PhotoScene indicated by \a type, with \a item,
  position \a pos, with mouse button \a button depressed, and any keyboard
  modifiers given by \a modifiers.
*/
int InstaDam::annotationDraw(PhotoScene::viewerTypes type,
                          SelectItem *item, QPointF pos,
                          const Qt::MouseButton button,
                          const Qt::KeyboardModifiers modifiers){
    if (!canDrawOnPhoto && (!currentItem || currentItem->type() !=
                            SelectItem::Polygon)) {
        cancelCurrentSelection();
        scene->inactiveAll();
        maskScene->inactiveAll();
        scene->update();
        maskScene->update();
    }

    if (type == PhotoScene::MASK_VIEWER_TYPE && currentSelectType !=
        SelectItem::Freedraw) {


        canDrawOnPhoto = false;
        undoGroup->setActiveStack(tempUndoStack);
        ui->addSelectionButton->setEnabled(true);
        ui->cancelSelectionButton->setEnabled(true);
    }

    if (!currentLabel || button == Qt::RightButton) {
        return -1;
    }

    if (currentItem && currentItem->type() == SelectItem::Polygon) {
        continueDrawingPolygon(pos);
        return -1;
    }
    qInfo()<<"clicked before selection!";
    scene->inactiveAll();
    maskScene->inactiveAll();
    switch (currentSelectType) {
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
             qInfo("Ellipse button was clicked0");
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
            if (drawing) {
                FreeDrawSelect *temp = new FreeDrawSelect(pos,
                                                          currentBrushSize,
                                                          brushMode,
                                                          currentLabel);
                FreeDrawSelect *mirror = new FreeDrawSelect(pos,
                                                            temp->myPen);
                tempItem = temp;
                mirrorItem = mirror;
                mirrorItem->setLabel(currentLabel);
                mirrorItem->updatePen(temp->myPen);
                tempItem->setMirror(mirrorItem);
                mirrorItem->setMirror(tempItem);
            } else {
                myErase = new FreeDrawErase(pos, currentBrushSize,
                                            brushMode, currentLabel);
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
            polygonSelectForm->finishPolygonButton->setEnabled(true);
        }
            break;
    }
    if ((currentSelectType != SelectItem::Freedraw && currentSelectType !=
         SelectItem::Freeerase) || drawing) {
        mirrorItem->setLabel(currentLabel);
        mirrorItem->updatePen(tempItem->myPen);
        scene->addItem(tempItem);
        maskScene->addItem(mirrorItem);
        switch (type) {
            case PhotoScene::PHOTO_VIEWER_TYPE:
                currentItem = tempItem;
                break;
            case PhotoScene::MASK_VIEWER_TYPE:
                currentItem = mirrorItem;
                break;
        }
        if (!canDrawOnPhoto)
            maskItem = currentItem;
    }
    scene->update();
    maskScene->update();
    return 0;

}

/*!
  Re-enable a PolygonSelect item for editing, at \a pos.
*/
void InstaDam::continueDrawingPolygon(QPointF pos)
{
    polygonSelectForm->finishPolygonButton->setEnabled(true);
    if (insertVertex && vertex1 >= 0 && vertex2 >= 0) {
        if (abs(vertex2 - vertex1) == 1) {
            currentItem->insertVertex(std::min(vertex1, vertex2), pos);
        } else {
            currentItem->insertVertex(std::max(vertex1, vertex2), pos);
        }
        vertex1 = -1;
        vertex2 = -1;
        insertVertex = false;
        polygonSelectForm->polygonMessageBox->setPlainText(currentItem->baseInstructions());
    } else {
        currentItem->addPoint(pos);
    }
    scene->update();
    maskScene->update();
}

/*!
  Transforms an annotation, \a item, on \a type, at \a pos, with \a button depressed
  and keyboard modifiers \a modifiers.
*/
int InstaDam::annotationTransform(PhotoScene::viewerTypes type,
                                  SelectItem *item, QPointF pos,
                                  const Qt::MouseButton button,
                                  const Qt::KeyboardModifiers modifiers) {
    if (item->type() != currentSelectType) {
        if (!canDrawOnPhoto)
            return -1;
        selectItemButton(item->type());
    }
    currentItem = item;
    if (!canDrawOnPhoto)
        maskItem = currentItem;

    checkLabel(currentItem->getLabel());
    scene->inactiveAll();
    maskScene->inactiveAll();
    currentItem->clickPoint(pos);
    currentItem->setItemActive();
    if (currentItem->type() == SelectItem::Polygon) {

        polygonSelectForm->finishPolygonButton->setEnabled(true);

        if (insertVertex) {
            int vert = currentItem->getActiveVertex();
            if (vert != SelectItem::UNSELECTED) {
                if (vertex1 < 0) {
                    vertex1 = vert;
                    polygonSelectForm->polygonMessageBox->appendPlainText("First vertex selected.");
                } else if (vertex2 < 0) {
                    if (abs(vert - vertex1) != 1 && abs(vert - vertex1) !=
                        (currentItem->numberOfVertices() - 1)) {
                        polygonSelectForm->polygonMessageBox->appendPlainText("Invalid second vertex, it must be adjacent to the first vertex.");
                    } else {
                        vertex2 = vert;
                        polygonSelectForm->polygonMessageBox->setPlainText("Click on the point where you want to insert a new vertex. "
                                                                           "(must be outside the current polygon, but can be dragged anywhere)");
                    }
                }
            }
        }
    }
    scene->update();
    maskScene->update();
}

/*!
 * Selects the appropriate button based on \a type.
*/
void InstaDam::selectItemButton(int type){
    selectItemButton(static_cast<SelectItem::SelectType>(type));
}

/*!
  Slot called when a selectItemButton is called with \a type, enabling the
  appropriate functionality.
*/
void InstaDam::selectItemButton(SelectItem::SelectType type){
    switch (type) {
        case SelectItem::Freedraw:
            on_freeSelectButton_clicked();
            break;
        case SelectItem::Polygon:
            on_polygonSelectButton_clicked();
            polygonSelectForm->finishPolygonButton->setEnabled(true);
            break;
        case SelectItem::Rectangle:
            on_rectangleSelectButton_clicked();
            break;
        case SelectItem::Ellipse:
            on_ellipseSelectButton_clicked();
            break;
    }
}

/*!
  Processes a mouse movement from \a fromPos to \a toPos, with keyboard
  modifiers \a modifiers.
*/
void InstaDam::processMouseMoved(QPointF fromPos, QPointF toPos,
                                 const Qt::KeyboardModifiers modifiers) {
    if (currentItem) {
        if (currentButton == Qt::LeftButton) {
            currentItem->moveItem(fromPos, toPos);
        } else {
            currentItem->rotate(fromPos, toPos);
        }
        scene->update();
        maskScene->update();
    }

}

/*!
  Processes a mouse button being released from scene \a type, mouse movement
  from \a oldPos to \a newPos, and the button released \a button.
*/
void InstaDam::processMouseReleased(PhotoScene::viewerTypes type,
                                    QPointF oldPos, QPointF newPos,
                                    const Qt::MouseButton button,
                                    const Qt::KeyboardModifiers modifiers) {
    UNUSED(button);
    UNUSED(modifiers);
    ctrlPanning = false;
    ui->panButton->setChecked(panning);
    ui->IdmPhotoViewer->setPanMode(panning);
    ui->IdmMaskViewer->setPanMode(panning);
    selectedViewer = type;

//    else if (!panning) {
    if (currentItem && !currentItem->isItemAdded()) {

        if (currentItem->type() == SelectItem::Freedraw && type ==
            PhotoScene::MASK_VIEWER_TYPE) {
            addCurrentSelection(true);

        } else if (currentItem->type() == SelectItem::Freeerase) {
            QUndoCommand *eraseCommand = new ErasePointsCommand(myErase, scene,
                                                                maskScene);
            undoGroup->activeStack()->push(eraseCommand);
        } else {
            QUndoCommand *addCommand =
                    new AddCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                       currentItem : currentItem->getMirror(),
                                   scene, this);
            undoGroup->activeStack()->push(addCommand);
            if (type == PhotoScene::MASK_VIEWER_TYPE) {
                currentItem->setFromMaskScene(true);
                currentItem->setOnMaskScene(true);
            }
            if (ui->showMaskSelections->checkState() == Qt::Unchecked)
                currentItem->hideMask();
        }
        currentItem->resetState();
        if (currentItem->type() != SelectItem::Polygon) {
            currentItem = nullptr;
        } else {
            currentItem->setActiveVertex(SelectItem::UNSELECTED);
        }
    } else if (currentItem && (currentItem->wasMoved() ||
                               currentItem->wasResized() ||
                               currentItem->wasRotated())) {

        if (currentItem->wasMoved()) {
            QUndoCommand *moveCommand =
                    new MoveCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                        currentItem : currentItem->getMirror(),
                                    oldPos, newPos);
            undoGroup->activeStack()->push(moveCommand);
        } else if (currentItem->wasResized()) {
            QUndoCommand *resizeCommand =
                    new MoveVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                              currentItem : currentItem->getMirror(),
                                          oldPos, newPos,
                                          currentItem->getActiveVertex());
            undoGroup->activeStack()->push(resizeCommand);
        } else {
            QUndoCommand *rotateCommand =
                    new RotateCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                          currentItem : currentItem->getMirror(),
                                      oldPos, newPos);
            undoGroup->activeStack()->push(rotateCommand);
        }
        currentItem->resetState();
    } else if (currentItem && currentItem->type() == SelectItem::Polygon &&
               currentItem->wasPointAdded()) {

        QUndoCommand *addVertexCommand =
                new AddVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                         currentItem : currentItem->getMirror(),
                                     newPos);
        undoGroup->activeStack()->push(addVertexCommand);
        currentItem->setActiveVertex(SelectItem::UNSELECTED);
        currentItem->resetState();
    }


    currentButton = Qt::NoButton;
    scene->update();
    maskScene->update();
//    }

}


/*!
 Closes the current polygon.
*/
void InstaDam::finishPolygonButtonClicked() {
    if (currentItem) {
        currentItem->setActiveVertex(SelectItem::UNSELECTED);
        currentItem->setInactive();
        polygonSelectForm->finishPolygonButton->setEnabled(false);
        currentItem = nullptr;
        scene->update();
        maskScene->update();
    }
}

/*!
 Defines keyboard modifier actions (delet and X) \a key for a viewerTypes of
 \a type.
*/
void InstaDam::processKeyPressed(PhotoScene::viewerTypes type, const int key) {

    if (!currentItem) {
        return;
    } else if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
        deleteCurrentObject(type);

    } else if (key == Qt::Key_X || key == Qt::Key_X + Qt::Key_Shift) {
        currentItem = nullptr;
    }
}

/*!
  Reads a QJsonObject \a json of fileTypes \a type.
*/
bool InstaDam::read(const QJsonObject &json, fileTypes type) {
    if (json.contains(InstaDamJson::LABELS) && json[InstaDamJson::LABELS].isArray()) {
        QJsonArray labelArray = json[InstaDamJson::LABELS].toArray();
        tempLabels.clear();
        tempLabels.reserve(labelArray.size());
        for (int i = 0; i < labelArray.size(); i++) {
            int labelId;
            if(runningLocally)
            {
                labelId = i;
            }
            else
            {
                labelId =  labelArray[i].toObject().value("label_id").toInt();;
            }
            QSharedPointer<Label> label =
                    QSharedPointer<Label>::create(labelArray[i].toObject(), labelId);
            tempLabels.push_back(label);

        }
        if (type == PROJECT) {
            currentProject = newProject->newPr;
            setLabels();
            scene->clearItems();
            maskScene->clearItems();
            currentProject->setLabels(tempLabels);
            QTextStream(stdout) << currentProject->numLabels() << "\n";
        } else {
            qInfo() << "reading annotation file";

            QStringList current = getLabelNames(currentProject->getLabels());
            QStringList newList = getLabelNames(tempLabels);
            current.sort(Qt::CaseInsensitive);
            newList.sort(Qt::CaseInsensitive);
            if (current.size() != 0 && current != newList) {
                QDialog *dialog = new QDialog(this);
                Ui::projectDialog *projectDialog = new Ui::projectDialog;
                projectDialog->setupUi(dialog);
                projectDialog->differences->setRowCount(std::max(current.size(),
                                                                 newList.size()));
                for (int i = 0; i < current.size(); i++) {
                    QTableWidgetItem *newItem = new QTableWidgetItem(current.at(i));
                    projectDialog->differences->setItem(i, 0, newItem);
                }
                for (int i = 0; i< newList.size(); i++) {
                    QTableWidgetItem *newItem = new QTableWidgetItem(newList.at(i));
                    projectDialog->differences->setItem(i, 1, newItem);
                }

                connect(projectDialog->keepButton, SIGNAL(clicked()), dialog,
                        SLOT(accept()));
                connect(projectDialog->cancelButton, SIGNAL(clicked()), dialog,
                        SLOT(reject()));
                connect(projectDialog->loadNewButton, &QPushButton::clicked, dialog,
                        [=]() {dialog->done(2);});
                dialog->exec();
                if (dialog->result() == QDialog::Rejected) {
                    return false;
                } else if (dialog->result() == 2) {
                    currentProject->setLabels(tempLabels);
                }
            } else {
                currentProject->setLabels(tempLabels);
            }
            QTextStream(stdout) << currentProject->numLabels();
        }
    }
    return true;
}

/*!
  Writes a QJsonObject \a json of fileTypes \a type.
*/
void InstaDam::write(QJsonObject &json, fileTypes type) {
    QJsonArray labs;
    for (int i = 0; i < currentProject->numLabels(); i++) {
        QJsonObject lab;
        if (type== PROJECT) {
            currentProject->getLabel(i)->write(lab);
        } else if (type == ANNOTATION) {
            currentProject->getLabel(i)->writeIdantn(lab);
        }
        labs.append(lab);
    }
    json[InstaDamJson::LABELS] = labs;
}

/*!
  Rasterizes the selection from the mask viewer, \a useCurrent determines
  whether to use the PhotoViewer item (\c true) or MaskViewer item (\c false).
*/
void InstaDam::addCurrentSelection(bool useCurrent) {
    SelectItem *item;
    item = useCurrent ? currentItem : maskItem;
    if (item->type() == SelectItem::Polygon)
        polygonSelectForm->finishPolygonButton->setEnabled(false);
    tempUndoStack->clear();
    undoGroup->setActiveStack(mainUndoStack);
    FreeDrawSelect *fds = new FreeDrawSelect(maskSelection(item),
                                             currentLabel);
    scene->addItem(fds);
    QUndoCommand *addCommand = new AddCommand(fds, scene, this);
    mainUndoStack->push(addCommand);
    scene->removeItem(item->getMirror());
    maskScene->removeItem(item->getMirror());
    currentLabel->removeItem(item->getMirror()->myID);
    currentLabel->removeItem(item->myID);
    currentLabel->addItem(fds);
    scene->removeItem(item);
    maskScene->removeItem(item);
    maskItem = nullptr;
    ui->addSelectionButton->setDisabled(true);
    ui->cancelSelectionButton->setDisabled(true);
    canDrawOnPhoto = true;
}

/*!
 Cancels the current selection.
*/
void InstaDam::cancelCurrentSelection() {
    undoGroup->setActiveStack(mainUndoStack);
    canDrawOnPhoto = true;
    if (!ui->addSelectionButton->isEnabled()) {
        return;
    }
    tempUndoStack->clear();
    scene->removeItem(maskItem->getMirror());
    maskScene->removeItem(maskItem->getMirror());
    scene->removeItem(maskItem);
    maskScene->removeItem(maskItem);
    maskItem = nullptr;
    ui->addSelectionButton->setDisabled(true);
    ui->cancelSelectionButton->setDisabled(true);
}

/*!
 * Conducts the pixmap operation of masking on the SelectItem*\a item.
*/
QPixmap InstaDam::maskSelection(SelectItem *item) {
    QPixmap map(SelectItem::myBounds);
    map.fill(Qt::transparent);
    QPainter *paint = new QPainter(&map);
    QBrush brush(currentLabel->getColor());
    paint->setPen(currentLabel->getColor());
    paint->setBrush(brush);
    paint->setCompositionMode(QPainter::CompositionMode_SourceOver);
    item->toPixmap(paint);
    paint->end();
    map = joinPixmaps(ui->IdmMaskViewer->photo->pixmap(), map,
                      QPainter::CompositionMode_DestinationIn);
    return map;
}

/*!
  Slot for ?
*/
void InstaDam::on_addSelectionButton_clicked() {
}

/*!
 Sets the id of the current project to \a id.
 */
void InstaDam::setCurrentProjectId(int id)
{
    this->currentProject->setId(id);
}

/*!
 Gets the list of label names from QVector \a labels.
*/
QStringList InstaDam::getLabelNames(QVector<QSharedPointer<Label> > labels) {
    QStringList labelList;
    QVectorIterator<QSharedPointer<Label> > it(labels);
    while (it.hasNext()) {
        labelList.append(it.next()->getText());
    }
    return labelList;
}

/*!
 Reverts to the current label list during an annotation opening conflict.
*/
void InstaDam::revert() {
    this->filename = this->oldFilename;
    this->file = this->oldFile;
    this->path = this->oldPath;
    this->imagesList = this->oldImagesList;
    this->annotationPath = this->oldAnnotationPath;

}

/*!
 Toggles hiding the labels on the mask viewer, by using \a state.
*/
void InstaDam::processShowHide(int state) {
    for (int i = 0; i < currentProject->numLabels(); i++) {
         currentProject->getLabel(i)->setMaskState(state);
    }
}

/*!
 Starts a user search widget.
*/
void InstaDam::on_actionSearch_triggered() {
    if (this->runningLocally==false) {
        if (currentProject->numLabels()!=0){
            AddUserToProject *addUserToProjectInterface = new AddUserToProject;
            addUserToProjectInterface->projectId = this->currentProject->getId();
            addUserToProjectInterface->accessToken = this->accessToken;
            addUserToProjectInterface->databaseURL = this->databaseURL;
            addUserToProjectInterface->show();
        } else {
            assertError("Please open a project first");
        }
    }
}

/*!
 Starts a user search widget.
*/
void InstaDam::on_actionUpdate_Privilege_triggered() {
    if (this->runningLocally==false) {
        on_actionSearch_triggered();
    }
}

/*!
 Sends a request to receive all the projects assoicated with a user
*/
void InstaDam::listProjects() {
    QString databaseProjectsURL = this->databaseURL + "/" +
            InstaDamJson::PROJECTS;
    QUrl dabaseLink = QUrl(databaseProjectsURL);
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = InstaDamJson::BEARER + this->accessToken;
    req.setRawHeader(InstaDamJson::AUTHORIZATION, loginToken.QString::toUtf8());
    rep = manager->get(req);
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(projectsReplyFin(QNetworkReply*)));
#else
    connect(rep, &QNetworkReply::finished,
            this, &InstaDam::projectsReplyFinished);
#endif
}

#ifdef WASM_BUILD
void InstaDam::projectsReplyFin(QNetworkReply* reply){
    rep = reply;
    projectsReplyFinished();
}
#endif

/*!
 Receives the reply regarding user projects
*/
void InstaDam::projectsReplyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);

    if (jsonError.error == QJsonParseError::NoError) {
        QJsonObject obj = jsonReply.object();
        ProjectList *pl = new ProjectList;
        pl->useCase = this->projecListUseCase;
        pl->show();
        pl->addItems(jsonReply, this->databaseURL, this->accessToken);
        connect(pl, &ProjectList::instadamClearAll, this,
                &InstaDam::getReadyForNewProject);
        connect(pl, &ProjectList::projectJsonReceived, this,
                &InstaDam::openFileFromJson);
        connect(pl, &ProjectList::projectIdChanged, this,
                &InstaDam::setCurrentProjectId);
        connect(pl, &ProjectList::projectDeleted, this,
                &InstaDam::currentProjectDeleted);
    }
}

/*!
 Checks if the deleted project is the currenlty open project and clears the labels if so.
 */
void InstaDam::currentProjectDeleted(int deletedProjectId){
    if(deletedProjectId==this->currentProject->getId()){
        assertError("The current project was deleted! Pleaese open or create another project");
        this->currentProject->resetLabels();
        clearLayout(ui->labelClassLayout);
        this->resetGUIclearLabels();
        currentProjectLoaded=false;
    }
}


/*!
 Sends a request to save new project information (name) on the server
*/
void InstaDam::saveToServer(){
    qInfo() << "saving project to server";
    QUrl dabaseLink = QUrl(this->databaseURL + "/" + InstaDamJson::PROJECT);
    QString fileName = this->sProjectName->ui->projectName->toPlainText();
    this->sProjectName->hide();
    QJsonObject js
    {
        {"project_name", fileName}
    };
    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = InstaDamJson::BEARER + this->accessToken;
    req.setRawHeader(InstaDamJson::AUTHORIZATION, loginToken.QString::toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  InstaDamJson::APPLICATIONJSON);
    rep = manager->post(req, bytes);
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFin(QNetworkReply*)));
#else
    connect(rep, &QNetworkReply::finished,
            this, &InstaDam::replyFinished);
#endif
}

#ifdef WASM_BUILD
void InstaDam::replyFin(QNetworkReply* reply){
    rep = reply;
    replyFinished();
}
#endif

/*!
 Receives the reply of saving a project name on the server
 Sends requests to save the labels assoicated with the new project
*/
void InstaDam::replyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if(jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject reply = jsonReply.object();
        this->currentProject->setId(reply.value(InstaDamJson::PROJECT_ID).toInt());

        // save labels
        qInfo() << "saving labels to server";

        QUrl lablesDatabaseLink = QUrl(this->databaseURL + "/" +
                                       InstaDamJson::PROJECT + "/" +
                                       QString::number(this->currentProject->getId()) +
                                       "/" + InstaDamJson::LABELS);
        QString loginToken = InstaDamJson::BEARER + this->accessToken;

        for (int i=0; i<this->currentProject->numLabels();i++) {
            QJsonObject jsLabel
            {
                {InstaDamJson::LABEL_TEXT,
                            this->currentProject->getLabel(i)->getText()},
                {InstaDamJson::LABEL_COLOR,
                            this->currentProject->getLabel(i)->getColor().name()}
            };

            QJsonDocument docLabel(jsLabel);
            QByteArray bytesLabel = docLabel.toJson();
            QNetworkRequest reqLabel = QNetworkRequest(lablesDatabaseLink);

            reqLabel.setRawHeader(InstaDamJson::AUTHORIZATION,
                                  loginToken.QString::toUtf8());
            reqLabel.setHeader(QNetworkRequest::ContentTypeHeader,
                               InstaDamJson::APPLICATIONJSON);
            rep = manager->post(reqLabel, bytesLabel);
#ifdef WASM_BUILD
            connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(labelReplyFin(QNetworkReply*)));

            QEventLoop loop;
            connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
#else
            connect(rep, &QNetworkReply::finished,
                    this, &InstaDam::labelReplyFinished);

            QEventLoop loop;
            connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
#endif
        }
        labelIdsRecieved = 0;
    }
}

#ifdef WASM_BUILD
void InstaDam::labelReplyFin(QNetworkReply* reply){
    rep = reply;
    labelReplyFinished();
}
#endif

/*!
 Receives the reply of saving a label on the server
 Sets the id of the currentprject label to the id received from the backend
*/
void InstaDam::labelReplyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if(jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        QJsonObject reply = jsonReply.object();
        int label_id = reply.value("label_id").toInt();
        qInfo() << label_id;
        this->currentProject->getLabel(labelIdsRecieved)->setId(label_id);
        labelIdsRecieved++;
    }
}

/*!
 Starts a projectList widget with a use of "DELETE" to delete a project when double clicking
*/
void InstaDam::on_actionDelete_2_triggered() {
    this->projecListUseCase = "DELETE";
    InstaDam::listProjects();
}

/*!
  Slot called when the delete action is called.
*/
void InstaDam::on_actionDelete_triggered() {
    deleteCurrentObject(selectedViewer);
}

/*!
  Deletes the current SelectItem on \a phototype
*/
void InstaDam::deleteCurrentObject(PhotoScene::viewerTypes phototype) {

    if (currentItem != nullptr) {
        if (currentItem->type() == SelectItem::Polygon &&
            currentItem->getActiveVertex() != SelectItem::UNSELECTED) {
            QUndoCommand *deleteVertexCommand =
                    new DeleteVertexCommand((phototype == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                                currentItem : currentItem->getMirror());
            undoGroup->activeStack()->push(deleteVertexCommand);
        } else {
            selectItemButton(currentItem->type());
            QUndoCommand *deleteCommand =
                    new DeleteCommand((phototype == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                          currentItem : currentItem->getMirror(),
                                      scene, this);
            undoGroup->activeStack()->push(deleteCommand);
        }
    }
}

/*!
  Slot called when editing a SelectItem.
*/
void InstaDam::on_actionEdit_Label_triggered() {
    if (currentItem != nullptr) {
        if ((currentItem->type() != SelectItem::Freedraw && currentItem->type() !=
             SelectItem::Freeerase)) {
            chooseLabelDialog* chooseLabel = new chooseLabelDialog(currentProject);
            connect(chooseLabel, SIGNAL(labelPicked(QSharedPointer<Label>)),
                    this, SLOT(editLabel(QSharedPointer<Label>)));
            return;
        }
    }
    assertError("Shift+select item whose label needs to be edited.");
}

/*!
  Edit label given as \a newLabel.
*/
void InstaDam::editLabel(QSharedPointer<Label> newLabel) {
    QUndoCommand *editLabelCommand =
            new EditLabelCommand((selectedViewer == PhotoScene::PHOTO_VIEWER_TYPE) ?
                                        currentItem : currentItem->getMirror(), newLabel,
                                 currentItem->getLabel(), scene, this);
    undoGroup->activeStack()->push(editLabelCommand);
}

/*!
  Slot called when import is clicked. Imports pngs.
*/
void InstaDam::on_actionImport_triggered() {
    if (!photoLoaded) {
        assertError("Please load an image first!");
        return;
    }

    QString baseName = this->file.baseName();
    QString labelPath = this->path.absolutePath() + "/" +
            InstaDamJson::LABELS + "/" + baseName + "/";
    QString message = QString("Please copy all labels to ") + labelPath +
                    " and name them as 00000_label.png, 00001_label.png, etc";
    if (!QDir(labelPath).exists()) {
        assertError(message.toStdString());
        return;
    }
    bool foundALabel = false;
    qInfo()<<labelPath;

    for (int i=0; i<currentProject->numLabels(); i++) {
        QString labfilePrefix = QString("%1").arg(i, 5, 10, QChar('0'));
        QString labelFile = labelPath + labfilePrefix + InstaDamJson::LABEL_PNG;
        QFileInfo check_file(labelFile);
        if (check_file.exists()) {
            qInfo()<<labelFile;
            QPixmap pixmap = QPixmap(labelFile);
            QSharedPointer<Label> lab = currentProject->getLabel(i);
            undoGroup->setActiveStack(mainUndoStack);

            FreeDrawSelect *fds = new FreeDrawSelect(pixmap, lab, nullptr, true);
            qInfo()<<fds->foundPixels;
            if (fds->foundPixels){
                lab->addItem(fds);
                scene->addItem(fds);
                QUndoCommand *addCommand = new AddCommand(fds, scene, this);
                mainUndoStack->push(addCommand);
            }

            foundALabel = true;
        }
    }
    if(!foundALabel)
        assertError(message.toStdString());
    scene->update();
}

/*!
  \fn void InstaDam::setCurrentItem(SelectItem *item, bool enable)

  Set \a item to be the currently active SelectItem. \a enable indicates whenther
  (\c true) or not (\c false) the finishPolygon Button needs to be enabled.
  */

void InstaDam::on_actionClear_All_can_t_undo_triggered()
{
    this->resetGUIclearLabels();
}


