#include <QtWidgets>
#include <QVector>
#include <iostream>
using namespace std;

#include "commands.h"
#include "selectItem.h"

AddCommand::AddCommand(SelectItem *item,
                       PhotoScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myScene = scene;
    myItem = item;
}


AddCommand::~AddCommand()
{
}


void AddCommand::undo()
{
    //cout << "UA" << endl;
    myItem->hide();
    myItem->mirrorHide();
    myScene->update();
    myItem->updateMirrorScene();
}

void AddCommand::redo()
{
    //cout << "RA" << endl;
    if(init){
        myItem->show();
        myItem->mirrorShow();
        myScene->clearSelection();
        myScene->update();
        myItem->updateMirrorScene();
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
    myItem->mirrorShow();
    myScene->update();
    myItem->updateMirrorScene();
}

void DeleteCommand::redo()
{
    //cout << "RD" << endl;
    //myScene->removeItem(myItem);
    //if(init){
        myItem->hide();
        myScene->update();
        myItem->mirrorHide();
        myItem->updateMirrorScene();
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
    myItem->resetActiveVertex();
    myItem->moveItem(mynewPos, myoldPos);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

void MoveCommand::redo(){
    //cout << "RM" << endl;
    if(init){
        myItem->resetActiveVertex();
        myItem->moveItem(myoldPos, mynewPos);
        myItem->scene()->update();
        myItem->updateMirrorScene();
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
    myItem->updateMirrorScene();
}
void MoveVertexCommand::redo(){
    //cout << "MVC" << endl;
    if(init){
        myItem->resizeItem(myVertex, mynewPos);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    }
    else{
        init = true;
    }
}

AddVertexCommand::AddVertexCommand(SelectItem *item, const QPointF point, QUndoCommand *parent)
    : QUndoCommand(parent){
    myItem = item;
    myPoint = point;
    init = false;
}
void AddVertexCommand::undo(){
    //cout << "RVC" << endl;
    myItem->removeVertex();
    myItem->scene()->update();
    myItem->updateMirrorScene();
}
void AddVertexCommand::redo(){
    if(init){
        //cout << "AVC" << endl;
        myItem->setActiveVertex(UNSELECTED);
        myItem->addPoint(myPoint);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    }
    else{
        init = true;
    }
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
    myItem->updateMirrorScene();
}

void DeleteVertexCommand::redo(){
    myItem->removeVertex(myVertex);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

ErasePointsCommand::ErasePointsCommand(FreeDrawErase *item, PhotoScene *scene, PhotoScene *maskScene, QUndoCommand *parent)
    : QUndoCommand(parent){
    myItem = item;
    myScene = scene;
    myMask = maskScene;
}

void ErasePointsCommand::undo(){
    //cout << "UNDO" << endl;
    EraseMapIterator it((*myItem->getMap()));
    while(it.hasNext()){
        //cout << "ADD P" << endl;
        it.next();
        it.key()->addPoints(it.value());
    }
    myScene->update();
    myMask->update();
}

void ErasePointsCommand::redo(){
    if(init){
        EraseMapIterator it((*myItem->getMap()));
        FreeMap tempMap = FreeMap();
        while(it.hasNext()){
            it.next();
            QVector<int> tempVector = it.value()->keys().toVector();
            it.key()->deletePoints(tempVector, &tempMap);
        }
    }
    else{
        init = true;
    }
    myScene->update();
    myMask->update();
}

RotateCommand::RotateCommand(SelectItem *item, const QPointF oldPos,
                         const QPointF newPos,
                         QUndoCommand *parent) : QUndoCommand(parent){
    myItem = item;
    myoldPos = oldPos;
    mynewPos = newPos;
}

void RotateCommand::undo(){
    //cout << "UM" << endl;
    myItem->resetActiveVertex();
    myItem->rotate(mynewPos, myoldPos);
    myItem->scene()->update();
    myItem->updateMirrorScene();
}

void RotateCommand::redo(){
    //cout << "RM" << endl;
    if(init){
        myItem->resetActiveVertex();
        myItem->rotate(myoldPos, mynewPos);
        myItem->scene()->update();
        myItem->updateMirrorScene();
    }
    else{
        init = true;
    }
}
