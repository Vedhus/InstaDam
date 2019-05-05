#include "fslider.h"
#include "enumconstants.h"
/*!
  \class fSlider
  \ingroup app
  \inmodule InstaDam
  \inherits QSlider
  \brief Slider widget for filterDialog.

*/

/*!
  Creates an instance based on \a maskType, \a propNums, \a tf, and parent
  QWidget \a parent, if any.
*/
fSlider::fSlider(EnumConstants::maskTypes maskType, int propNums,
                 EnumConstants::threshold_or_filter tf, QWidget *parent)
    : QSlider(parent) {
    selectedMask = maskType;
    propNum = propNums;
    thof = tf;
    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(reemitValueChanged(int)));
    connect(this, SIGNAL(sliderReleased()),
            this, SLOT(reemitSliderReleased()));
}

/*!
  This slot reemits the \a value through the filterValueChanged signal
  along with other information like the selected mask, property number and the threhold or filter type
*/
void fSlider::reemitValueChanged(int value) {

}

/*!
  This slot emits the fSlider released with the selected mask and the threhold or filter type
*/
void fSlider::reemitSliderReleased() {
    emit  fSliderReleased(selectedMask, thof);
    emit filterValueChanged(selectedMask, propNum, value(), thof);
}

/*!
  This slot is called when the value of the slider is changed from outside(
  */
void fSlider::sliderChangedByCall(EnumConstants::maskTypes maskType, int propNums,
                                 int value){
    if( selectedMask == maskType && propNum == propNums)
    {
        setValue(value);
        emit sliderReleased();
    }

}

/*!
  \fn void fSlider::filterValueChanged(EnumConstants::maskTypes selectedMask, int propNum, int value, EnumConstants::threshold_or_filter thof)

  This signal is emitted when the filter value changes with \a selectedMask,
  \a propNum, \a value, and \a thof.
*/

/*!
  \fn void fSlider::fSliderReleased(EnumConstants::maskTypes selectedMask, EnumConstants::threshold_or_filter tf)

  This signal is emitted when the state changes giving \a selectedMask and \a tf.
*/

/*!
  \class fSpinBox
  \ingroup app
  \inmodule InstaDam
  \inherits QSlider
  \brief Spinbox widget for filterDialog.

*/

/*!
  Creates an instance based on \a maskType, \a propNums, \a tf, and parent
  QWidget \a parent, if any.
*/
fSpinBox::fSpinBox(EnumConstants::maskTypes maskType, int propNums,
                   EnumConstants::threshold_or_filter tf, QWidget *parent)
    : QSpinBox(parent) {
    selectedMask = maskType;
    propNum = propNums;
    thof = tf;
    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(reemitValueChanged(int)));
}

/*!
  This slot reemits the \a value through the filterValueChanged signal
  along with other information like the selected mask, property number and the threhold or filter type
*/
void fSpinBox::reemitValueChanged(int value) {
    emit filterValueChanged(selectedMask, propNum, value, thof);
    emit fSlotChanged(selectedMask, thof);
}

/*!
  Sets the display value to \a value.
*/
void fSpinBox::displayValue(int value) {
    blockSignals(true);
    setValue(value);
    blockSignals(false);
}

/*!
  \fn fSpinBox::filterValueChanged(EnumConstants::maskTypes selectedMask, int propNum, int value, EnumConstants::threshold_or_filter tf)

  This signal is emitted when the filter value changes with \a selectedMask, \a propNum, \a value, and \a tf.
*/

/*!
  \fn fSpinBox::fSlotChanged(EnumConstants::maskTypes selectedMask, EnumConstants::threshold_or_filter tf)

  This signal is emitted when the state changes giving \a selectedMask and \a tf.
*/


/*!
  \class fCheckBox
  \ingroup app
  \inmodule InstaDam
  \inherits QSlider
  \brief Checbox widget for filterDialog.

*/

/*!
  Creates an instance based on \a maskType, \a propNums, \a tf, and parent
  QWidget \a parent, if any.
*/
fCheckBox::fCheckBox(EnumConstants::maskTypes maskType, int propNums,
                     EnumConstants::threshold_or_filter tf, QWidget *parent)
    : QCheckBox(parent) {
    selectedMask = maskType;
    propNum = propNums;
    thof = tf;
    connect(this, SIGNAL(stateChanged(int)),
            this, SLOT(reemitStateChanged(int)));
}

/*!
  This slot reemits the \a value through the filterValueChanged signal
  along with other information like the selected mask, property number and the threhold or filter type
*/
void fCheckBox::reemitStateChanged(int value) {
    emit filterValueChanged(selectedMask, propNum, value, thof);
    emit fStateChanged(selectedMask, thof);
}

/*!
  \fn fCheckBox::filterValueChanged(EnumConstants::maskTypes selectedMask, int propNum, int value, EnumConstants::threshold_or_filter tf)

  This signal is emitted when the filter value changes with \a selectedMask, \a propNum, \a value, and \a tf.
*/

/*!
  \fn fCheckBox::fStateChanged(EnumConstants::maskTypes selectedMask, EnumConstants::threshold_or_filter tf)

  This signal is emitted when the state changes giving \a selectedMask and \a tf.
*/
