#include <QGraphicsScene>
#include "testSelect.h"
#include <iostream>
using namespace std;

void TestSelect::testFreeMove(){
    fitem = new FreeDrawSelect(point, 5, myLabel);
    fitem->moveItem(point, brc);
}

void TestSelect::testEraseMove(){
    fitem = new FreeDrawSelect(point, 5, myLabel);
    myLabel->addItem(fitem);
    fitem->moveItem(point, brc);
    feitem = new FreeDrawErase(point, 5, myLabel);
    feitem->moveItem(point, brc);
}
