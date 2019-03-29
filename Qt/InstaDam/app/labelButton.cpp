#include "labelButton.h"

LabelButton::LabelButton(QSharedPointer<Label> label) : QPushButton(){
    myLabel = label;
    setCheckable(true);
    connect(this, &QPushButton::clicked, this, &LabelButton::wasClicked );
    slider = new QSlider;
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(reemitValueChanged(int)));
}

void LabelButton::wasClicked(){
    emit cclicked(myLabel);
}

void LabelButton::reemitValueChanged(int value)
{
    emit opacity(myLabel, value);
}



