#ifndef IDAM_LABEL
#define IDAM_LABEL
#include "label.h"
#include <iostream>
#include <fstream>
#include <QJsonArray>
using namespace std;
Label::Label()
{

}

Label::Label(const QJsonObject &json){
    read(json);
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

void Label::read(const QJsonObject &json){
    rectangleObjects.clear();
    ellipseObjects.clear();
    polygonObjects.clear();
    freeDrawObjects.clear();
    setText(json["text"].toString());
    setColor(QColor(json["red"].toInt(), json["green"].toInt(), json["blue"].toInt(), json["alpha"].toInt()));

    if(json.contains("rectangles")){
        QJsonArray rectArray = json["rectangles"].toArray();
        for(QJsonArray::iterator it = rectArray.begin(); it != rectArray.end(); ++it){
            RectangleSelect *rect = new RectangleSelect(it->toObject(), this);
            addItem(rect);
        }
    }
    if(json.contains("ellipses")){
        QJsonArray ellipseArray = json["ellipses"].toArray();
        for(QJsonArray::iterator it = ellipseArray.begin(); it != ellipseArray.end(); ++it){
            EllipseSelect *ellipse = new EllipseSelect(it->toObject(), this);
            addItem(ellipse);
        }
    }
    if(json.contains("polygons")){
        QJsonArray polyArray = json["polygons"].toArray();
        for(QJsonArray::iterator it = polyArray.begin(); it != polyArray.end(); ++it){
            PolygonSelect *poly = new PolygonSelect(it->toObject(), this);
            addItem(poly);
        }
    }
    if(json.contains("freedraw")){
        FreeDrawSelect *fd = new FreeDrawSelect(json["freedraw"].toObject(), this);
        addItem(fd);
    }
    setText(json["text"].toString());
    setColor(QColor(json["red"].toInt(), json["green"].toInt(), json["blue"].toInt(), json["alpha"].toInt()));
}

void Label::write(QJsonObject &json) const{
    json["text"] = text;
    json["red"] = color.red();
    json["green"] = color.green();
    json["blue"] = color.blue();
    json["alpha"] = color.alpha();
    if(!rectangleObjects.isEmpty()){
        QJsonArray rectangles;
        QHashIterator<int, RectangleSelect*> rit(rectangleObjects);
        while(rit.hasNext()){
            rit.next();
            QJsonObject rect;
            rit.value()->write(rect);
            rectangles.append(rect);
        }
        json["rectangles"] = rectangles;
    }
    if(!ellipseObjects.isEmpty()){
        QJsonArray ellipses;
        QHashIterator<int, EllipseSelect*> eit(ellipseObjects);
        while(eit.hasNext()){
            eit.next();
            QJsonObject ellipse;
            eit.value()->write(ellipse);
            ellipses.append(ellipse);
        }
        json["ellipses"] = ellipses;
    }
    if(!polygonObjects.isEmpty()){
        QJsonArray polygons;
        QHashIterator<int, PolygonSelect*> pit(polygonObjects);
        while(pit.hasNext()){
            pit.next();
            QJsonObject poly;
            pit.value()->write(poly);
            polygons.append(poly);
        }
        json["polygons"] = polygons;
    }
    if(!freeDrawObjects.isEmpty()){
        //QList<FreeDrawSelect*> items = freeDrawObjects.values();
        FreeDrawSelect *item = new FreeDrawSelect(freeDrawObjects.values());
        QJsonObject fdo;
        item->write(fdo);
        json["freedraw"] = fdo;
    }
}
#endif
