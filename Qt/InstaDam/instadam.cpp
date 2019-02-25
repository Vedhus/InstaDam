#include "instadam.h"
#include "ui_instadam.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <iostream>
#include <string>
#include <stdio.h>

#include "Selector/selectItem.h"
#include "Selector/ellipseSelect.h"
#include "Selector/rectangleSelect.h"
#include "Selector/commands.h"


InstaDam::InstaDam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstaDam)
{
    ui->setupUi(this);
    undoStack = new QUndoStack(this);
    type = Ellipse;
    scene = ui->IdmPhotoViewer->scene;
    currentItem = nullptr;
    connect(scene, SIGNAL(pointClicked(SelectItem*, QPointF)), this,
            SLOT(processPointClicked(SelectItem*, QPointF)));
    connect(scene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(scene, SIGNAL(leftMouseReleased(QPointF, QPointF)), this,
            SLOT(processLeftMouseReleased(QPointF, QPointF)));
    connect(scene, SIGNAL(keyPressed(const int)), this,
            SLOT(processKeyPressed(const int)));
    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui->menuEdit->addAction(undoAction);
    ui->menuEdit->addAction(redoAction);
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

void InstaDam::on_panButton_clicked()
{
    ui->IdmPhotoViewer->setPanMode();
}

void InstaDam::on_boxButton_clicked(){
    type = Rect;
}

void InstaDam::on_ellipseButton_clicked(){
    type = Ellipse;
}

void InstaDam::on_polygonButton_clicked(){
    type = Polygon;
}

void InstaDam::on_roundBrush_clicked()
{
    qInfo("Round");
    ui->IdmPhotoViewer->setBrushMode(Qt::RoundCap);
}

void InstaDam::on_squareBrush_clicked()
{
    ui->IdmPhotoViewer->setBrushMode(Qt::SquareCap);
}

void InstaDam::processPointClicked(SelectItem *item, QPointF pos){
    //cout << "NEW" << endl;
    if(!item){
        if(currentItem && currentItem->type() == Polygon){
            currentItem->addPoint(pos);
            scene->update();
            return;
        }
        scene->inactiveAll();
        switch(type){
            case Rect:
            {
                //cout << "RECT" << endl;
                RectangleSelect *temp = new RectangleSelect(pos);
                scene->addItem(temp);
                currentItem = temp;
                scene->update();
                //cout << "UPDATE" << endl;
            }
                break;
            case Ellipse:
            {
                //cout << "RECT" << endl;
                EllipseSelect *temp = new EllipseSelect(pos);
                scene->addItem(temp);
                currentItem = temp;
                scene->update();
                //cout << "UPDATE" << endl;
            }
                break;
            case Freedraw:
            {
                //FreeDrawSelect *temp = new FreeDrawSelect(pos);
                //diagramScene->addItem(temp);
                //currentItem = temp;
                //diagramScene->update();
            }
                break;
            case Polygon:
            {
                PolygonSelect *temp = new PolygonSelect(pos);
                scene->addItem(temp);
                currentItem = temp;
                scene->update();
            }
                break;
        //default:
        //    break;
    //case Generic:


        }
    }
    else{
        currentItem = item;
        scene->inactiveAll();
        currentItem->clickPoint(pos);
        scene->update();
    }
}

void InstaDam::processMouseMoved(QPointF fromPos, QPointF toPos){
    if(currentItem){
        currentItem->moveItem(fromPos, toPos);
        scene->update();
    }
}

void InstaDam::processLeftMouseReleased(QPointF oldPos, QPointF newPos)
{
    if(currentItem && !currentItem->isItemAdded()){
        QUndoCommand *addCommand = new AddCommand(currentItem, scene);
        undoStack->push(addCommand);
        if(currentItem->type() != Polygon){
            currentItem = nullptr;
        }
        else{
            currentItem->setActiveVertex(UNSELECTED);
        }
    }
    else if(currentItem && (currentItem->wasMoved() || currentItem->wasResized())){
        if(currentItem->wasMoved()){
            QUndoCommand *moveCommand = new MoveCommand(currentItem, oldPos, newPos);
            undoStack->push(moveCommand);
        }
        else{
            QUndoCommand *resizeCommand = new MoveVertexCommand(currentItem, oldPos, newPos, currentItem->getActiveVertex());
            undoStack->push(resizeCommand);
        }
        currentItem->resetState();
    }
    else if(currentItem && currentItem->type() == Polygon && !currentItem->wasPointAdded()){
        QUndoCommand *addVertexCommand = new AddVertexCommand(currentItem, newPos);
        undoStack->push(addVertexCommand);
        currentItem->setActiveVertex(UNSELECTED);
    }
}

void InstaDam::processKeyPressed(const int key){
    if(!currentItem){

    }
    else if(key == Qt::Key_Delete || key == Qt::Key_Backspace){
        if(currentItem->type() == Polygon && currentItem->getActiveVertex() != UNSELECTED){
            QUndoCommand *deleteVertexCommand = new DeleteVertexCommand(currentItem);
            undoStack->push(deleteVertexCommand);
        }
        else{
            QUndoCommand *deleteCommand = new DeleteCommand(currentItem, scene);
            undoStack->push(deleteCommand);
        }
    }
    else if(key == Qt::Key_X || key == Qt::Key_X + Qt::Key_Shift){
        currentItem = nullptr;
    }

}
