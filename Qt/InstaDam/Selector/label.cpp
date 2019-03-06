#ifndef IDAM_LABEL
#define IDAM_LABEL
#include "label.h"
#include <iostream>
#include <fstream>
using namespace std;
Label::Label()
{

}

Label::~Label()
{

}

QColor Label::getColor(){
    return color;
}

void Label::setColor(QColor col){
    color = col;
}

QString Label::getText(){
    return text;
}

void Label::setText(QString tx){
    text = tx;
}

/*void Label::addItem(SelectItem *item){
    switch(item->type()){
        case Rect:
            rectangleObjects.insert(item->ID, item);
            break;
        case Ellipse:
            ellipseObjects.insert(item->ID, item);
            break;
        case Polygon:
            polygonObjects.insert(item->ID, item);
            break;
        case Freedraw:
            freeDrawObjects.insert(item->ID, item);
            break;
    }
    //item->setLabel(this);
}*/
void Label::addItem(RectangleSelect *item){
    rectangleObjects.insert(item->myID, item);
}

void Label::addItem(EllipseSelect *item){
    ellipseObjects.insert(item->myID, item);
}

void Label::addItem(PolygonSelect *item){
    polygonObjects.insert(item->myID, item);
}

void Label::addItem(FreeDrawSelect *item){
    freeDrawObjects.insert(item->myID, item);
}


void Label::removeItem(const int id){
    if(rectangleObjects.remove(id) != 0){
        if(ellipseObjects.remove(id) != 0){
            if(polygonObjects.remove(id) != 0){
                freeDrawObjects.remove(id);
            }
        }
    }
}
#endif
