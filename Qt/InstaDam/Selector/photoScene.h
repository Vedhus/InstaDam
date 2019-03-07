
#ifndef PHOTOSCENE_H
#define PHOTOSCENE_H

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

enum viewerTypes{PHOTO_VIEWER_TYPE, MASK_VIEWER_TYPE};

class PhotoScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PhotoScene(viewerTypes type, QObject *parent = nullptr);
    SelectItem* itemAt(QPointF point);
    SelectItem* itemAt(QPointF point, std::string label);
    void addLabel(std::string label);
    void setCurrentLabel(std::string label);
    void addLabelItem(SelectItem* item, std::string label);
    void addItem(SelectItem* item);
    void addItem(QGraphicsItem* item);
    void inactiveAll();
    viewerTypes myViewerType;

signals:
    void pointClicked(viewerTypes type, SelectItem* item, QPointF point, const Qt::MouseButton button);
    void mouseMoved(QPointF fromPos, QPointF toPos);
    void mouseReleased(viewerTypes type, QPointF oldPos, QPointF newPos, const Qt::MouseButton button);
    void keyPressed(viewerTypes type, const int key);

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


#endif

