#include "fslider.h"


fSlider::fSlider(maskTypes maskType, int propNums, threshold_or_filter tf, QWidget *parent)
    : QSlider(parent)
{
    selectedMask = maskType;
    propNum = propNums;
    thof = tf;
    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(reemitValueChanged(int)));
    connect(this, SIGNAL(sliderReleased()),
            this, SLOT(reemitSliderReleased()));

}

void fSlider::reemitValueChanged(int value)
{
    emit filterValueChanged(selectedMask, propNum, value, thof);
}

void fSlider::reemitSliderReleased()
{
    emit  fSliderReleased(selectedMask,thof);
}



fSpinBox::fSpinBox(maskTypes maskType, int propNums,threshold_or_filter tf,  QWidget *parent)
    : QSpinBox(parent)
{
    selectedMask = maskType;
    propNum = propNums;
    thof = tf;
    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(reemitValueChanged(int)));


}
void fSpinBox::reemitValueChanged(int value)
{
    emit filterValueChanged(selectedMask, propNum, value, thof);
    emit fSlotChanged(selectedMask, thof);
}
\


fCheckBox::fCheckBox(maskTypes maskType, int propNums,threshold_or_filter tf,  QWidget *parent)
    : QCheckBox(parent)
{
    selectedMask = maskType;
    propNum = propNums;
    thof = tf;
    connect(this, SIGNAL(stateChanged(int)),
            this, SLOT(reemitStateChanged(int)));


}
void fCheckBox::reemitStateChanged(int value)
{
    emit filterValueChanged(selectedMask, propNum, value, thof);
    emit fStateChanged(selectedMask, thof);
}
