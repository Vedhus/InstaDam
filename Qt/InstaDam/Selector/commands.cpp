#include <QtWidgets>
#include <iostream>
using namespace std;

#include "commands.h"
#include "selectItem.h"

AddCommand::AddCommand(SelectItem *item,
                       PhotoScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    static int itemCount = 0;
    myScene = scene;
    myItem = item;

    scene->update();
    ++itemCount;
    
}


AddCommand::~AddCommand()
{
    if (!myItem->scene())
        delete myItem;
}


void AddCommand::undo()
{
    //cout << "UA" << endl;
    myItem->hide();
    myScene->update();
}

void AddCommand::redo()
{
    //cout << "RA" << endl;
    if(init){
        myItem->show();
        myScene->clearSelection();
        myScene->update();
    }
    else{
        myItem->itemWasAdded();
        init = true;
    }
}

DeleteCommand::DeleteCommand(SelectItem* item, PhotoScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myScene = scene;
    myItem = item;
}

void DeleteCommand::undo()
{
    //cout << "UD" << endl;
    //myScene->addItem(myItem);
    myItem->show();
    myScene->update();
}

void DeleteCommand::redo()
{
    //cout << "RD" << endl;
    //myScene->removeItem(myItem);
    //if(init){
        myItem->hide();
        myScene->update();
    //}
    //else{
    //    init = true;
    //}
}

MoveCommand::MoveCommand(SelectItem *item, const QPointF oldPos,
                         const QPointF newPos,
                         QUndoCommand *parent) : QUndoCommand(parent){
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
}

void MoveCommand::undo(){
    //cout << "UM" << endl;
    myItem->setActiveVertex(0);
    myItem->moveItem(mynewPos, myoldPos);
    myItem->scene()->update();
}

void MoveCommand::redo(){
    //cout << "RM" << endl;
    if(init){
        myItem->setActiveVertex(0);
        myItem->moveItem(myoldPos, mynewPos);
        myItem->scene()->update();
    }
    else{
        init = true;
    }
}

MoveVertexCommand::MoveVertexCommand(SelectItem *item, const QPointF oldPos, const QPointF newPos,
                             const int vertex, QUndoCommand *parent) : QUndoCommand(parent){
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
    myVertex = vertex;
}

void MoveVertexCommand::undo(){
    myItem->resizeItem(myVertex, myoldPos);
    myItem->scene()->update();
}
void MoveVertexCommand::redo(){
    if(init){
        myItem->resizeItem(myVertex, mynewPos);
        myItem->scene()->update();
    }
    else{
        init = true;
    }
}

AddVertexCommand::AddVertexCommand(SelectItem *item, const QPointF point, QUndoCommand *parent)
    : QUndoCommand(parent){
    myItem = item;
    myPoint = point;
}
void AddVertexCommand::undo(){
    myItem->setActiveVertex(UNSELECTED);
    myItem->addPoint(myPoint);
    myItem->scene()->update();
}
void AddVertexCommand::redo(){
    myItem->removeVertex();
    myItem->scene()->update();
}

DeleteVertexCommand::DeleteVertexCommand(SelectItem *item, QUndoCommand *parent)
    : QUndoCommand(parent){
    myItem = item;
    myPoint = item->getActivePoint();
    myVertex = item->getActiveVertex();
}
void DeleteVertexCommand::undo(){
    myItem->setActiveVertex(UNSELECTED);
    myItem->addPoint(myPoint, myVertex);
    myItem->scene()->update();
}
void DeleteVertexCommand::redo(){
    myItem->removeVertex(myVertex);
    myItem->scene()->update();
}
