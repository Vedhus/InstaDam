#ifndef LABEL_H
#define LABEL_H

#include <QColor>
#include <QJsonObject>
#include <QPixmap>

#include "freeDrawSelect.h"
#include "freeDrawErase.h"
#include "rectangleSelect.h"
#include "ellipseSelect.h"
#include "polygonSelect.h"

//class SelectItem;
enum fileTypes {PROJECT, ANNOTATION};
class Label: public QEnableSharedFromThis<Label>{
  private:
    QColor color;
    QString text;
    int labelId;

  public:

    Label();
    Label(const QJsonObject &json, int);

    ~Label();
    void setId(int j);

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
    void clear();
    void read(const QJsonObject &json);
    //void readIdantn(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void writeIdantn(QJsonObject &json) const;
    QPixmap exportLabel(QSize &rect);
};

#endif // LABEL_H
