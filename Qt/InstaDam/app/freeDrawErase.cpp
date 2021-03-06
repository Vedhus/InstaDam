#include "freeDrawErase.h"
#include "pixmapops.h"
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
                             QSharedPointer<Label> label,
                             PhotoScene::viewerTypes sceneType,
                             QGraphicsItem *item)
    : FreeDrawSelect(point, brushSize, brushMode, nullptr, sceneType, item) {
    myLabel = label;
    undoMap = QSharedPointer<EraseMap>::create();
    QHashIterator<int, FreeDrawSelect*> it(myLabel->freeDrawObjects);
    myPen.setColor(Qt::transparent);
    while (it.hasNext()) {
        it.next();
        QSharedPointer<QPixmap> delHash =
                QSharedPointer<QPixmap>::create(SelectItem::myBounds);
        delHash->fill(Qt::transparent);
        qInfo()<<"In FreeDrawErase";
        qInfo()<<it.key();
        it.value()->deletePoints(point, point, myPen, delHash);
        undoMap->insert(it.value(), delHash);

    }
    deleteList = QVector<int>();
    mytype = SelectItem::Freeerase;
}

/* This function removes pastmerged items from the undo stack from the erase map
 * and adds new merged items */
FreeDrawErase::addToMap(FreeDrawSelect* pastMerged, FreeDrawSelect* stack0, FreeDrawSelect* merged){


    QSharedPointer<QPixmap> hash0 = QSharedPointer<QPixmap>::create(SelectItem::myBounds);
    hash0->fill(Qt::transparent);
    QSharedPointer<QPixmap> hashPastMerged = QSharedPointer<QPixmap>::create(SelectItem::myBounds);
    hashPastMerged->fill(Qt::transparent);


    QMap<FreeDrawSelect*, QSharedPointer<QPixmap>>::iterator eraseMap0 = undoMap->find(pastMerged);

    if (eraseMap0!= undoMap->constEnd()){
        hash0 = eraseMap0.value();
        undoMap->remove(pastMerged);
    }
    merged->deletePoints(myPen, hash0);
    QMap<FreeDrawSelect*, QSharedPointer<QPixmap>>::iterator eraseMapMerged = undoMap->find(stack0);

    if (eraseMapMerged!= undoMap->constEnd()){
        hashPastMerged = eraseMapMerged.value();
        undoMap->remove(stack0);
    }

    merged->deletePoints(myPen, hashPastMerged);
    QPixmap hashMergedPixmap = joinSharedPointerPixmaps(hashPastMerged, hash0);
    QSharedPointer<QPixmap> hashMerged =
            QSharedPointer<QPixmap>::create(hashMergedPixmap);
    undoMap->insert(merged, hashMerged);


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

  Empty function as FreeDrawErase objects are not painted.
  */

/*!
  \fn QSharedPointer<EraseMap> FreeDrawErase::getMap() const

  Returns a pointer to an EraseMap containing a map of the points deleted from
  each FreeDrawSelect object.
  */

/*!
  \fn void FreeDrawErase::toPixmap(QPainter *painter)
  \reimp

  Empty function as FreeDrawErase objects are not painted.
  */

/*!
  \fn void FreeDrawErase::setOpacity(qreal val)

  Empty function as FreeDrawErase objects are not painted. \a val is unused.
  */
