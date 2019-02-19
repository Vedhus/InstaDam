#include <QtWidgets>
#include <iostream>
using namespace std;

#include "mainwindow.h"
#include "photoScene.h"
#include "selectItem.h"
#include "ellipseSelect.h"
#include "rectangleSelect.h"
#include "commands.h"

MainWindow::MainWindow()
{
    undoStack = new QUndoStack(this);
    type = Ellipse;
    createActions();
    createMenus();

    //createUndoView();

    diagramScene = new PhotoScene();
    QPixmap mPix = QPixmap("/home/friedel/Pictures/Wallpapers/Space-Background-1200x1920.jpg");
    //QRectF mRect = QRectF(0,0,mPix.width(),mPix.height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(mPix);
    background->setZValue(-100);
    //diagramScene->addPixmap(mPix);
    diagramScene->addItem(background);
    //connect(diagramScene, SIGNAL(itemMoved(DiagramItem*,QPointF)),
    //        this, SLOT(itemMoved(DiagramItem*,QPointF)));
    connect(diagramScene, SIGNAL(pointClicked(QPointF)), this,
            SLOT(newItem(QPointF)));
    connect(diagramScene, SIGNAL(itemSelected(SelectItem*, QPointF)), this,
            SLOT(selected(SelectItem*, QPointF)));
    connect(diagramScene, SIGNAL(movedPoint(QPointF)), this,
            SLOT(movePoint(QPointF)));
    connect(diagramScene, SIGNAL(addNewItem(QPointF, QPointF)), this,
            SLOT(addItem(QPointF, QPointF)));
    connect(diagramScene, SIGNAL(doRefresh()), this,
            SLOT(myupdate()));
    connect(diagramScene, SIGNAL(deleteObject(SelectItem*)), this,
            SLOT(deleteItem(SelectItem*)));
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

void MainWindow::newItem(QPointF pos){
    //cout << "NEW" << endl;
    switch(type){
        case Rect:
           {
            //cout << "RECT" << endl;
            RectangleSelect *temp = new RectangleSelect(pos);
            diagramScene->addItem(temp);
            item = temp;
            diagramScene->update();
            //cout << "UPDATE" << endl;
            }
            break;
        case Ellipse:
            {
            //cout << "RECT" << endl;
            EllipseSelect *temp = new EllipseSelect(pos);
            diagramScene->addItem(temp);
            item = temp;
            diagramScene->update();
            //cout << "UPDATE" << endl;
            }
            break;
        case Generic:
            break;
        case Polygon:
            break;
        //default:
        //    break;
    //case Generic:
    //case Polygon:


    }
}

void MainWindow::movePoint(QPointF pos){
    //cout << "  MP" << endl;
    item->addPoint(pos);
    diagramScene->update();
}

void MainWindow::addItem(QPointF oldPos, QPointF newPos)
{
    //cout << "ADD ITEM" << endl;
    if(item){
        //cout << "C1" << endl;
        QUndoCommand *addCommand = new AddCommand(item, diagramScene);
        undoStack->push(addCommand);
        item = nullptr;
    }
    else if(selectedItem){
        if(selectedItem->wasMoved()){
            //cout << "MC " << oldPos.x() << "," << oldPos.y() << "    " << newPos.x() << "," << newPos.y() <<endl;
            QUndoCommand *moveCommand = new MoveCommand(selectedItem, oldPos, newPos);
            undoStack->push(moveCommand);
        }
        else{
            //cout << "RMS" << endl;
            QUndoCommand *resizeCommand = new ResizeCommand(selectedItem, oldPos, newPos, selectedItem->getActiveCorner());
            undoStack->push(resizeCommand);
        }
        selectedItem->resetState();
    }
}

void MainWindow::deleteItem(SelectItem *item){
    QUndoCommand *deleteCommand = new DeleteCommand(item, diagramScene);
    undoStack->push(deleteCommand);

}
void MainWindow::selected(SelectItem* selected, QPointF pos){
    if(item){
        diagramScene->removeItem(item);
        item = nullptr;
    }
    selectedItem = selected;
    selected->clickPoint(pos);
    diagramScene->update();
}

void MainWindow::myupdate(){
    diagramScene->update();
}
//void MainWindow::drawBox(QRectF rect){
//    cout << "DRAW BOX" << endl;
    //QUndoCommand *addCommand = new AddCommand(rect, diagramScene);
    //undoStack->push(addCommand);
//}
