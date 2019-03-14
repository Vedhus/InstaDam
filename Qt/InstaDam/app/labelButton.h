#ifndef LABELBUTTON_H
#define LABELBUTTON_H
#include <QPushButton>
#include "../Selector/label.h"

class LabelButton : public QPushButton{
    Q_OBJECT
public:
    LabelButton(QSharedPointer<Label> label);
    QSharedPointer<Label> myLabel;
signals:
    void cclicked(QSharedPointer<Label> label);
public slots:
    void wasClicked();

};

#endif // LABELBUTTON_H
