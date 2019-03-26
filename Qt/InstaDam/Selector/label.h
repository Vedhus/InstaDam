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
enum fileTypes {PROJECT, ANNOTATION};
//class SelectItem;

class Label: public QEnableSharedFromThis<Label>{
  private:
    QColor color;
    QString text;

  public:

    Label();
    Label(const QJsonObject &json, int);
    int labelId;
    void clear();

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
    void readIdpro(const QJsonObject &json);
    void readIdantn(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void writeIdantn(QJsonObject &json) const;
    QPixmap exportLabel(QSize &rect);
};

#endif // LABEL_H
