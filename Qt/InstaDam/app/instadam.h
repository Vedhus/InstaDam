#ifndef INSTADAM_H
#define INSTADAM_H

#include <stdio.h>

#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDialog>
#include <QGraphicsItem>
#include <QUndoGroup>
#include <QObject>
#include <QGridLayout>
#include <QMenuBar>
#include <QMenu>
#include <QBuffer>
#include <QUndoStack>

#include <QWidget>
#include <QtNetwork/QNetworkReply>

#include <iostream>
#include <string>
#include "imagelist.h"
#include "newproject.h"
#include "picpushbutton.h"
#include "ui_blankFrame.h"
#include "ui_freeSelect.h"
#include "ui_polygonSelect.h"
#include "../Selector/label.h"
#include "labelButton.h"
#include "enumconstants.h"
#include "project.h"
#include "Selector/photoScene.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "serverprojectname.h"
#include "ui_serverprojectname.h"

class filterControls;
class PicPushButton;

namespace Ui {
class InstaDam;
}

class InstaDam : public QMainWindow {
    Q_OBJECT

 public:
    explicit InstaDam(QWidget *parent = nullptr, QString databseURL ="", QString token = "");
    ~InstaDam();
    void connectFilters();
    filterControls * filterControl;

    int fileId = 0;
    QFileInfo file, oldFile;
    QString filename, oldFilename;
    QString labelFile;
    bool runningLocally = false;
    int labelIdsRecieved = 0;
    QVector<QString> labelPaths, oldLabelPaths;
    QString annotationPath, oldAnnotationPath;
    QStringList imagesList, oldImagesList;
    QDir path, oldPath;
    void openFile_and_labels();
    void setLabels();
    void selectItemButton(SelectItem::SelectType);
    void selectItemButton(int  type);
    void generateLabelFileName();
    void assertError(std::string errorMessage);
    void exportImages(bool asBuffers = false);
    void clearLayout(QLayout * layout);
    void setCurrentProject(Project*);
    void setCurrentProjectId(int id);
    void setButtonsConfiguration();
    void setCurrentItem(SelectItem *item, bool enable = false) {
        currentItem = item;
        polygonSelectForm->finishPolygonButton->setEnabled(enable);
    }
    QList<EnumConstants::maskTypes> maskTypeList;
    QList<PicPushButton*> maskButtonList;
    ImageList* il;
    void resetGUIclearLabels();
    void saveAndProgress(int);
    PhotoScene::viewerTypes selectedViewer;
    void deleteCurrentObject(PhotoScene::viewerTypes type);

 private slots:
    void on_addSelectionButton_clicked();
    void on_saveAndBack_clicked();
    void on_actionSave_Annotation_triggered();
    void on_actionOpen_File_triggered();
    void on_rectangleSelectButton_clicked();
    void on_ellipseSelectButton_clicked();
    void on_polygonSelectButton_clicked();
    void on_freeSelectButton_clicked();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExport_triggered();
    void on_actionExport_zip_triggered();
    void on_saveAndNext_clicked();

    void setInsert();
    void toggleDrawing();
    void toggleErasing();
    void checkLabel(QSharedPointer<Label> label);
    void setCurrentLabel(QSharedPointer<Label> label);
    void setCurrentLabel(LabelButton *button);
    void setOpacity(QSharedPointer<Label>, int val);
    void setCurrentBrushSize(int);
    void setNewProject();
    void addCurrentSelection(bool useCurrent = false);
    void cancelCurrentSelection();
    void finishPolygonButtonClicked();
    void panButton_clicked();
    void roundBrushButtonClicked();
    void squareBrushButtonClicked();
    void populateSceneFromProjectLabels();

    void processMouseMoved(QPointF fromPos, QPointF toPos,
                           const Qt::KeyboardModifiers modifiers);
    void processPointClicked(PhotoScene::viewerTypes type, SelectItem *item,
                             QPointF pos, const Qt::MouseButton button,
                             const Qt::KeyboardModifiers modifiers);
    void processMouseReleased(PhotoScene::viewerTypes type, QPointF oldPos,
                              QPointF newPos, const Qt::MouseButton button,
                              const Qt::KeyboardModifiers modifiers);
    void processKeyPressed(PhotoScene::viewerTypes type, const int key);
    void processShowHide(int state);

    void on_filterOptions_clicked();

    void on_actionSearch_triggered();
    void on_actionUpdate_Privilege_triggered();
    void saveToServer();
    void replyFinished();
    void labelReplyFinished();


#ifdef TEST
    friend class IntegrationTest;
#endif
    void on_actionDelete_2_triggered();

    void on_actionDelete_triggered();

public slots:
    void resetPixmapButtons();
    void fileDownloaded(QString path);
    bool loadLabelJson(QJsonObject json, fileTypes fileType);

 private:
#ifdef WASM_BUILD
    struct MyConnector{
            std::function<void(void)> onActivate;
    };
    void loadRawImage();
    void addImageConnector(QString text, std::function<void(void)> onActivate);
    void addIdproConnector(QString text, std::function<void(void)> onActivate);
    MyConnector *openImageConnector;
    MyConnector *openIdproConnector;
    QByteArray imageFileContent;
    QByteArray idproFileContent;
#endif

#ifdef TEST
    bool assertThrown = false;
    QHash<QString, QColor> labelHash;
    void addLabelHash(QString text, QColor color){
        labelHash.insert(text, color);
    }

    QString prjInName = "";
    QString prjOutName = "";
    QString imgInName = "";

    void setPrjInName(QString name) {prjInName = name;}
    void setPrjOutName(QString name) {prjOutName = name;}
    void setImgInName(QString name) {imgInName = name;}
#endif
    QSharedPointer<Label> currentLabel;
    QVector<LabelButton*> labelButtons;
    Ui::InstaDam *ui;
    newproject *newProject;
    Project *currentProject = nullptr;
    PhotoScene *scene;
    PhotoScene *maskScene;
    QUndoStack *mainUndoStack;
    QUndoStack *tempUndoStack;
    QUndoGroup *undoGroup;
    SelectItem::SelectType currentSelectType;
    SelectItem *currentItem;
    QAction *undoAction;
    QAction *redoAction;
    QByteArray imgData;
    QString idproFileName;
    QWidget *blankWidget;
    QWidget *freeSelectWidget;
    QWidget *polygonSelectWidget;
    QGridLayout *controlLayout;
    Ui::blankForm *blankForm;
    Ui::freeSelectForm *freeSelectForm;
    Ui::polygonSelectForm *polygonSelectForm;
    //PhotoScene::viewerTypes currentViewer;
    bool drawing = true;
    bool panning = false;
    bool ctrlPanning = false;
    bool canDrawOnPhoto = true;
    int lastType = -1;
    bool insertVertex = false;
    int vertex1 = -1;
    int vertex2 = -1;
    int currentBrushSize = 5;
    FreeDrawErase *myErase;
    SelectItem *tempItem;
    SelectItem *mirrorItem;
    SelectItem *maskItem;

    serverProjectName *sProjectName;
    QString accessToken;
    QString databaseURL;
    QString projecListUseCase;
    QNetworkReply *rep;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    Qt::PenCapStyle brushMode = Qt::RoundCap;
    Qt::MouseButton currentButton = Qt::NoButton;
    QHash<QString, QBuffer*> exportFiles;
    QVector<QSharedPointer<Label> > tempLabels;

    QPixmap maskSelection(SelectItem *item);
    void imagesReplyFinished();
    void saveAnnotationReplyFinished();
    bool read(const QJsonObject &json, fileTypes type = PROJECT);
    void write(QJsonObject &json, fileTypes type = PROJECT);

    QStringList getLabelNames(QVector<QSharedPointer<Label> > labels);
    bool loadLabelFile(QString filename, fileTypes fileType);
    void revert();
    void listProjects();
    void projectsReplyFinished();
    void fileReplyFinished();
    void openFileFromJson(QJsonObject);
    void getReadyForNewProject();
};

#endif  // INSTADAM_H
