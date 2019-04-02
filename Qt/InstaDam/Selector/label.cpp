#ifndef IDAM_LABEL
#define IDAM_LABEL
#include "label.h"
#include <fstream>
#include <QJsonArray>

using namespace std;

/*!
  \class Label
  \ingroup Selector
  \inmodule InstaDam
  \inherits QEnableSharedFromThis
  \brief The Label class provides a class for holding SelectItems.

  Provides a class for holing SelectItems which annotate the same type of feature.
  */

/*!
  Default constructor
  */
Label::Label()
{

}

/*!
  Constructs a Label object based on data read in from \a json. The Label color and
  text are read from the QJsonObject and any SelectItems in the object are
  constructed and added to this Label.
  */
Label::Label(const QJsonObject &json, int j, bool server){
        if(server)
            readServer(json);
        else {
             read(json);
        }
        labelId = j;
}

/*!
  Destructor
  */
Label::~Label()
{

}

/*!
  Convenience function which returns the QColor of this Label.
  */
QColor Label::getColor(){
    return color;
}

/*!
  Convenience function for setting the color of this Label to \a col.
  */
void Label::setColor(QColor col){
    color = col;
}

/*!
  Convenience funtion for setting the id this Label as the \a labelId .
  */

void Label::setId(int j){
    labelId = j;
}


/*!
  Convenience funtion for getting the test of this Label as a QString
  */
QString Label::getText(){
    return text;
}

/*!
  Convenience funtion for setting the text of this laael to \a tx
  */
void Label::setText(QString tx){
    text = tx;
}

/*!
  \overload addItem()

  This convenience function adds a RectangleSelect object to the Label
  */
void Label::addItem(RectangleSelect *item){
    rectangleObjects.insert(item->myID, item);
}

/*!
  \overload addItem()

  This convenience function adds an EllipseSelect object to the Label

  */
void Label::addItem(EllipseSelect *item){
    ellipseObjects.insert(item->myID, item);
}

/*!
  \overload addItem()

  This convenience function adds a PolygonSelect object to the Label

  */
void Label::addItem(PolygonSelect *item){
    polygonObjects.insert(item->myID, item);
}

/*!
  \overload addItem()

  This convenience function adds a FreeDrawSelect object to the Label

  */
void Label::addItem(FreeDrawSelect *item){
    freeDrawObjects.insert(item->myID, item);
}


/*!
  Removes a SelectItem from the Label based on it's unique ID given as \a id.
  */
void Label::removeItem(const int id){
    if(rectangleObjects.remove(id) != 0){
        if(ellipseObjects.remove(id) != 0){
            if(polygonObjects.remove(id) != 0){
                freeDrawObjects.remove(id);
            }
        }
    }
}
/*!
  Clears the label data in memory.
  */
void Label::clear()
{
    rectangleObjects.clear();
    ellipseObjects.clear();
    polygonObjects.clear();
    freeDrawObjects.clear();
}

void Label::setOpacity(int val)

{

    float normalizedValue = val/100.0;
    qInfo("Opacity\n%f", normalizedValue);
    QHash<int, RectangleSelect*>::iterator rectItems;
    QHash<int, PolygonSelect*>::iterator polygonItems;
    QHash<int, FreeDrawSelect*>::iterator freeDrawItems;
    QHash<int, EllipseSelect*>::iterator ellipseItems;

    for (rectItems = rectangleObjects.begin(); rectItems != rectangleObjects.end(); ++rectItems)
        rectItems.value()->setOpacity(normalizedValue);
        qInfo("Opacity Rectangle\n");
    for (polygonItems = polygonObjects.begin(); polygonItems != polygonObjects.end(); ++polygonItems)
        polygonItems.value()->setOpacity(normalizedValue);
        qInfo("Opacity Polygon\n");
}

/*!
  Reads a QJsonObject \a json and sets the Label annotation's to the data it reads.
Any SelectItems found in the QJsonObject are also constructed and added to this
Label.
*/

void Label::read(const QJsonObject &json){
    clear();
    setText(json["text"].toString());
    setColor(QColor(json["red"].toInt(), json["green"].toInt(), json["blue"].toInt(), json["alpha"].toInt()));

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
    SelectItem::ID += 1;
}

void Label::readServer(const QJsonObject &json)
{
    clear();
    setText(json["name"].toString());
    setColor(QColor(json["color"].toString()));
}
/*!
  Creates a QPixmap bitmask based on all of the SelectItems that this Label holds.
  It paints each item based on their internal parameters. \a rect specifies the
  size of the output QPixmap.
  */
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
            rit.value()->toPixmap(paint);
        }
    }
    if(!ellipseObjects.isEmpty()){
        QHashIterator<int, EllipseSelect*> eit(ellipseObjects);
        while(eit.hasNext()){
            eit.next();
            if(!eit.value()->isVisible())
                continue;
            eit.value()->toPixmap(paint);
        }
    }
    if(!polygonObjects.isEmpty()){
        QHashIterator<int, PolygonSelect*> pit(polygonObjects);
        while(pit.hasNext()){
            pit.next();
            if(!pit.value()->isVisible())
                continue;
            pit.value()->toPixmap(paint);
        }
    }
    if(!freeDrawObjects.isEmpty()){
        QHashIterator<int, FreeDrawSelect*> fit(freeDrawObjects);
        while(fit.hasNext()){
            fit.next();
            if(!fit.value()->isVisible())
                continue;
            fit.value()->toPixmap(paint);
        }

    }
    delete paint;
    return map;
}

/*!
  Writes the contents of the Label class to QJsonObject \a json.
  */
void Label::write(QJsonObject &json) const{
    json["text"] = text;
    json["red"] = color.red();
    json["green"] = color.green();
    json["blue"] = color.blue();
    json["alpha"] = color.alpha();
}

/*!
  Writes the contents of the Label data to QJsonObject \a json.
Any SelectItems this Label holds are written.
  */
void Label::writeIdantn(QJsonObject &json) const{
    write(json);
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

/*!
  \fn Label::addItem(FreeDrawErase *item)

  Empty function as FreeDrawErase objects are not saved.
  */
#endif
