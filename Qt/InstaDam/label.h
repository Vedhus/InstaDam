#ifndef LABEL_H
#define LABEL_H

#include <QColor>

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
};

#endif // LABEL_H
