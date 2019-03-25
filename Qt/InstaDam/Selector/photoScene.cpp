#include <QtWidgets>
#include <iostream>
using namespace std;

#include "photoScene.h"
#include "selectItem.h"

/*!
  \class PhotoScene
  \ingroup Selector
  \inmodule InstaDam
  \inherits QGraphicsScene
  \brief The PhotoScene class provides a scene for rendering SelectItems.

  Provides a class for rendering SelectItems on top of a background image.
  This class emits signals when the mouse moves and/or clicks and when
  keystrokes are made. It holds an internal list of SelectItems that have
  been drawn in the scene.
  */

/*!
  Constructs a PhotoScene instance of type \a type, with parent \a parent, if any.
  */
PhotoScene::PhotoScene(viewerTypes type, QObject *parent)
    : QGraphicsScene(parent), currentItems(), labelmap()
{
    myViewerType = type;
    movingItem = nullptr;
    mousePressed = false;
}

// NEED TO MAKE SURE DRAWING DOES NOT GO BEYOND BOUNDARY
/*!
  Sets all SelectItems in the PhotoScene to inactive.
  */
void PhotoScene::inactiveAll(){
    for(std::list<SelectItem*>::const_iterator iterator = currentItems.begin(), end = currentItems.end(); iterator != end; ++iterator) {
        (*iterator)->setInactive();
    }
}

/*!
  \overload addItem()
  Adds \a item to the scene.
  */
void PhotoScene::addItem(QGraphicsItem* item){
    QGraphicsScene::addItem(item);
}

/*!
  \overload addItem()
  Adds \a item to the scene.
  */
void PhotoScene::addItem(SelectItem* item){
    currentItems.push_front(item);
    QGraphicsScene::addItem(item);
}

/*!
  \overload removeItem()
  Adds \a item to the scene.
  */
void PhotoScene::removeItem(SelectItem* item){
    cout << "  R " << currentItems.size() << endl;
    currentItems.remove(item);
    cout << "    " << currentItems.size() << endl;
    QGraphicsScene::removeItem(item);
}

/*!
  \overload keyPressEvent()

  Executed when a key is pressed. \a event contains the key information.

  \sa QGraphicsScene::keyPressEvent()
  */
void PhotoScene::keyPressEvent(QKeyEvent *event){
    emit keyPressed(myViewerType, event->key());
    QGraphicsScene::keyPressEvent(event);
}

/*!
  \overload mousePressEvent()

  Executed when a mouse button is pressed. \a event contains which button
  was pressed and where in the PhotoScene. The PhotoScene searches for any
  SelectItems under the clicked point and \c emits a pointClicked() signal
  with the result.

  \sa QGraphicsScene::mousePressEvent()
  */
void PhotoScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    cout << "SCENE PRESS" << endl;
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
        mousePressed = true;
        QPointF mousePos(event->scenePos().x(),
                         event->scenePos().y());
        SelectItem *item = itemAt(mousePos);

        oldPos = mousePos;
        newPos = mousePos;
        if(item && item->type() != 7){
            emit pointClicked(myViewerType, item, mousePos, event->button());
        }
        else{
            emit pointClicked(myViewerType, nullptr, mousePos, event->button());
        }

    }
    cout << "S DONE" << endl;
    QGraphicsScene::mousePressEvent(event);
    cout << "SQ DONE" << endl;
}

/*!
  \overload mouseMoveEvent()

  Executed when the mouse moves in the PhotoScene. \a event contains the new
  position of the mouse. If a mouse button is depressed a mouseMoved() signal is
  emitted.

  \sa QGraphicsScene::mouseMoveEvent()
  */
void PhotoScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(mousePressed){
        //cout << "   L" << endl;
        QPointF mousePos(event->scenePos().x(),
                     event->scenePos().y());
        emit mouseMoved(newPos, mousePos);
        newPos = mousePos;
    }
    QGraphicsScene::mouseMoveEvent(event);
}

/*!
  \overload mouseReleaseEvent()

  Executed when a mouse button is released. \a event contains which
  button was released. A mouseReleased() signal is emitted with the PhotoScene
  type, any mouse movement information, and which button was released.

  \sa QGraphicsScene::mouseReleaseEvent()
  */
void PhotoScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
        //cout << "   LEFT" << endl;
        mousePressed = false;
        //cout << oldPos.x() << "," << oldPos.y() << "  " << newPos.x() << "," << newPos.y() << endl;
        emit mouseReleased(myViewerType, oldPos, newPos, event->button());
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

/*!
  Returns a pointer to the top SelectItem at \a point in scene coordinates.
  If there is no SelectItem under that position then \c nullptr is returned.
  */
SelectItem* PhotoScene::itemAt(QPointF point){
    cout << "ITEMAT " << currentItems.size() << endl;
    for(std::list<SelectItem*>::const_iterator iterator = currentItems.begin(), end = currentItems.end(); iterator != end; ++iterator) {
        cout <<"Q" << endl;
        if((*iterator)->isVisible() && (*iterator)->isInside(point)){
            //cout << "Z" << endl;
            return (*iterator);
        }
    }
    //cout << "W" << endl;
    return nullptr;

}

/*!
  \overload itemAt()
  Returns a pointer to the top SelectItem at \a point in scene coordinates and with
  Label \a label. If there is no SelectItem from that Label under that position
  then \c nullptr is returned.
  */
SelectItem* PhotoScene::itemAt(QPointF point, std::string label){

    for(std::list<SelectItem*>::const_iterator iterator = labelmap[label].begin(), end = labelmap[label].end(); iterator != end; ++iterator) {
        if((*iterator)->isInside(point)){
            return (*iterator);
        }
    }
    return nullptr;
}

/*!
  Adds \a label to the PhotoScene
  */
void PhotoScene::addLabel(std::string label){
    std::map<string, std::list<SelectItem*> >::iterator it = labelmap.find(label);
    if (it == labelmap.end()){
        std::list<SelectItem*> newlist;
        labelmap[label] = newlist;
    }
}

/*!
  Sets the currently active Label to be \a label.
  */
void PhotoScene::setCurrentLabel(std::string label){
    currentLabel = label;
}

/*!
  Adds \a item to the Label given by \a label.
  */
void PhotoScene::addLabelItem(SelectItem* item, std::string label){
    currentItems.push_front(item);
    labelmap[label].push_front(item);
}

/*!
  \fn void PhotoScene::pointClicked(viewerTypes type, SelectItem* item, QPointF point, const Qt::MouseButton button)

  Triggered when a mouse button is depressed in the PhotoScene. The signal contains
  which type of viewer this is as \a type, any SelectItem that exists under the
  mouse as \a item, the position of the mouse as \a point, and which mouse button
  was depressed as \a button.
  */

/*!
  \fn void PhotoScene::mouseMoved(QPointF fromPos, QPointF toPos)

  Triggered when the mouse moves in the PhotoScene and a mouse button is depressed.
  The signal contains the starting (\a fromPos) and ending (\a toPos) positions
  of the mouse.
  */

/*!
  \fn void PhotoScene::mouseReleased(viewerTypes type, QPointF oldPos, QPointF newPos, const Qt::MouseButton button)

  Triggered when a mouse button is released. The signal contains which type of
  viewer this is as \a type, he starting (\a oldPos) and ending (\a newPos) positions
  of the mouse (during the time the button was depressed), and which mouse button
  was released as \a button.
  */

/*!
  \fn void PhotoScene::keyPressed(viewerTypes type, const int key)

  Triggered when a key is pressed in the PhotoScene. The signal contains which type of
  viewer this is as \a type and what key was pressed as \a key.
  */

/*!
  \enum PhotoScene::viewerTypes

  This enum is used to indicate which panel this PhotoScene belongs to.

  \value PHOTO_VIEWER_TYPE
      The scene containing the opened image.
  \value MASK_VIEWER_TYPE
      The scene containing the masked image.
  */
