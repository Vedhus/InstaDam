#include <QtWidgets>
#include <iostream>
using namespace std;

#include "photoScene.h"
#include "selectItem.h"

PhotoScene::PhotoScene(QObject *parent)
    : QGraphicsScene(parent), currentItems(), labelmap()
{
    movingItem = nullptr;
    mousePressed = false;
}

// NEED TO MAKE SURE DRAWING DOES NOT GO BEYOND BOUNDARY
void PhotoScene::inactiveAll(){
    for(std::list<SelectItem*>::const_iterator iterator = currentItems.begin(), end = currentItems.end(); iterator != end; ++iterator) {
        (*iterator)->setInactive();
    }

}
void PhotoScene::addItem(QGraphicsItem* item){
    QGraphicsScene::addItem(item);
}
void PhotoScene::addItem(SelectItem* item){
    currentItems.push_front(item);
    QGraphicsScene::addItem(item);
    //item->setScene();
}


void PhotoScene::keyPressEvent(QKeyEvent *event){
    //cout << "KEY " << event->key() << "  " << Qt::Key_Delete << "  " << Qt::Key_Backspace << endl;
    if(currentItem && (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)){
        //cout << "EMIT" << endl;
        emit deleteObject(currentItem);
        //cout << "NULL" << endl;
        currentItem = nullptr;
        //cout << "INVAL" << endl;
        inactiveAll();
    }

    QGraphicsScene::keyPressEvent(event);
}

void PhotoScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //cout << "BUTTON" << endl;
    if(event->button() == Qt::LeftButton){
        //cout << "   LEFT" << endl;
        mousePressed = true;
        //cout << "X" << endl;
        QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                         event->buttonDownScenePos(Qt::LeftButton).y());
        //QTransform transform = QTransform();
        //cout << "Y" << endl;
        SelectItem *item = itemAt(mousePos);
        //cout << "TYPE " << item->type() << endl;
        //cout << "X" << endl;

        oldPos = mousePos;
        newPos = mousePos;
        if(item && item->type() != 7){
            //cout << "1" << endl;
            switch(item->type()){
                case Rect:
                    currentItem = (RectangleSelect *)item;
                    break;
                case Ellipse:
                    currentItem = (EllipseSelect *)item;
                    break;
            }
            //if((SelectItem *)item != currentItem){
            //    inactiveAll();
            //    currentItem = (SelectItem *)item;
                //cout << typeid(item).name() << endl;
                //int mytype = item->type();
                //cout << "H" << mytype<< endl;
                inactiveAll();
                emit itemSelected(currentItem, mousePos);
            //}
        }
        else{
            //cout << "0" << endl;
            inactiveAll();
            currentItem = nullptr;
            emit pointClicked(mousePos);
        }
        //myRect.setTopLeft(mousePos);
        //myRect.setBottomRight(mousePos);
        //emit itemAdded(mousePose);
    /*QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                     event->buttonDownScenePos(Qt::LeftButton).y());
    const QList<QGraphicsItem *> itemList = items(mousePos);
    movingItem = itemList.isEmpty() ? 0 : itemList.first();

    if (movingItem != 0 && event->button() == Qt::LeftButton) {
        oldPos = movingItem->pos();
    }

    clearSelection();
     * */
    }
    QGraphicsScene::mousePressEvent(event);
}

void PhotoScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    //cout << "MOVED" << endl;
    if(mousePressed){
        //cout << "   L" << endl;
        QPointF mousePos(event->scenePos().x(),
                     event->scenePos().y());
        //myRect.setBottomRight(mousePos);

        if(!currentItem){
            //cout << "MNB  " << mousePos.x() << "," << mousePos.y()<< endl;
            emit movedPoint(mousePos);
        }
        else{
            //cout << "MMMM" << endl;
            currentItem->moveItem(newPos, mousePos);
            //oldPos = mousePos;
            //cout << "DOREF" << endl;
            emit doRefresh();
        }
        newPos = mousePos;
        //emit drawItem(myRect);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void PhotoScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    /*
    if (movingItem != 0 && event->button() == Qt::LeftButton) {
        if (oldPos != movingItem->pos())
            emit itemMoved(qgraphicsitem_cast<DiagramItem *>(movingItem),
                           oldPos);
        movingItem = 0;
    }*/
    //cout << "RELEASED" << endl;
    if(event->button() == Qt::LeftButton){
        //cout << "   LEFT" << endl;
        mousePressed = false;
        emit addNewItem(oldPos, newPos);//(myRect);
        //currentItem = nullptr;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

SelectItem* PhotoScene::itemAt(QPointF point){
    //cout << "ITEMAT " << currentItems.size() << endl;
    for(std::list<SelectItem*>::const_iterator iterator = currentItems.begin(), end = currentItems.end(); iterator != end; ++iterator) {
        //cout <<"Q" << endl;
        if((*iterator)->isVisible() && (*iterator)->isInside(point)){
            //cout << "Z" << endl;
            return (*iterator);
        }
    }
    //cout << "W" << endl;
    return nullptr;

}

SelectItem* PhotoScene::itemAt(QPointF point, std::string label){

    for(std::list<SelectItem*>::const_iterator iterator = labelmap[label].begin(), end = labelmap[label].end(); iterator != end; ++iterator) {
        if((*iterator)->isInside(point)){
            return (*iterator);
        }
    }
    return nullptr;
}

void PhotoScene::addLabel(std::string label){
    std::map<string, std::list<SelectItem*> >::iterator it = labelmap.find(label);
    if (it == labelmap.end()){
        std::list<SelectItem*> newlist;
        labelmap[label] = newlist;
    }
}
void PhotoScene::setCurrentLabel(std::string label){
    currentLabel = label;
}

void PhotoScene::addLabelItem(SelectItem* item, std::string label){
    currentItems.push_front(item);
    labelmap[label].push_front(item);
}

