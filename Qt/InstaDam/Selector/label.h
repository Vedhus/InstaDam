#ifndef LABEL_H
#define LABEL_H

#include <QColor>
#include "freeDrawSelect.h"
#include "freeDrawErase.h"
#include "rectangleSelect.h"
#include "ellipseSelect.h"
#include "polygonSelect.h"

//class SelectItem;

class Label{
  private:
    QColor color;
    QString text;

  public:

    Label();

    ~Label();

    QColor getColor();

    void setColor(QColor col);
    QString getText();

    void setText(QString tx);

    QHash<int, RectangleSelect*> rectangleObjects;
    QHash<int, EllipseSelect*> ellipseObjects;
    QHash<int, PolygonSelect*> polygonObjects;
    QHash<int, FreeDrawSelect*> freeDrawObjects;

    void addItem(FreeDrawSelect *item);
    void addItem(RectangleSelect *item);
    void addItem(EllipseSelect *item);
    void addItem(PolygonSelect *item);
    void addItem(FreeDrawErase *item){UNUSED(item);}
    void removeItem(const int id);
};

#endif // LABEL_H