
#ifndef PHOTOSCENE_H
#define PHOTOSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <list>
#include <map>
#include <string>

#include "selectItem.h"
#include "ellipseSelect.h"
#include "rectangleSelect.h"
#include "polygonSelect.h"

//class SelectItem;
//QT_BEGIN_NAMESPACE
//class QGraphicsSceneDragDropEvent;
//class QGraphicsViewItem;
//QT_END_NAMESPACE


class PhotoScene : public QGraphicsScene {
    Q_OBJECT

 public:
    enum viewerTypes{
        PHOTO_VIEWER_TYPE,
        MASK_VIEWER_TYPE};

    explicit PhotoScene(viewerTypes type, QObject *parent = nullptr);

    SelectItem* itemAt(QPointF point) const;
    SelectItem* itemAt(QPointF point, std::string label);
    void addLabel(std::string label);
    void setCurrentLabel(const std::string &label);
    void addLabelItem(SelectItem* item, std::string label);
    void addItem(SelectItem* item);
    void addItem(QGraphicsItem* item);
    void inactiveAll() const;
    viewerTypes myViewerType;
    void removeItem(SelectItem* item);
    void clearItems();
    void permanentRemove(SelectItem* );


#ifdef TEST
    friend class TestSelect;
    friend class IntegrationTest;
#endif
 signals:
    void pointClicked(const PhotoScene::viewerTypes type, SelectItem* item,
                      const QPointF point, const Qt::MouseButton button,
                      const Qt::KeyboardModifiers modifiers);
    void mouseMoved(const QPointF fromPos, const QPointF toPos,
                    const Qt::KeyboardModifiers modifiers);
    void mouseReleased(const PhotoScene::viewerTypes type, const QPointF oldPos,
                       const QPointF newPos, const Qt::MouseButton button,
                       const Qt::KeyboardModifiers modifiers);
    void keyPressed(PhotoScene::viewerTypes type, const int key);
    void keyReleased(PhotoScene::viewerTypes type, const int key);

 protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

 private:
    QGraphicsItem *movingItem;
    QPointF oldPos;
    QPointF newPos;
    std::string currentLabel;
    bool mousePressed;
    QList<SelectItem* > labelItems;
    std::list<SelectItem*> currentItems;
    std::map<std::string, std::list<SelectItem*> > labelmap;
};


#endif

