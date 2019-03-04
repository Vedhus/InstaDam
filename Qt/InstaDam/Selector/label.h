#ifndef LABEL_H
#define LABEL_H

#include <QColor>
#include "selectItem.h"
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

    QHash<int, SelectItem*> rectangleObjects;
    QHash<int, SelectItem*> ellipseObjects;
    QHash<int, SelectItem*> polygonObjects;
    QHash<int, SelectItem*> freeDrawObjects;

    void addItem(SelectItem *item);
    void removeItem(const int id);
};

#endif // LABEL_H
