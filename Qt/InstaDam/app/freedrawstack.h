#ifndef FREEDRAWSTACK_H
#define FREEDRAWSTACK_H

#include <QPixmap>
#include <QVector>
#include <QList>
#include "freeDrawSelect.h"
#include "freeDrawErase.h"
#include "photoScene.h"
class FreeDrawStack
{
public:
    FreeDrawStack(int, PhotoScene* , PhotoScene*, int*);
    PhotoScene* photoScene;
    PhotoScene* maskScene;
    QList<FreeDrawSelect*> stack;
    QList<FreeDrawSelect*> tempUndoStack;
    void push(FreeDrawSelect*);
    void trimUndoStack();
    int maxLength;
    void merge();
    void clear();
    void undo();
    void redo(SelectItem * );
    int* showMirrorPtr;
    void clearUndoStack();
    void mergeErase(SelectItem* , FreeDrawSelect*, FreeDrawSelect* );
    bool isClear;


};





#endif // FREEDRAWSTACK_H
