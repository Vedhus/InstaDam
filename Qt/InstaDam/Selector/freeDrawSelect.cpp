#include "freeDrawSelect.h"
#include <QPainter>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>
using namespace std;
FreeDrawSelect::FreeDrawSelect(QPointF point, QGraphicsItem *item)
    : QAbstractGraphicsShapeItem(item), SelectItem(item){
    //QPointF pnt = QPointF(int(point.x()), int(point.y()));
    polygon << point.toPoint();
    offset = point;
    //gridSize = QPointF(scene()->width(), scene()->height());
    //addBlock(point);

    setActiveVertex(0);
    myRect = QRectF(point, point).adjusted(-2.5,-2.5,2.5,2.5);
    mytype = Freedraw;
    active = true;

    QPen pen(Qt::green);
    pen.setWidth(5);
    myPen = pen;
    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsSelectable);
    QAbstractGraphicsShapeItem::setFlag(QGraphicsItem::ItemIsMovable);

}

QPoint FreeDrawSelect::getBox(QPoint &point){
    return QPoint(int(std::floor(point.x()/baseSize)), int(std::floor(point.y()/baseSize)));
}

void FreeDrawSelect::updateMap(QPoint &point){
    //QPoint currentBox = getBox(point);
    int x = int(std::floor(point.x()/baseSize));
    int y = int(std::floor(point.y()/baseSize));
    if(!myMap[x][y]){
        myMap[x][y] = makeBlock();
    }
    int xx = point.x()%baseSize;
    int yy = point.y()%baseSize;
    (*myMap[x][y])[xx][yy] = true;
}

void FreeDrawSelect::init(QPointF &point){
    //myMap = makeBlock();
    gridSize = QPoint(int(std::floor(scene()->width()/baseSize)) + 1, int(std::floor(scene()->height()/baseSize)) + 1);
    //inUse = Block(gridSize.x(), QVector <bool>(gridSize.y(), false));
    myMap = QVector<QVector <Block*> >(gridSize.x(), QVector <Block*>(gridSize.y(), nullptr));
    QPoint newPoint = point.toPoint();
    updateMap(newPoint);
    //QPoint currentBox = getBox(newPoint);
    //inUse[currentBox.x()][currentBox.y()] = true;
    //Block *temp = makeBlock();
    //(*temp)[currentBox.x()][currentBox.y()] = true;
    //myMap[currentBox.x()][currentBox.y()] = temp;
}

Block* FreeDrawSelect::makeBlock(){
    Block *block = new Block;
    block->resize(100);
    for(int i = 0; i < baseSize; i++){
        (*block)[i] = baseVector;
    }
    return block;
}

QVector<QPoint> FreeDrawSelect::rasterizeLine(QPoint &start, QPoint &end){
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();
    qreal step;
    qreal x, y;
    QVector<QPoint> points = QVector<QPoint>();
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
        points.push_back(QPointF(x, y).toPoint());
        x += dx;
        y += dy;
    }
    return points;
}

//void updateCorner(QPointF point);
void FreeDrawSelect::addPoint(QPointF &point, int vertex ){
    cout << "NEW POINT" << endl;
    QAbstractGraphicsShapeItem::prepareGeometryChange();

    myRect = this->boundingRect();
}

QRectF FreeDrawSelect::boundingRect() const{
    return myRect.adjusted(-5, -5, 5, 5);
}

QGraphicsScene* FreeDrawSelect::scene(){
    return SelectItem::scene();
}

void FreeDrawSelect::checkPoint(QPointF &point){
    point.setX(std::min(std::max(0., point.x()), scene()->width()));
    point.setY(std::min(std::max(0., point.y()), scene()->height()));
}

void FreeDrawSelect::checkPoint(QPoint &point){
    point.setX(std::min(std::max(0, point.x()), int(scene()->width())));
    point.setY(std::min(std::max(0, point.y()), int(scene()->height())));
}

void FreeDrawSelect::moveItem(QPointF &oldPos, QPointF &newPos){
    cout << "MOVE" << endl;
    QPoint start = oldPos.toPoint();
    QPoint end = newPos.toPoint();
    int dx, dy;
    if(start.x() <= end.x()){
        start.rx() -= 2;
        end.rx() += 2.;
        dx = 1;
    }
    else{
        start.rx() += 2;
        end.rx() -= 2;
        dx = -1;
    }
    if(start.y() <= end.y()){
        start.ry() -= 2;
        start.ry() += 2;
        dy = 1;
    }
    else{
        start.ry() += 2;
        start.ry() -= 2;
        dy = -1;
    }
    checkPoint(start);
    checkPoint(end);
    QVector<QPoint> points = QVector<QPoint>();

    for(int i = 0; i < pixel; i++){
        points.append(rasterizeLine(start, end));
        start.rx() += dx;
        end.rx() += dx;
    }

    for(int i = 1; i < pixel; i++){
        start.ry() += dy;
        end.ry() += dy;
        points.append(rasterizeLine(start, end));
    }
    for(QVector<QPoint>::iterator it = points.begin(); it != points.end(); ++it){
        updateMap((*it));
    }
    polygon.append(points);
}


void FreeDrawSelect::resizeItem(int vertex, QPointF &point){

}

void FreeDrawSelect::clickPoint(QPointF &point){
    active = true;
    cout << "CLICK" << endl;
    activeVertex = UNSELECTED;
}

bool FreeDrawSelect::isInside(QPointF &point){
    return false;
}

void FreeDrawSelect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //QAbstractGraphicsShapeItem::paint(painter, option, widget);
    cout << "PAINT" << endl;
    painter->setPen(myPen);
    painter->setBrush(brush());
    for(QPolygon::iterator it = polygon.begin() ; it != polygon.end(); ++it){
        cout << (*it).x() << "  " << (*it).y() << endl;
        //painter->drawPoint((*it));
    }
    painter->drawPoints(polygon);
    //}
}
