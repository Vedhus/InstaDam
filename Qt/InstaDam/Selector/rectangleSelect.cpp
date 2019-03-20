#include "rectangleSelect.h"
#include "label.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>
using namespace std;

/*!
  \class RectangleSelect
  \ingroup Selector
  \inmodule InstaDam
  \inherits SelectItem QGraphicsRectItem
  \brief The RectangleSelect class provides a class for annotating rectangular regions.

  Provides a class for annotating rectangular regions in InstaDam. The region is described
  by its top-left and lower-right vertices, and by its angle of rotation.
  */

/*!
  Constructs a RectangleSelect object with all vertices at 0,0 in scene coordinates
  */
RectangleSelect::RectangleSelect() : RectangleSelect(QPointF(0.,0.)){

}

/*!
  Constructs a RectangleSelect object by reading a QJsonObject and setting the internal rectangle and rotation angle
  to the values given in \a json. \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
RectangleSelect::RectangleSelect(const QJsonObject &json, QSharedPointer<Label> label, QGraphicsItem *item)
    : BoxBasedSelector(json, label, item), QGraphicsRectItem(item){
    setRect(myRect);
    mytype = SelectItem::Rectangle;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    calcCorners();
    QGraphicsRectItem::prepareGeometryChange();
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Constructs a RectangleSelect object by setting all vertices to be a \a point, \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
RectangleSelect::RectangleSelect(QPointF point, QSharedPointer<Label> label, QGraphicsItem *item)
    : BoxBasedSelector(point, label, item), QGraphicsRectItem(item)
{
    setRect(myRect);
    mytype = SelectItem::Rectangle;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Constructs a RectangleSelect object by setting all vertices to be a \a point, \a vertSize indicates the size of the
  vertex highlight boxes, \a label is the Label which owns this object and \a item is the
  parent QGraphicsItem, if any.
  */
RectangleSelect::RectangleSelect(QPointF point, qreal vertSize, QSharedPointer<Label> label, QGraphicsItem *item)
    : BoxBasedSelector(point, vertSize,label, item), QGraphicsRectItem(item)
{
    //cout << "RR" << endl;
    setRect(myRect);
    mytype = SelectItem::Rectangle;
    if(label)
        label->addItem(this);
    updatePen(myPen);
    invertColorForPen();
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QGraphicsRectItem::setFlag(QGraphicsItem::ItemIsMovable);
}

/*!
  Destructor
  */
RectangleSelect::~RectangleSelect(){

}

/*---------------------- Overrides ---------------------------*/
/*!
  \reimp
  */
void RectangleSelect::addPoint(QPointF &point, int vertex){
    UNUSED(vertex);
    sortCorners(myRect, point);
    calcCorners();

    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
    if(mirror != nullptr)
        mirror->setRectUnchecked(myRect);
}

/*!
  \reimp
  */
QRectF RectangleSelect::boundingRect() const{
    return QGraphicsRectItem::boundingRect();
}

/*!
  \reimp
  */
bool RectangleSelect::isInside(QPointF &point){
    return QGraphicsRectItem::contains(point);
}


/*!
  \reimp
  */
void RectangleSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    // if there is an active vertex then we are resizing
    if(activeVertex != 0){
        resized = true;
        setMirrorResized();
        addPoint(newPos);
    }
    // otherwise we are moving the entire object
    else{
        moved = true;
        setMirrorMoved();
        QPointF shift = newPos - oldPos;
        checkBoundaries(shift, myRect);
    }
    calcCorners();
    QGraphicsRectItem::prepareGeometryChange();
    setRect(myRect);
    // update the mirror if there is one
    if(mirror != nullptr)
        mirror->setRectUnchecked(myRect);
}

/*!
  \reimp
  */
void RectangleSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsRectItem::paint(painter, option, widget);

    if(active){
        painter->setBrush(brush());
        painter->drawRect(tl);
        painter->drawRect(bl);
        painter->drawRect(tr);
        painter->drawRect(br);
    }
}

/*!
  \reimp
  */
void RectangleSelect::updatePen(QPen pen){
    setPen(pen);
    QColor col = pen.color();
    col.setAlphaF(0.25);
    QBrush brush(col);
    setBrush(brush);
}

/*!
  \reimp
  */
void RectangleSelect::setRectUnchecked(QRectF rect){
    QGraphicsRectItem::prepareGeometryChange();
    //cout << "S R U" << endl;
    myRect = rect;
    setRect(myRect);
}


/*---------------------------- Mirror functions ----------------------*/
/*!
  \reimp
  */
void RectangleSelect::mirrorHide(){
    if(mirror != nullptr)
        mirror->SelectItem::hide();
}

/*!
  \reimp
  */
void RectangleSelect::mirrorShow(){
    if(mirror != nullptr)
        mirror->SelectItem::show();
}

/*!
  \reimp
  */
void RectangleSelect::rotateMirror(){
    if(mirror != nullptr){
        mirror->myRotation = myRotation;
        mirror->BoxBasedSelector::setTransformOriginPoint(myRect.center());
        mirror->BoxBasedSelector::setRotation(myRotation);
    }
}

/*!
  \reimp
  */
void RectangleSelect::setMirror(SelectItem *item){
    //cout << "MIRROR " << myID << endl;
    mirror = dynamic_cast<RectangleSelect*>(item);
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorActive(){
    if(mirror != nullptr)
        mirror->active = true;
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorCorners(QRectF tlc, QRectF blc, QRectF trc, QRectF brc){
    if(mirror != nullptr){
        //cout << " SET M C" << endl;
        mirror->tl = tlc;
        mirror->bl = blc;
        mirror->tr = trc;
        mirror->br = brc;
    }
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorMoved(){
    if(mirror != nullptr)
        mirror->moved = moved;
}


/*!
  \reimp
  */
void RectangleSelect::setMirrorResized(){
    if(mirror != nullptr)
        mirror->resized = resized;
}

/*!
  \reimp
  */
void RectangleSelect::setMirrorVertex(int vertex){
    if(mirror != nullptr){
        //cout << "SET A V M" << endl;
        mirror->setActiveVertex(vertex);
    }
}

/*!
  \reimp
  */
void RectangleSelect::updateMirrorScene(){
    if(mirror != nullptr)
        mirror->scene()->update();
}

/*!
  \overload SelectItem::scene()

  Returns the QGraphicsScene to which this item belongs.
  */
QGraphicsScene* RectangleSelect::scene(){
    return SelectItem::scene();
}

/*!
  \fn QString RectangleSelect::baseInstructions()

  \reimp
  */

/*!
  \fn RectangleSelect* RectangleSelect::getMirror()

  \reimp
  */

/*!
  \fn bool RectangleSelect::isVisible()

  Returns whether the RectangleSelect object is visible (\c true) on the QGraphicsScene or not (\c false).
  */

/*!
  \fn void RectangleSelect::setMirrorAdded()

  \reimp
  */

/*!
  \fn int RectangleSelect::type()
  \overload SelectItem::type()

  This function returns the type of this item.

  \sa QGraphicsItem::type(), SelectItem::type()
  */


