#ifndef INSTADAM_H
#define INSTADAM_H

#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "project.h"
#include <QDialog>
#include <QGraphicsItem>
#include <QObject>
#include <QGridLayout>
#include <QMenuBar>
#include <QMenu>
#include <QBuffer>

#include "newproject.h"
#include "ui_blankFrame.h"
#include "ui_freeSelect.h"
#include "ui_polygonSelect.h"
#include "../Selector/label.h"
#include "labelButton.h"

#include <iostream>
#include <string>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/imgproc.hpp"

class filterControls;

enum maskTypes{CANNY, THRESHOLD, BLUR, OTHER};
enum threshold_or_filter{THRESH, FILTER};
#include <QUndoStack>

#include "Selector/photoScene.h"


namespace Ui {
class InstaDam;
}

class InstaDam : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstaDam(QWidget *parent = nullptr);
    ~InstaDam();
    void fileOpen();
    void connectFilters();
    filterControls * filterControl;

    int fileId= 0;
    QFileInfo file;
    QString filename;
    QString labelFile;
    QStringList imagesList;
    QDir path;
    void openFile_and_labels();
    void generateLabelFileName();
    void assertError(std::string errorMessage);
    void exportImages(bool asBuffers = false);
    void clearLayout(QLayout * layout);

private slots:
    void on_actionOpen_File_triggered();
    void on_rectangleSelectButton_clicked();
    void on_ellipseSelectButton_clicked();
    void on_polygonSelectButton_clicked();
    void on_freeSelectButton_clicked();
    void processMouseMoved(QPointF fromPos, QPointF toPos);
    void processPointClicked(PhotoScene::viewerTypes type, SelectItem *item, QPointF pos, const Qt::MouseButton button);
    void processMouseReleased(PhotoScene::viewerTypes type, QPointF oldPos, QPointF newPos, const Qt::MouseButton button);
    void processKeyPressed(PhotoScene::viewerTypes type, const int key);
    void finishPolygonButtonClicked();
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void panButton_clicked();

    void roundBrushButtonClicked();

    void squareBrushButtonClicked();

    void on_pushButton_14_clicked();

    void on_actionExport_triggered();
    void on_actionExport_zip_triggered();
    void on_saveAndNext_clicked();
    void setInsert();
    void toggleDrawing();
    void toggleErasing();
    void setCurrentLabel(QSharedPointer<Label> label);
    void setCurrentLabel(LabelButton *button);
    void setCurrentBrushSize(int);
    void setNewProject();
public slots:
    void resetPixmapButtons();


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
    QSharedPointer<Label> currentLabel;
    QVector<LabelButton*> labelButtons;
    Ui::InstaDam *ui;
    newproject *newProject;
    Project currentProject;
    PhotoScene *scene;
    PhotoScene *maskScene;
    QUndoStack *undoStack;
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
    bool drawing = true;
    bool panning = false;
    int lastType = -1;
    bool insertVertex = false;
    int vertex1 = -1;
    int vertex2 = -1;
    int currentBrushSize = 5;
    FreeDrawErase *myErase;
    SelectItem *tempItem;
    SelectItem *mirrorItem;
    int brushMode = Qt::SquareCap;
    Qt::MouseButton currentButton = Qt::NoButton;
    QHash<QString, QBuffer*> exportFiles;

    void read(const QJsonObject &json);
    void write(QJsonObject &json);
    void loadLabelFile(QString filename);
};




#endif // INSTADAM_H
