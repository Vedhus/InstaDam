#include "labelButton.h"

LabelButton::LabelButton(QSharedPointer<Label> label) : QPushButton(){
    myLabel = label;
    connect(this, &QPushButton::clicked, this, &LabelButton::wasClicked );
}

void LabelButton::wasClicked(){
    emit cclicked(myLabel);
}
