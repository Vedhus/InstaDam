#ifndef IDAM_LABEL
#define IDAM_LABEL
#include "label.h"
#include <fstream>
#include <QJsonArray>

using namespace std;
Label::Label()
{

}

Label::Label(const QJsonObject &json, int j){
    readIdpro(json);
    labelId = j;
}


Label::~Label()
{

}

QColor Label::getColor(){
    return color;
}

void Label::setId(int j){
    labelId = j;
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

void Label::readIdpro(const QJsonObject &json){
    clear();
    setText(json["text"].toString());
    setColor(QColor(json["red"].toInt(), json["green"].toInt(), json["blue"].toInt(), json["alpha"].toInt()));
}

void Label::clear()
{
    rectangleObjects.clear();
    ellipseObjects.clear();
    polygonObjects.clear();
    freeDrawObjects.clear();
}

void Label::readIdantn(const QJsonObject &json){
    clear();
    if(json.contains("rectangles")){
        QJsonArray rectArray = json["rectangles"].toArray();
        for(QJsonArray::iterator it = rectArray.begin(); it != rectArray.end(); ++it){
            RectangleSelect *rect = new RectangleSelect(it->toObject(), sharedFromThis());
            SelectItem::ID = max(SelectItem::ID, rect->myID);
            addItem(rect);
        }
    }
    if(json.contains("ellipses")){
        QJsonArray ellipseArray = json["ellipses"].toArray();
        for(QJsonArray::iterator it = ellipseArray.begin(); it != ellipseArray.end(); ++it){
            EllipseSelect *ellipse = new EllipseSelect(it->toObject(), sharedFromThis());
            SelectItem::ID = max(SelectItem::ID, ellipse->myID);
            addItem(ellipse);
        }
    }
    if(json.contains("polygons")){
        QJsonArray polyArray = json["polygons"].toArray();
        for(QJsonArray::iterator it = polyArray.begin(); it != polyArray.end(); ++it){
            PolygonSelect *poly = new PolygonSelect(it->toObject(), sharedFromThis());
            SelectItem::ID = max(SelectItem::ID, poly->myID);
            addItem(poly);
        }
   }

    if(json.contains("freedraw")){
        FreeDrawSelect *fd = new FreeDrawSelect(json["freedraw"].toObject(), sharedFromThis());
        SelectItem::ID = max(SelectItem::ID, fd->myID);
        addItem(fd);
    }

    qInfo("Start annotation read");
    SelectItem::ID += 1;
}


QPixmap Label::exportLabel(QSize &rect){
    QPixmap map(rect.width(), rect.height());
    QPainter *paint = new QPainter(&map);
    QBrush brush(Qt::black);
    paint->setPen(Qt::black);
    paint->setBrush(brush);
    paint->fillRect(0.,0.,rect.width(), rect.height(), Qt::white);
    if(!rectangleObjects.isEmpty()){
        QHashIterator<int, RectangleSelect*> rit(rectangleObjects);
        while(rit.hasNext()){
            rit.next();
            if(!rit.value()->isVisible())
                continue;
            QRectF r = rit.value()->getRect();
            paint->translate(r.center());
            paint->rotate(rit.value()->getRotationAngle());
            paint->translate(-r.center());
            paint->drawRect(r);
            paint->resetTransform();
        }
    }
    if(!ellipseObjects.isEmpty()){
        QHashIterator<int, EllipseSelect*> eit(ellipseObjects);
        while(eit.hasNext()){
            eit.next();
            if(!eit.value()->isVisible())
                continue;
            QRectF r = eit.value()->getRect();
            paint->translate(r.center());
            paint->rotate(eit.value()->getRotationAngle());
            paint->translate(-r.center());
            paint->drawEllipse(r);
            paint->resetTransform();
        }
    }
    if(!polygonObjects.isEmpty()){
        QHashIterator<int, PolygonSelect*> pit(polygonObjects);
        while(pit.hasNext()){
            pit.next();
            if(!pit.value()->isVisible())
                continue;
            paint->drawPolygon(pit.value()->getPolygon());
        }
    }
    if(!freeDrawObjects.isEmpty()){
        QHashIterator<int, FreeDrawSelect*> fit(freeDrawObjects);
        while(fit.hasNext()){
            fit.next();
            if(!fit.value()->isVisible())
                continue;
            paint->drawPoints(fit.value()->getPoints());
        }

    }
    delete paint;
    return map;
}

void Label::write(QJsonObject &json) const{
    json["text"] = text;
    json["red"] = color.red();
    json["green"] = color.green();
    json["blue"] = color.blue();
    json["alpha"] = color.alpha();
}
void Label::writeIdantn(QJsonObject &json) const{
    if(!rectangleObjects.isEmpty()){
        QJsonArray rectangles;
        QHashIterator<int, RectangleSelect*> rit(rectangleObjects);
        while(rit.hasNext()){
            rit.next();
            if(!rit.value()->isVisible())
                continue;
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
            if(!eit.value()->isVisible())
                continue;
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
            if(!pit.value()->isVisible())
                continue;
            QJsonObject poly;
            pit.value()->write(poly);
            polygons.append(poly);
        }
        json["polygons"] = polygons;
    }
    if(!freeDrawObjects.isEmpty()){
        QList<FreeDrawSelect*> items;
        QVector<FreeDrawSelect*> it = freeDrawObjects.values().toVector();
        for(int i = 0; i < it.size(); i++){
            if(it[i]->isVisible())
                items.push_back(it[i]);
        }

        FreeDrawSelect *item = new FreeDrawSelect(items);
        QJsonObject fdo;
        item->write(fdo);
        json["freedraw"] = fdo;
    }
}
#endif
