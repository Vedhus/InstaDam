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
    myItem->hide();
    myScene->update();
}

void AddCommand::redo()
{
    if(init){
        myItem->show();
        myScene->clearSelection();
        myScene->update();
    }
    else{
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
    //myScene->addItem(myItem);
    myItem->show();
    myScene->update();
}

void DeleteCommand::redo()
{
    //myScene->removeItem(myItem);
    if(init){
        myItem->hide();
        myScene->update();
    }
    else{
        init = true;
    }
}

MoveCommand::MoveCommand(SelectItem *item, const QPointF oldPos,
                         const QPointF newPos,
                         QUndoCommand *parent) : QUndoCommand(parent){
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
}

void MoveCommand::undo(){
    myItem->setActiveVertex(0);
    myItem->moveItem(mynewPos, myoldPos);
    myItem->scene()->update();
}

void MoveCommand::redo(){
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

