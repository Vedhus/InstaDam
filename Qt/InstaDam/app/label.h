#ifndef LABEL_H
#define LABEL_H

#include <QColor>
#include <QJsonObject>
#include <QPixmap>

#include "freeDrawSelect.h"
#include "freeDrawErase.h"
#include "freedrawstack.h"
#include "rectangleSelect.h"
#include "ellipseSelect.h"
#include "polygonSelect.h"

enum fileTypes {PROJECT, ANNOTATION};
class Label: public QEnableSharedFromThis<Label>{
 private:
    QColor color;
    QString text;
    int labelId;

 public:
    Label();
    Label(const QJsonObject &json, int, FreeDrawStack* , bool=false);

    ~Label();
    void setId(int);
    QColor getColor() const;
    void setColor(QColor col);
    QString getText() const;
    int getId();

    void setText(const QString tx);
    void setFDSstack(FreeDrawStack*);

    QHash<int, RectangleSelect*> rectangleObjects;
    QHash<int, EllipseSelect*> ellipseObjects;
    QHash<int, PolygonSelect*> polygonObjects;
    QHash<int, FreeDrawSelect*> freeDrawObjects;
    FreeDrawSelect* mergedFDS;
    void mergeFDS(FreeDrawSelect *, int);
    FreeDrawStack* freeDrawStack;

    QVector<FreeDrawSelect*> deleteFreeDrawObjects;
    void addItem(SelectItem *item);
    void addItem(FreeDrawSelect *item);
    void addItem(RectangleSelect *item);
    void addItem(EllipseSelect *item);
    void addItem(PolygonSelect *item);
    void addItem(FreeDrawErase *item) {UNUSED(item);}
    void removeItem(const int id);
    void clear();
    void read(const QJsonObject &json);
    void readServer(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void writeIdantn(QJsonObject &json) const;
    QPixmap exportLabel(const QSize &rect) const;
    void setOpacity(int val);
    void setMaskState(int state);
    void deleteList();
    void bringToFront();
    void sendToBack();

#ifdef TEST
    friend class TestSelect;
#endif
};

#endif  // LABEL_H
