#include "freedrawstack.h"

#include "label.h"

#include "QDebug"
/*!
 * \brief FreeDrawStack::FreeDrawStack
 * This stores the merge stack of freedraw items
 */
FreeDrawStack::FreeDrawStack(int maxStackLength, PhotoScene* scene, PhotoScene* mScene, int* showMirror)
{
    maxLength = maxStackLength;
    photoScene = scene;
    maskScene = mScene;
    showMirrorPtr = showMirror;
//    stack.reserve(maxStackLength);

}

void FreeDrawStack::push(FreeDrawSelect* fdsInput){
    //if (fdsInput)
    stack.push_back(fdsInput);
    qInfo()<<stack.size();
    merge();
}

void FreeDrawStack::clear(){
    stack.clear();
}

void FreeDrawStack::undo(){
    FreeDrawSelect *last = stack.takeLast();
    QSharedPointer<Label> currentLabel = last->getLabel();
    currentLabel->removeItem(last->myID);
}

void FreeDrawStack::redo(FreeDrawSelect* last){
    QSharedPointer<Label> currentLabel = last->getLabel();
    currentLabel->addItem(last);


}
void FreeDrawStack::merge(){

    if (stack.size()==maxLength){
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


        /* Remove the old merged items in the scene */
        if (stack[0] != pastMerged){
            /* Replace the merged pointer in the label queue and scene */
            maskScene->permanentRemove(pastMerged->getMirror());
            photoScene->permanentRemove(pastMerged);
        }
        mergeList.clear();

        /* Remove the most recently merged item */
        currentLabel->mergeFDS(merged, stack[0]->myID);
        maskScene->permanentRemove(stack[0]->getMirror());
        photoScene->permanentRemove(stack[0]);
        stack.removeFirst();


        /* Finally, add the new merged item to the scene */
        photoScene->addItem(merged);
        maskScene->addItem(mergedMirror);
        photoScene->update();
        qInfo()<<"Temp assigned created!";
        currentLabel->setMaskState(*showMirrorPtr);


    }
}
