#include "freedrawstack.h"
#include "pixmapops.h"
#include "label.h"
#include "freeDrawErase.h"
#include "QDebug"
/*!
 * \brief FreeDrawStack::FreeDrawStack
 * This stores the merge stack of freedraw items to save memory. Everytime the number of freedraw actions
 * surpasses the max undo limit, the odlest valid action is merged and added to the scene and label.
 */
FreeDrawStack::FreeDrawStack(int maxStackLength, PhotoScene* scene, PhotoScene* mScene, int* showMirror)
{
    maxLength = maxStackLength+1;
    photoScene = scene;
    maskScene = mScene;
    showMirrorPtr = showMirror;
    stack.reserve(maxStackLength);

}

void FreeDrawStack::push(FreeDrawSelect* fdsInput){
    stack.push_back(fdsInput);

    merge();
}

void FreeDrawStack::clear(){
    stack.clear();
    tempUndoStack.clear();
}

void FreeDrawStack::undo(){
    qInfo()<<"In freedrawstack undo";
    if (!stack.isEmpty()){
        SelectItem *last = stack.takeLast();
        last->inUndoQ = true;
        tempUndoStack.push_back((FreeDrawSelect*)last);
        qInfo()<<"Size of undo Stack: "<<tempUndoStack.size();

//        if (last->type() == SelectItem::Freedraw){
//            QSharedPointer<Label> currentLabel = last->getLabel();
//            currentLabel->removeItem(last->myID);
//        }
    }
}

//void FreeDrawStack::redo(FreeDrawSelect* last){
//    QSharedPointer<Label> currentLabel = last->getLabel();
//    SelectItem* temp = last;

//    if (temp->type() == SelectItem::Freeerase){
//        stack.push_back(last);
//        //photoScene->addItem(last);
//        merge();
//    }
//    else{
//        currentLabel->addItem(last);
//    }


//}

void FreeDrawStack::redo(SelectItem* last){
    stack.push_back((FreeDrawSelect*)last);
    tempUndoStack.removeOne((FreeDrawSelect*)last);
    merge();
//    if (last->type() == SelectItem::Freeerase){
//        stack.push_back((Freedraw*)last);
//    }
//    else if (last_type() == SelectItem::Freedraw){
//        stack.push_back()
//    }
}

void FreeDrawStack::clearUndoStack(){
    while (!tempUndoStack.isEmpty()){
            trimUndoStack();
    }
}


void FreeDrawStack::trimUndoStack(){
    if (!tempUndoStack.isEmpty()){

        SelectItem* item = tempUndoStack.takeLast();
        QSharedPointer<Label> currentLabel = item->getLabel();

        if (item->type() == SelectItem::Freedraw){
            currentLabel->removeItem(item->myID);
            maskScene->permanentRemove(item->getMirror());
        }
        photoScene->permanentRemove(item);
    }
}


void FreeDrawStack::mergeErase(SelectItem* eraseTemp, FreeDrawSelect* pastMerged,
                               FreeDrawSelect* merged){

    FreeDrawErase* erase = (FreeDrawErase*) eraseTemp;
    erase->addToMap(pastMerged, stack[0], merged);


//    QSharedPointer<QPixmap> hash0 = QSharedPointer<QPixmap>::create(SelectItem::myBounds);
//    hash0->fill(Qt::transparent);


//    QSharedPointer<QPixmap> hashPastMerged = QSharedPointer<QPixmap>::create(SelectItem::myBounds);
//    hashPastMerged->fill(Qt::transparent);


//    FreeDrawErase* erase = (FreeDrawErase*) eraseTemp;
//    QSharedPointer<EraseMap>  map = erase->getMap();


//    QMap<FreeDrawSelect*, QSharedPointer<QPixmap>>::iterator eraseMap0 = map->find(pastMerged);

//    if (eraseMap0!= map->constEnd()){
//        hash0 = eraseMap0.value();
//        map->remove(pastMerged);

//    }
//    //merged->deletePoints(erase->myPen, hash0);
//    QMap<FreeDrawSelect*, QSharedPointer<QPixmap>>::iterator eraseMapMerged = erase->getMap()->find(stack[0]);

//    if (eraseMapMerged!= map->constEnd()){
//        hashPastMerged = eraseMapMerged.value();
//        map->remove(stack[0]);

//    }

//    QPixmap hashMergedPixmap = joinSharedPointerPixmaps(hashPastMerged, hash0);
////    QSharedPointer<QPixmap>hashMerged (&hashMergedPixmap);


//    erase->addToMap(hashMergedPixmap, merged);

    qInfo()<<"Map inseted";

}
void FreeDrawStack::merge(){
    qInfo()<<stack.size();
    /* Check first if the element at the top of the stack is an undo element. If it is not, then
     * the elements in the undo stack can be removed from memory */
    FreeDrawErase* eraseTemp;
    QSharedPointer<EraseMap>  map;

    SelectItem* lastItem = stack.last();
    while (tempUndoStack.size() + stack.size()>maxLength)
    {
        trimUndoStack();
    }
    if (stack.size()==maxLength){
        SelectItem* temp = stack[0];
         if (temp->type() == SelectItem::Freeerase){
             photoScene->permanentRemove(stack.takeFirst());
         }
         else{
            QSharedPointer<Label> currentLabel = stack[0]->getLabel();
            FreeDrawSelect* pastMerged = currentLabel->mergedFDS;
            QList<FreeDrawSelect*> mergeList;
            mergeList.push_back(pastMerged);
            mergeList.push_back(stack[0]);
            qInfo()<<"Merge list created!";


            FreeDrawSelect *merged = new FreeDrawSelect(mergeList, nullptr);
            FreeDrawSelect *mergedMirror = new FreeDrawSelect(mergeList, nullptr);
            mergedMirror->setMirror(merged);
            merged->setMirror(mergedMirror);
            mergedMirror->setLabel(currentLabel);
            merged->setLabel(currentLabel);

            /* Replace merged items from freedrawobjects of label */
            currentLabel->mergeFDS(merged, stack[0]->myID);

            // Remove the old merged items from all free erase items in the stack
            for (int i = 0; i<stack.size(); i++)
            {
                SelectItem* erase = stack[i];

                if (erase->type() == SelectItem::Freeerase)
                {
                    mergeErase(erase, pastMerged, merged);
                    eraseTemp = (FreeDrawErase*) erase;
                    map = eraseTemp->getMap();
                    qInfo()<<"FreeDrawStack";
                   // mergeErase(erase->getMirror(), pastMerged->getMirror(), mergedMirror);

                }

            }




            if (stack[0] != pastMerged){
                /* Replace the merged pointer in the label queue and scene */
                maskScene->permanentRemove(pastMerged->getMirror());
                photoScene->permanentRemove(pastMerged);
            }


            mergeList.clear();

            /* Remove the most recently merged item */            
            maskScene->permanentRemove(stack[0]->getMirror());
            photoScene->permanentRemove(stack[0]);
            stack.removeFirst();


            /* Finally, add the new merged item to the scene */
            photoScene->addItem(merged);
            maskScene->addItem(mergedMirror);
            photoScene->update();
            qInfo()<<"Temp assigned created!";
            currentLabel->setMaskState(*showMirrorPtr);
            qInfo()<<"Done with merge!";


        }
    }
}
