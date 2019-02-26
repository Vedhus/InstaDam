/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   diagramscene.h
 * Author: friedel
 *
 * Created on February 6, 2019, 9:00 PM
 */

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <list>
//#include <bits/stdc++.h>
#include <map>
#include "selectItem.h"
#include "ellipseSelect.h"
#include "rectangleSelect.h"
#include "polygonSelect.h"

class SelectItem;
QT_BEGIN_NAMESPACE
class QGraphicsSceneDragDropEvent;
class QGraphicsViewItem;
QT_END_NAMESPACE
class PhotoScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PhotoScene(QObject *parent = nullptr);
    //void setCurrentItem(RectItem *rectItem);
    //void setCurrentType(SelectType newType);
    SelectItem* itemAt(QPointF point);
    SelectItem* itemAt(QPointF point, std::string label);
    void addLabel(std::string label);
    void setCurrentLabel(std::string label);
    void addLabelItem(SelectItem* item, std::string label);
    void addItem(SelectItem* item);
    void addItem(QGraphicsItem* item);
    void inactiveAll();

signals:
    void pointClicked(SelectItem* item, QPointF point);
    void mouseMoved(QPointF fromPos, QPointF toPos);
    void leftMouseReleased(QPointF oldPos, QPointF newPos);
    void keyPressed(const int key);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QGraphicsItem *movingItem;
    QPointF oldPos;
    QPointF newPos;
    std::string currentLabel;
    bool mousePressed;
    std::list<SelectItem*> currentItems;
    std::map<std::string, std::list<SelectItem*> > labelmap;
};


#endif /* DIAGRAMSCENE_H */

