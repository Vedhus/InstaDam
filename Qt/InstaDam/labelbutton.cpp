

#include "labelbutton.h"

labelButton::labelButton(int i)
{
    labelNum =i;
    connect(this, SIGNAL(clicked()), this, SLOT(reemitClicked()));
    slider = new QSlider;
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(reemitValueChanged(int)));

}
void labelButton::reemitClicked()
{
    emit lClicked(labelNum, palette().color(QPalette::Button));
}
void labelButton::reemitValueChanged(int value)
{
    emit opacity(labelNum, value);
}





