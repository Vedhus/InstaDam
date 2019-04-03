#include "instadam.h"

#include <QByteArray>
#include <QDialog>

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
#include "Selector/commands.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "startingwidget.h"
#include "imagelist.h"
#ifdef WASM_BUILD
#include "htmlFileHandler/qhtml5file.h"
#endif

InstaDam::InstaDam(QWidget *parent, QString databaseURL, QString token) :
    QMainWindow(parent), ui(new Ui::InstaDam) {
    ui->setupUi(this);
    filterControl = new filterControls();
    maskTypeList = { BLUR, CANNY, THRESHOLD, LABELMASK};
    maskButtonList = {ui->blur_label, ui->canny_label, ui->threshold_label,
                      ui->labelmask_label};
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
                                       QPointF, const Qt::MouseButton)), this,
            SLOT(processPointClicked(PhotoScene::viewerTypes, SelectItem*,
                                     QPointF, const Qt::MouseButton)));
    connect(scene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(scene, SIGNAL(mouseReleased(PhotoScene::viewerTypes, QPointF,
                                        QPointF, const Qt::MouseButton)), this,
            SLOT(processMouseReleased(PhotoScene::viewerTypes, QPointF, QPointF,
                                      const Qt::MouseButton)));
    connect(scene, SIGNAL(keyPressed(PhotoScene::viewerTypes, const int)), this,
            SLOT(processKeyPressed(PhotoScene::viewerTypes, const int)));
    connect(maskScene, SIGNAL(pointClicked(PhotoScene::viewerTypes, SelectItem*,
                                           QPointF, const Qt::MouseButton)), this,
            SLOT(processPointClicked(PhotoScene::viewerTypes, SelectItem*,
                                     QPointF, const Qt::MouseButton)));
    connect(maskScene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(maskScene, SIGNAL(mouseReleased(PhotoScene::viewerTypes, QPointF,
                                            QPointF, const Qt::MouseButton)), this,
            SLOT(processMouseReleased(PhotoScene::viewerTypes, QPointF, QPointF,
                                      const Qt::MouseButton)));
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

    StartingWidget *sw = new StartingWidget;
    sw->show();

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
                loadLabelFile(idproFileName);
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
InstaDam::~InstaDam() {
    delete ui;
}

void InstaDam::setNewProject() {
    currentProject = newProject->newPr;
    setLabels();
    scene->clearItems();
    maskScene->clearItems();
}

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
        qInfo("Button Added Start!");
        connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), this,
                SLOT(setCurrentLabel(QSharedPointer<Label>)));
        button->slider->setValue(50);
        connect(button, SIGNAL(opacity(QSharedPointer<Label>, int)), this,
                SLOT(setOpacity(QSharedPointer<Label>, int)));

        labelButtons.push_back(button);

        qInfo("Button Added End!");
    }
}

void InstaDam::on_actionNew_triggered() {
    currentProject->resetLabels();
    newProject = new newproject(this);
    newProject->setModal(true);
    connect(newProject, SIGNAL(accepted()), this, SLOT(setNewProject()));
    newProject->exec();
    setNewProject();
}

void InstaDam::setCurrentLabel(LabelButton *button) {
    currentLabel = button->myLabel;
}

void InstaDam::setOpacity(QSharedPointer<Label> label, int val) {
    qInfo("Instadam Opacity");
    label->setOpacity(val);
    scene->update();
    maskScene->update();
}

void InstaDam::setCurrentLabel(QSharedPointer<Label> label) {
    if (currentLabel == label)
        return;
    for (int i = 0; i < labelButtons.size(); i++) {
        if (label != labelButtons[i]->myLabel) {
            labelButtons[i]->setChecked(false);
        } else {
            labelButtons[i]->setChecked(true);
        }
    }
    currentLabel = label;
    cancelCurrentSelection();
}

void InstaDam::clearLayout(QLayout * layout) {
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

void InstaDam::on_actionOpen_triggered() {
#ifdef WASM_BUILD
      openIdproConnector->onActivate();
#else
    // Reading and Loading
    QString myfileName = QFileDialog::getOpenFileName(this,
        tr("Open Project"), "../", tr("Instadam Project (*.idpro);; All Files (*)"));
    if (myfileName.isEmpty()) {
            return;  // remove that part and add an alert
    } else {
        currentProject = newProject->newPr;
        currentProject->resetLabels();
        clearLayout(ui->labelClassLayout);
        scene->clearItems();
        maskScene->clearItems();
        loadLabelFile(myfileName, PROJECT);
        mainUndoStack->clear();
        tempUndoStack->clear();
        undoGroup->setActiveStack(mainUndoStack);
        scene->clearItems();
        maskScene->clearItems();
        scene->update();
        maskScene->update();
    }
#endif
}

void InstaDam::toggleDrawing() {
    drawing = true;
    freeSelectForm->eraseButton->setChecked(false);
    freeSelectForm->drawButton->setChecked(true);
}

void InstaDam::toggleErasing() {
    drawing = false;
    freeSelectForm->eraseButton->setChecked(true);
    freeSelectForm->drawButton->setChecked(false);
}

void InstaDam::connectFilters() {
    for (int i = 0; i < maskButtonList.size(); ++i) {
        for (int j = 0; j < maskButtonList.size(); ++j) {
            if (i == j) {
                maskButtonList[i]->setMaskType(maskTypeList[i]);
                connect(maskButtonList[i], SIGNAL(checked(maskTypes)),
                        ui->IdmPhotoViewer, SLOT(setImMask(maskTypes)));
            } else {
                connect(maskButtonList[i], SIGNAL(checked(maskTypes)),
                        maskButtonList[j], SLOT(otherBoxChecked(maskTypes)));
            }
        }
    }
    connect(ui->IdmPhotoViewer, SIGNAL(changedMask(maskTypes)),
            ui->IdmMaskViewer, SLOT(setImMask(maskTypes)));
    connect(ui->IdmPhotoViewer, SIGNAL(zoomed(int, float, QPointF)),
            ui->IdmMaskViewer, SLOT(zoomedInADifferentView(int, float, QPointF)));
    connect(ui->IdmMaskViewer, SIGNAL(zoomed(int, float, QPointF)),
            ui->IdmPhotoViewer, SLOT(zoomedInADifferentView(int, float, QPointF)));
    connect(ui->IdmPhotoViewer, SIGNAL(loadedPhoto()), this,
            SLOT(resetPixmapButtons()));
}

void InstaDam::resetPixmapButtons() {
    cv::Mat thumbnail = ui->IdmPhotoViewer->cvThumb;
    for (int i = 0; i < maskButtonList.size(); ++i) {
         maskButtonList[i]->resetPixmaps(filterControl->thumb2pixmap(thumbnail, maskTypeList[i]));
    }
}

void InstaDam::setCurrentBrushSize(int size) {
    currentBrushSize = size;
}

void InstaDam::on_actionSave_Annotation_triggered() {
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
        outFile.write(saveDoc.toJson(QJsonDocument::Compact));
    #endif
}

void InstaDam::on_actionSave_triggered() {
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

void InstaDam::imagesReplyFinished() {
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);  // parse and capture the error flag

      if (jsonError.error != QJsonParseError::NoError) {
            qInfo() << "Error: " << jsonError.errorString();
      } else {
          QJsonObject obj = jsonReply.object();
          ImageList* il = new ImageList(nullptr, this->databaseURL, this->accessToken);
          il->show();
          il->addItems(obj);
          qInfo() << connect(il, &ImageList::fileDownloaded, this, &InstaDam::fileDownloaded);
      }
}


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
        QString filename_temp = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "../", tr("Image Files (*.jpg *.png *.JPG *PNG *jpeg *JPEG );; All Files (*)"));
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
                QTextStream(stdout) << currentProject->numLabels() << "\n";
            }
        } else {
            assertError("That doesn't seem to be a valid image file.");
        }
    } else {
        QString databaseImagesURL = this->databaseURL+"/projects/1/images";
        QUrl dabaseLink = QUrl(databaseImagesURL);

        qInfo() << databaseImagesURL;

        QNetworkRequest req = QNetworkRequest(dabaseLink);
        req.setRawHeader("Authorization", "Bearer " + this->accessToken.toUtf8());
        // debugRequest(req);
        rep = manager->get(req);

        connect(rep, &QNetworkReply::finished,
                this, &InstaDam::imagesReplyFinished);

        qInfo() << "waiting for the reply...";
    }
#endif
}

#ifdef WASM_BUILD
void InstaDam::loadRawImage() {
    openFile_and_labels();
}
#endif
// Generates the file name for the next file in the folder

void InstaDam::on_saveAndNext_clicked() {
    if (imagesList.empty()) {
            assertError("No file loaded! Please go to File->Open File and select an image to open");
    } else {

        qInfo("Going to save idantn");
        on_actionSave_Annotation_triggered();
        qInfo("saved idantn");
        int newId = (fileId+1)%imagesList.size();
        QTextStream(stdout) << "NewId = " << newId << "\n";
        scene->clearItems();
        maskScene->clearItems();
        for (int i = 0; i < currentProject->getLabels().size(); i++) {
            currentProject->getLabel(i)->clear();
        }

        fileId = newId;
        this->filename = path.absolutePath()+"/"+imagesList[fileId];
        this->file = QFileInfo(this->filename);
        openFile_and_labels();

        qInfo("File opened");
    }
}

void InstaDam::on_saveAndBack_clicked() {
    if (imagesList.empty()) {
            assertError("No file loaded! Please go to File->Open File and select an image to open");
    } else {
        qInfo("Going to save idantn");
        on_actionSave_Annotation_triggered();
        qInfo("saved idantn");
        int newId = ((fileId-1)%imagesList.size()+imagesList.size())%imagesList.size();
        scene->clearItems();
        maskScene->clearItems();
        for (int i = 0; i < currentProject->getLabels().size(); i++) {
            currentProject->getLabel(i)->clear();
        }

        fileId = newId;
        this->filename = path.absolutePath()+"/"+imagesList[fileId];
        this->file = QFileInfo(this->filename);
        openFile_and_labels();

        qInfo("File opened");
    }
}

void InstaDam::exportImages(bool asBuffers) {
    QString baseName = this->filename;
    for (int i = 0; i < currentProject->numLabels(); i++) {
        QSharedPointer<Label> label = currentProject->getLabel(i);
        QString filename = baseName + "_" + label->getText() + ".png";
        if (asBuffers) {
            QByteArray *bytes = new QByteArray();
            QBuffer *buffer = new QBuffer(bytes);
            label->exportLabel(SelectItem::myBounds).save(buffer, "PNG");
            exportFiles.insert(filename, buffer);
        } else {
            label->exportLabel(SelectItem::myBounds).save(filename, "PNG");
        }
    }
}

void InstaDam::assertError(std::string errorMessage) {
    QMessageBox *messageBox = new QMessageBox;
    messageBox->critical(nullptr, "Error", QString::fromStdString(errorMessage));
    messageBox->setFixedSize(500, 200);
}

// Uses name of current file and generates name of label image.
void InstaDam::generateLabelFileName() {
    QString baseName = this->file.baseName();
    QString aPath = this->path.absolutePath()+"/annotations/"+baseName+"/";
    QString exPath = this->path.absolutePath()+"/exports/"+baseName+"/";
    if (!QDir(aPath).exists()) {
        QDir().mkpath(aPath);
    }
    if (!QDir(exPath).exists()) {
        QDir().mkpath(exPath);
    }
    this->oldLabelPaths = this->labelPaths;
    labelPaths.clear();
    this->oldAnnotationPath = this->annotationPath;
    this->annotationPath = aPath+baseName+".idantn";
    this->path = file.dir();
}

// This function uses the defined QStringList of images in the path as well as the id of the current file
// and opens the file. If labels exist, the labels are opened
void InstaDam::openFile_and_labels() {
#ifdef WASM_BUILD
    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromByteArray(imageFileContent, labelNameTemp);
#else
    // Open labels
    generateLabelFileName();
    if (QFileInfo(this->annotationPath).isFile()) {
        QTextStream(stdout) << "Loading labels\n" << this->annotationPath << endl;
        QTextStream(stdout) << "\n" << this->file.baseName() << endl;
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
    QTextStream(stdout) << "Loading photoX" << endl;;

    SelectItem::myBounds = ui->IdmPhotoViewer->setPhotoFromFile(filename);
    qInfo("my bounds set");
#endif
    ui->IdmMaskViewer->LinkToPhotoViewer(ui->IdmPhotoViewer);

    qInfo("photo viewer linked!");
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
    scene->inactiveAll();
    mainUndoStack->clear();
    tempUndoStack->clear();
    undoGroup->setActiveStack(mainUndoStack);
    scene->update();
    maskScene->update();
}

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

    if (read(loadDoc.object(), fileType)) {
        setLabels();
        scene->clearItems();
        maskScene->clearItems();
        return true;
    } else {
        return false;
    }
}

void InstaDam::panButton_clicked() {
    panning = !panning;  // !ui->panButton->isChecked();
    ui->panButton->setChecked(panning);
    ui->IdmPhotoViewer->setPanMode(panning);
    ui->IdmMaskViewer->setPanMode(panning);
}

void InstaDam::on_rectangleSelectButton_clicked() {
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
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
    ui->rectangleSelectButton->setChecked(true);
    blankForm->blankText->setPlainText(rectBaseString);
    blankWidget->show();
    currentSelectType = SelectItem::Rectangle;
    scene->update();
    maskScene->update();
}

void InstaDam::on_ellipseSelectButton_clicked() {
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
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
    ui->ellipseSelectButton->setChecked(true);
    blankForm->blankText->setPlainText(ellipseBaseString);
    blankWidget->show();
    currentSelectType = SelectItem::Ellipse;
    scene->update();
    maskScene->update();
}

void InstaDam::on_polygonSelectButton_clicked() {
    scene->inactiveAll();
    maskScene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
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
    ui->polygonSelectButton->setChecked(true);
    polygonSelectWidget->show();
    polygonSelectForm->polygonMessageBox->setPlainText(polygonBaseInstruction);
    currentSelectType = SelectItem::Polygon;
    scene->update();
}

void InstaDam::on_freeSelectButton_clicked() {
    scene->inactiveAll();
    cancelCurrentSelection();
    currentItem = nullptr;
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
    ui->freeSelectButton->setChecked(true);
    freeSelectWidget->show();
    currentSelectType = SelectItem::Freedraw;
    scene->update();
    maskScene->update();
}

void InstaDam::roundBrushButtonClicked() {
    brushMode = Qt::RoundCap;
    freeSelectForm->squareBrushButton->setChecked(false);
    freeSelectForm->roundBrushButton->setChecked(true);
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
    ui->IdmMaskViewer->setBrushMode(Qt::RoundCap);
}

void InstaDam::squareBrushButtonClicked() {
    brushMode = Qt::SquareCap;
    freeSelectForm->squareBrushButton->setChecked(true);
    freeSelectForm->roundBrushButton->setChecked(false);
    ui->IdmPhotoViewer->setBrushMode(Qt::SquareCap);
    ui->IdmMaskViewer->setBrushMode(Qt::SquareCap);
}

void InstaDam::on_pushButton_14_clicked() {
    if (currentProject== nullptr) {
        assertError("Please create or open a project first. Projects define the label classes and the color to annotate them. You can open or create a project from the Project menu.");
    } else {
        filterDialog* dialogs = new filterDialog(ui->IdmPhotoViewer->selectedMask,
                                                 filterControl, ui->IdmPhotoViewer,
                                                 currentProject);
        dialogs->show();
    }
}

void InstaDam::setInsert() {
    insertVertex = true;
    vertex1 = -1;
    vertex2 = -1;
    polygonSelectForm->polygonMessageBox->setPlainText("Click the vertices between which you want to insert a point.");
}

void InstaDam::on_actionExport_triggered() {
    exportImages();
}

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

void InstaDam::processPointClicked(PhotoScene::viewerTypes type,
                                   SelectItem *item, QPointF pos,
                                   const Qt::MouseButton button) {
    currentButton = button;
    if (!item) {
        if (!canDrawOnPhoto && (!currentItem || currentItem->type() !=
                                SelectItem::Polygon)) {
            scene->inactiveAll();
            maskScene->inactiveAll();
            scene->update();
            maskScene->update();
            return;
        }
        if (type == PhotoScene::MASK_VIEWER_TYPE && currentSelectType !=
            SelectItem::Freedraw) {
            canDrawOnPhoto = false;
            undoGroup->setActiveStack(tempUndoStack);
            ui->addSelectionButton->setEnabled(true);
            ui->cancelSelectionButton->setEnabled(true);
        }

        if (!currentLabel || button == Qt::RightButton) {
            return;
        }

        if (currentItem && currentItem->type() == SelectItem::Polygon) {
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
            return;
        }
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
    } else {
        if (item->type() != currentSelectType) {
            if (!canDrawOnPhoto)
                return;
            switch (item->type()) {
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
        if (!canDrawOnPhoto)
            maskItem = currentItem;
        currentLabel = currentItem->getLabel();
        scene->inactiveAll();
        maskScene->inactiveAll();
        currentItem->clickPoint(pos);
        currentItem->setItemActive();
        if (currentItem->type() == SelectItem::Polygon && insertVertex) {
            int vert = currentItem->getActiveVertex();
            if (vert != SelectItem::UNSELECTED) {
                if (vertex1 <0) {
                    vertex1 = vert;
                    polygonSelectForm->polygonMessageBox->appendPlainText("First vertex selected.");
                } else if (vertex2 < 0) {
                    if (abs(vert - vertex1) != 1 && abs(vert - vertex1) !=
                        (currentItem->numberOfVertices() - 1)) {
                        polygonSelectForm->polygonMessageBox->appendPlainText("Invalid second vertex, it must be adjacent to the first vertex.");
                    } else {
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

void InstaDam::processMouseMoved(QPointF fromPos, QPointF toPos) {
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

void InstaDam::processMouseReleased(PhotoScene::viewerTypes type,
                                    QPointF oldPos, QPointF newPos,
                                    const Qt::MouseButton button) {
    UNUSED(button);
    if (currentItem && !currentItem->isItemAdded()) {
        if (currentItem->type() == SelectItem::Freedraw && type ==
            PhotoScene::MASK_VIEWER_TYPE) {
            FreeDrawSelect fds(maskSelection(currentItem), currentItem->myPen);
            FreeDrawSelect *temp = dynamic_cast<FreeDrawSelect*>(currentItem);
            temp->setPointsUnchecked(fds.getPixmap());
        }
        if (currentItem->type() == SelectItem::Freeerase) {
            QUndoCommand *eraseCommand = new ErasePointsCommand(myErase, scene,
                                                                maskScene);
            undoGroup->activeStack()->push(eraseCommand);
        } else {
            QUndoCommand *addCommand = new AddCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(),
                                                      scene);
            undoGroup->activeStack()->push(addCommand);
            if (type == PhotoScene::MASK_VIEWER_TYPE)
                currentItem->setFromMaskScene(true);
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
            QUndoCommand *moveCommand = new MoveCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(),
                                                        oldPos, newPos);
            undoGroup->activeStack()->push(moveCommand);
        } else if (currentItem->wasResized()) {
            QUndoCommand *resizeCommand = new MoveVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(),
                                                                oldPos, newPos, currentItem->getActiveVertex());
            undoGroup->activeStack()->push(resizeCommand);
        } else {
            QUndoCommand *rotateCommand = new RotateCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(),
                                                            oldPos, newPos);
            undoGroup->activeStack()->push(rotateCommand);
        }
        currentItem->resetState();
    } else if (currentItem && currentItem->type() == SelectItem::Polygon && !currentItem->wasPointAdded()) {
        QUndoCommand *addVertexCommand = new AddVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(),
                                                              newPos);
        undoGroup->activeStack()->push(addVertexCommand);
        currentItem->setActiveVertex(SelectItem::UNSELECTED);
        currentItem->resetState();
    }
    currentButton = Qt::NoButton;
    scene->update();
    maskScene->update();
}

void InstaDam::finishPolygonButtonClicked() {
    if (currentItem)
        currentItem->setActiveVertex(SelectItem::UNSELECTED);
    currentItem = nullptr;
    scene->update();
    maskScene->update();
}

void InstaDam::processKeyPressed(PhotoScene::viewerTypes type, const int key) {
    if (!currentItem) {
        return;
    } else if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
        if (currentItem->type() == SelectItem::Polygon &&
            currentItem->getActiveVertex() != SelectItem::UNSELECTED) {
            QUndoCommand *deleteVertexCommand = new DeleteVertexCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror());
            undoGroup->activeStack()->push(deleteVertexCommand);
        } else {
            QUndoCommand *deleteCommand = new DeleteCommand((type == PhotoScene::PHOTO_VIEWER_TYPE) ? currentItem : currentItem->getMirror(),
                                                            scene);
            undoGroup->activeStack()->push(deleteCommand);
        }
    } else if (key == Qt::Key_X || key == Qt::Key_X + Qt::Key_Shift) {
        currentItem = nullptr;
    }
}

bool InstaDam::read(const QJsonObject &json, fileTypes type) {
    if (json.contains("labels") && json["labels"].isArray()) {
        QJsonArray labelArray = json["labels"].toArray();
        tempLabels.clear();
        tempLabels.reserve(labelArray.size());
        for (int i = 0; i < labelArray.size(); i++) {
            QSharedPointer<Label> label = QSharedPointer<Label>::create(labelArray[i].toObject(), i);
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
                connect(projectDialog->loadNewButton, SIGNAL(clicked()), dialog,
                        SLOT(reject()));
                dialog->exec();
                if (dialog->result() == QDialog::Accepted) {
                    return false;
                }
            }
            currentProject->setLabels(tempLabels);
            QTextStream(stdout) << currentProject->numLabels();
        }
    }
    return true;
}

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
    json["labels"] = labs;
}

void InstaDam::addCurrentSelection() {
    tempUndoStack->clear();
    undoGroup->setActiveStack(mainUndoStack);
    FreeDrawSelect *fds = new FreeDrawSelect(maskSelection(maskItem),
                                             currentLabel);
    scene->addItem(fds);
    QUndoCommand *addCommand = new AddCommand(fds, scene);
    mainUndoStack->push(addCommand);
    scene->removeItem(maskItem->getMirror());
    maskScene->removeItem(maskItem->getMirror());
    scene->removeItem(maskItem);
    maskScene->removeItem(maskItem);
    maskItem = nullptr;
    ui->addSelectionButton->setDisabled(true);
    ui->cancelSelectionButton->setDisabled(true);
    canDrawOnPhoto = true;
}

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
    map = joinPixmaps(ui->IdmMaskViewer->photo->pixmap(), map, QPainter::CompositionMode_DestinationIn);
    return map;
}

void InstaDam::on_addSelectionButton_clicked() {
}

void InstaDam::setCurrentProject(Project* pr) {
    this->currentProject = pr;
}

QStringList InstaDam::getLabelNames(QVector<QSharedPointer<Label> > labels) {
    QStringList labelList;
    QVectorIterator<QSharedPointer<Label> > it(labels);
    while (it.hasNext()) {
        labelList.append(it.next()->getText());
    }
    return labelList;
}

void InstaDam::revert() {
    this->filename = this->oldFilename;
    this->file = this->oldFile;
    this->path = this->oldPath;
    this->imagesList = this->oldImagesList;
    this->annotationPath = this->oldAnnotationPath;
    this->labelPaths = this->oldLabelPaths;
}

void InstaDam::processShowHide(int state) {
    for (int i = 0; i < currentProject->numLabels(); i++) {
         currentProject->getLabel(i)->setMaskState(state);
    }
}
