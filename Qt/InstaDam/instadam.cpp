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
    item = nullptr;
    connect(scene, SIGNAL(pointClicked(QPointF)), this,
            SLOT(newItem(QPointF)));
    connect(scene, SIGNAL(itemSelected(SelectItem*, QPointF)), this,
            SLOT(selected(SelectItem*, QPointF)));
    connect(scene, SIGNAL(movedPoint(QPointF)), this,
            SLOT(movePoint(QPointF)));
    connect(scene, SIGNAL(addNewItem(QPointF, QPointF)), this,
            SLOT(addItem(QPointF, QPointF)));
    connect(scene, SIGNAL(doRefresh()), this,
            SLOT(myupdate()));
    connect(scene, SIGNAL(deleteObject(SelectItem*)), this,
            SLOT(deleteItem(SelectItem*)));
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

void InstaDam::on_freeDrawButton_clicked(){
    type = Free;
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

void InstaDam::newItem(QPointF pos){
    //cout << "NEW" << endl;
    if(item){
        if(item->type() == Polygon){
            item->addPoint(pos);
            scene->update();
            return;
        }
    }
    switch(type){
        case Rect:
           {
            //cout << "RECT" << endl;
            RectangleSelect *temp = new RectangleSelect(pos);
            scene->addItem(temp);
            item = temp;
            scene->update();
            //cout << "UPDATE" << endl;
            }
            break;
        case Ellipse:
            {
            //cout << "RECT" << endl;
            EllipseSelect *temp = new EllipseSelect(pos);
            scene->addItem(temp);
            item = temp;
            scene->update();
            //cout << "UPDATE" << endl;
            }
            break;
        case Generic:
            break;
        case Polygon:
            {
            PolygonSelect *temp = new PolygonSelect(pos);
            scene->addItem(temp);
            item = temp;
            scene->update();
            }
        break;
        case Free:
            break;
    }
}

void InstaDam::movePoint(QPointF pos){
    //cout << "  MP" << endl;
    item->addPoint(pos);
    scene->update();
}

void InstaDam::addItem(QPointF oldPos, QPointF newPos)
{
    //cout << "ADD ITEM" << endl;
    if(item){
        if(item->type() != Polygon){
            //cout << "C1" << endl;
            QUndoCommand *addCommand = new AddCommand(item, scene);
            undoStack->push(addCommand);
            item = nullptr;
        }
    }
    else if(selectedItem){
        if(selectedItem->wasMoved()){
            //cout << "MC " << oldPos.x() << "," << oldPos.y() << "    " << newPos.x() << "," << newPos.y() <<endl;
            QUndoCommand *moveCommand = new MoveCommand(selectedItem, oldPos, newPos);
            undoStack->push(moveCommand);
        }
        else{
            //cout << "RMS" << endl;
            QUndoCommand *moveVertexCommand = new MoveVertexCommand(selectedItem, oldPos, newPos, selectedItem->getActiveVertex());
            undoStack->push(moveVertexCommand);
        }
        selectedItem->resetState();
    }
}

void InstaDam::deleteItem(SelectItem *item){
    QUndoCommand *deleteCommand = new DeleteCommand(item, scene);
    undoStack->push(deleteCommand);

}
void InstaDam::selected(SelectItem* selected, QPointF pos){
    if(item){
        scene->removeItem(item);
        item = nullptr;
    }
    selectedItem = selected;
    selected->clickPoint(pos);
    scene->update();
}

void InstaDam::myupdate(){
    scene->update();
}
