#include "freeDrawErase.h"
#include "label.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265
using namespace std;

FreeDrawErase::FreeDrawErase(QPointF point, int brushSize, int brushMode, Label *label, QGraphicsItem *item)
    : FreeDrawSelect(point, brushSize, brushMode, nullptr, item){
    myLabel = label;
    undoMap = new EraseMap();
    QHashIterator<int, FreeDrawSelect*> it(myLabel->freeDrawObjects);
    while(it.hasNext()){
        it.next();
        FreeMap *delHash = new FreeMap();
        it.value()->deletePoint(pointToInt(point.toPoint()), delHash);
        undoMap->insert(it.value(), delHash);
    }
    deleteList = QVector<int>();
    mytype = Freeerase;
}

FreeDrawErase::~FreeDrawErase(){
    if(myMap != nullptr)
        delete myMap;
    if(undoMap != nullptr)
        delete undoMap;
    if(myLabel != nullptr)
        delete myLabel;
}

void FreeDrawErase::voidPoint(QPoint &point){
    {UNUSED(point);}
}

void FreeDrawErase::rasterizeLine(QPoint &start, QPoint &end){
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();
    qreal step;
    qreal x, y;
    if(abs(dx) >= abs(dy)){
        step = abs(dx);
    }
    else{
        step = abs(dy);
    }
    dx = dx / step;
    dy = dy / step;
    x = start.x();
    y = start.y();
    for(int i = 1.; i <= step; i++){
        deleteList.push_back(coordsToInt(int(x), int(y)));
        x += dx;
        y += dy;
    }
}

void FreeDrawErase::drawWithSquare(QPointF &oldPos, QPointF &newPos){
    QPoint start = oldPos.toPoint();
    QPoint end = newPos.toPoint();
    int sdx, sdy, edx, edy;
    if(start.x() == end.x()){
        sdx = edx = 1;
        start.rx() -= halfWidth;
        end.rx() -= halfWidth;
        sdy = edy = 0;
    }
    else if(start.y() == end.y()){
        sdy = edy = 1;
        start.ry() -= halfWidth;
        end.ry() -= halfWidth;
        sdx = edx = 0;
    }
    else if(start.x() > end.x()){
        sdx = -1;
        edx = 1;
        start.rx() += halfWidth;
        end.rx() -= halfWidth;
        if(start.y() > end.y()){
            sdy = -1;
            edy = 1;
            start.ry() += halfWidth;
            end.ry() -= halfWidth;
        }
        else{
            sdy = 1;
            edy = -1;
            start.ry() -= halfWidth;
            end.ry() += halfWidth;
        }
    }
    else{
        sdx = 1;
        edx = -1;
        start.rx() -= halfWidth;
        end.rx() += halfWidth;
        if(start.y() > end.y()){
            start.ry() += halfWidth;
            end.ry() -= halfWidth;
            sdy = -1;
            edy = 1;
        }
        else{
            sdy = 1;
            edy = -1;
            start.ry() -= halfWidth;
            end.ry() += halfWidth;
        }
    }
    if(sdx == 0){
        for(int i = 0; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.ry() += sdy;
            end.ry() += edy;
        }
    }
    else if(sdy == 0){
        for(int i = 0; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.rx() += sdx;
            end.rx() += edx;
        }
    }
    else{
        for(int i = 0; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.rx() += sdx;
            end.ry() += edy;
        }
        start.rx() -= (fullWidth - 1) * sdx;
        end.ry() -= (fullWidth - 1) *edy;
        start.ry() += sdy;
        end.rx() += edx;
        for(int i = 1; i < fullWidth; i++){
            rasterizeLine(start, end);
            start.ry() += sdy;
            end.rx() += edx;
        }
    }
}
void rotatePoint(QPointF &point, const qreal angle);

void FreeDrawErase::drawWithCircle(QPointF &oldPos, QPointF &newPos){
    //cout << "CIRCLE " << oldPos.x() << "," << oldPos.y() << "  " << newPos.x() << "," << newPos.y() << endl;
    QPointF shift = newPos - oldPos;
    qreal angle = std::atan2(shift.y(), shift.x()) - PI/2.;
    //cout << "ANG " << angle*180./PI << endl;
    qreal rotate = -PI/(fullWidth - 1);
    //cout << "ROT " << rotate*180./PI << endl;
    QPointF addOnStart = QPointF(halfWidth, 0.);
    QPointF addOnEnd = QPointF(halfWidth, 0.);
    rotatePoint(addOnStart, angle);
    rotatePoint(addOnEnd, angle);
    //addOn.setX(addOn.x()*cos(angle) - addOn.y()*sin(angle));
    //addOn.setY(addOn.y()*cos(angle) + addOn.x()*sin(angle));
    QPoint start;
    QPoint end;
    //angle -= PI/2.;
    for(int i = 0; i < fullWidth; i++){
        rotatePoint(addOnStart, rotate);
        rotatePoint(addOnEnd, -rotate);
        start = (oldPos + addOnStart).toPoint();
        end = (newPos + addOnEnd).toPoint();
        //cout << "   " << i << " " << int(rotate*i*180./PI) << "_" << int(addOnStart.x()) << "," << int(addOnStart.y()) << "  " << int(-rotate*i*180./PI) << "_"<< int(addOnEnd.x()) << "," << int(addOnEnd.y()) << endl;
        rasterizeLine(start, end);
    }

}

void FreeDrawErase::moveItem(QPointF &oldPos, QPointF &newPos){
    if(brushType == Qt::SquareCap){
        drawWithSquare(oldPos, newPos);
    }
    else{
        drawWithCircle(oldPos, newPos);
    }


    EraseMapIterator it((*undoMap));
    while(it.hasNext()){
        it.next();
        it.key()->deletePoints(deleteList, undoMap->value(it.key()));
    }
    lastPoint = newPos.toPoint();
#ifndef TEST
    deleteList.clear();
#endif
}

