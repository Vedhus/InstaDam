#include <QtWidgets>
#include <iostream>
using namespace std;

#include "mainwindow.h"
#include "photoScene.h"
#include "selectItem.h"
#include "ellipseSelect.h"
#include "rectangleSelect.h"
#include "polygonSelect.h"
#include "commands.h"

MainWindow::MainWindow()
{
    undoStack = new QUndoStack(this);
    type = Polygon;
    createActions();
    createMenus();
    currentItem = nullptr;

    diagramScene = new PhotoScene();
    QPixmap mPix = QPixmap("/home/friedel/Pictures/Wallpapers/Space-Background-1200x1920.jpg");

    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(mPix);
    background->setZValue(-100);

    diagramScene->addItem(background);

    connect(diagramScene, SIGNAL(pointClicked(SelectItem*, QPointF)), this,
            SLOT(processPointClicked(SelectItem*, QPointF)));
    connect(diagramScene, SIGNAL(mouseMoved(QPointF, QPointF)), this,
            SLOT(processMouseMoved(QPointF, QPointF)));
    connect(diagramScene, SIGNAL(leftMouseReleased(QPointF, QPointF)), this,
            SLOT(processLeftMouseReleased(QPointF, QPointF)));
    connect(diagramScene, SIGNAL(keyPressed(const int)), this,
            SLOT(processKeyPressed(const int)));
    setWindowTitle("Undo Framework");
    QGraphicsView *view = new QGraphicsView(diagramScene);
    setCentralWidget(view);
}

void MainWindow::createActions()
{
    deleteAction = new QAction(tr("&Delete Item"), this);
    deleteAction->setShortcut(tr("Del"));
    //connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    addBoxAction = new QAction(tr("Add &Box"), this);

    addBoxAction->setShortcut(tr("Ctrl+O"));
    //connect(addBoxAction, SIGNAL(triggered()), this, SLOT(addBox()));

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    //editMenu->addAction(deleteAction);
    //connect(editMenu, SIGNAL(aboutToShow()),
    //        this, SLOT(itemMenuAboutToShow()));
    //connect(editMenu, SIGNAL(aboutToHide()),
    //        this, SLOT(itemMenuAboutToHide()));

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(addBoxAction);
}

void MainWindow::processPointClicked(SelectItem *item, QPointF pos){
    //cout << "NEW" << endl;
    if(!item){
        if(currentItem && currentItem->type() == Polygon){
            currentItem->addPoint(pos);
            diagramScene->update();
            return;
        }
        diagramScene->inactiveAll();
        switch(type){
            case Rect:
            {
                //cout << "RECT" << endl;
                RectangleSelect *temp = new RectangleSelect(pos);
                diagramScene->addItem(temp);
                currentItem = temp;
                diagramScene->update();
                //cout << "UPDATE" << endl;
            }
                break;
            case Ellipse:
            {
                //cout << "RECT" << endl;
                EllipseSelect *temp = new EllipseSelect(pos);
                diagramScene->addItem(temp);
                currentItem = temp;
                diagramScene->update();
                //cout << "UPDATE" << endl;
            }
                break;
            case Generic:
                break;
            case Free:
                break;
            case Polygon:
            {
                PolygonSelect *temp = new PolygonSelect(pos);
                diagramScene->addItem(temp);
                currentItem = temp;
                diagramScene->update();
            }
                break;
        //default:
        //    break;
    //case Generic:


        }
    }
    else{
        currentItem = item;
        diagramScene->inactiveAll();
        currentItem->clickPoint(pos);
        diagramScene->update();
    }
}

void MainWindow::processMouseMoved(QPointF fromPos, QPointF toPos){
    if(currentItem){
        //if(currentItem->type() == Polygon){
        //    currentItem->addPoint(toPos);
        //}
        //else{
            currentItem->moveItem(fromPos, toPos);
        //}
        diagramScene->update();
    }
}

void MainWindow::processLeftMouseReleased(QPointF oldPos, QPointF newPos)
{
    //cout << "ADD ITEM" << endl;
    if(currentItem && !currentItem->isItemAdded()){
        //if(currentItem->type() != Polygon){
            //cout << "C1" << endl;
            QUndoCommand *addCommand = new AddCommand(currentItem, diagramScene);
            undoStack->push(addCommand);
            if(currentItem->type() != Polygon){
                currentItem = nullptr;
            }
            else{
                //cout << "C3" << endl;
                currentItem->setActiveVertex(UNSELECTED);
            }
        //}
        //else{
        //    cout << "C2" << endl;
        //    currentItem->setActiveVertex(UNSELECTED);
        //}
    }
    else if(currentItem && (currentItem->wasMoved() || currentItem->wasResized())){
        //cout << "C2" << endl;
        if(currentItem->wasMoved()){
            //cout << "MC" << endl;
            //cout << "MC " << oldPos.x() << "," << oldPos.y() << "    " << newPos.x() << "," << newPos.y() <<endl;
            QUndoCommand *moveCommand = new MoveCommand(currentItem, oldPos, newPos);
            undoStack->push(moveCommand);
        }
        else{
            //cout << "RMS" << endl;
            QUndoCommand *resizeCommand = new MoveVertexCommand(currentItem, oldPos, newPos, currentItem->getActiveVertex());
            undoStack->push(resizeCommand);
        }
        currentItem->resetState();
    }
    else if(currentItem && currentItem->type() == Polygon){
        //cout << "C2" << endl;
        QUndoCommand *addVertexCommand = new AddVertexCommand(currentItem, newPos);
        undoStack->push(addVertexCommand);
        currentItem->setActiveVertex(UNSELECTED);
    }
}

void MainWindow::processKeyPressed(const int key){
    //cout << key << "   " << Qt::Key_Delete << "  " << Qt::Key_Backspace <<endl;
    if(!currentItem){

    }
    else if(key == Qt::Key_Delete || key == Qt::Key_Backspace){
        //cout << "DEL " << (currentItem->type() == Polygon) << "  " << currentItem->getActiveVertex() << endl;
        if(currentItem->type() == Polygon && currentItem->getActiveVertex() != UNSELECTED){
            QUndoCommand *deleteVertexCommand = new DeleteVertexCommand(currentItem);
            undoStack->push(deleteVertexCommand);
        }
        else{
            //cout << "DD" << endl;
            QUndoCommand *deleteCommand = new DeleteCommand(currentItem, diagramScene);
            undoStack->push(deleteCommand);
        }
    }

}
