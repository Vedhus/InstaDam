#include "freeDrawErase.h"

#include "label.h"

/*!
  \class FreeDrawErase
  \ingroup Selector
  \inmodule InstaDam
  \inherits FreeDrawSelect
  \brief The FreeDrawErase class provides a class for erasing regions breated
         by a FreeDrawSelect.

  Provides a class for erasing from selection regions created by FreeDrawSelect
  objects. This is done  by painting on the canvas with a brush. The region is
  defined by the pixels that are painted on. This class exists only to remove
  pixels from existing FreeDrawSelect objects and provide undo/redo capabilities
  to the deletion.
  */

/*!
  Constructs a FreeDrawErase instance withan initial selected point \a point,
  with a brush size of \a brushSize pixels, brush type of \a brushMode, \a label
  as the Label which owns this object and \a item is the parent QGraphicsItem,
  if any. Acceptable values for the \a brushMode are:

  \list
  \li Qt::SquareCap
  \li Qt::RoundCap
  \endlist
  */
FreeDrawErase::FreeDrawErase(QPointF point, int brushSize,
                             Qt::PenCapStyle brushMode,
                             QSharedPointer<Label> label, QGraphicsItem *item)
    : FreeDrawSelect(point, brushSize, brushMode, nullptr, item) {
    myLabel = label;
    undoMap = QSharedPointer<EraseMap>::create();
    QHashIterator<int, FreeDrawSelect*> it(myLabel->freeDrawObjects);
    myPen.setColor(Qt::transparent);
    while (it.hasNext()) {
        it.next();
        QSharedPointer<QPixmap> delHash =
                QSharedPointer<QPixmap>::create(SelectItem::myBounds);
        delHash->fill(Qt::transparent);
        it.value()->deletePoints(point, point, myPen, delHash);
        undoMap->insert(it.value(), delHash);
    }
    deleteList = QVector<int>();
    mytype = SelectItem::Freeerase;
}

/*!
  Destructor
  */
FreeDrawErase::~FreeDrawErase() {
}

/*-------------------------- Overrides ---------------------------*/
/*!
  \reimp
  */
void FreeDrawErase::moveItem(const QPointF &oldPos, QPointF &newPos) {
    EraseMapIterator it((*undoMap));
    while (it.hasNext()) {
        it.next();
        it.key()->deletePoints(oldPos, newPos, myPen, it.value());
    }
    lastPoint = newPos.toPoint();
#ifndef TEST
    deleteList.clear();
#endif
}
/*------------------------- End Overrides ----------------------*/


/*!
  \fn bool FreeDrawErase::isInside(const QPointF &point) const
  \reimp

  Empty function as a point is not in an erased region.
  */

/*!
  \fn void FreeDrawErase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
  \reimp

  Empty function as FreeDrawErase objects are not painted
  */

/*!
  \fn QSharedPointer<EraseMap> FreeDrawErase::getMap() const

  Returns a pointer to an EraseMap containing a map of the points deleted from
  each FreeDrawSelect object.
  */

/*!
  \fn void FreeDrawErase::toPixmap(QPainter *painter)
  \reimp

  Empty function as FreeDrawErase objects are not painted
  */
