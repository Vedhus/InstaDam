#include <QGraphicsScene>
#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::testFreeMove(){
    QPointF p = QPointF(20,20);
    QPointF e = QPointF(20,100);
    fitem = new FreeDrawSelect(p, 20, Qt::RoundCap, myLabel);
    FreeDrawSelect *mir = new FreeDrawSelect(p, 20, Qt::RoundCap, nullptr);
    fitem->setMirror(mir);
    mir->setMirror(fitem);
    fitem->moveItem(p, e);
}

void TestSelect::testEraseMove(){
    //fitem = new FreeDrawSelect(point, 5, myLabel);
    //myLabel->addItem(fitem);
    //fitem->moveItem(point, brc);
    //feitem = new FreeDrawErase(point, 5, myLabel);
    //feitem->moveItem(point, brc);
}
