#ifndef LABELBUTTON_H
#define LABELBUTTON_H
#include <QPushButton>
#include <QSlider>
#include "../Selector/label.h"

class LabelButton : public QPushButton{
    Q_OBJECT
public:
    LabelButton(QSharedPointer<Label> label);
    QSharedPointer<Label> myLabel;
    QSlider *slider;
signals:
    void cclicked(QSharedPointer<Label> label);
    void opacity(QSharedPointer<Label> label, int);
public slots:
    void wasClicked();
    void reemitValueChanged(int);
};


#endif // LABELBUTTON_H
