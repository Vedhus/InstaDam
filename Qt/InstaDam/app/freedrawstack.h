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
    void push(FreeDrawSelect*);
    int maxLength;
    void merge();
    void clear();
    void undo();
    void redo(FreeDrawSelect* );
    int* showMirrorPtr;


};





#endif // FREEDRAWSTACK_H
