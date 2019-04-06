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
    emit filterValueChanged(selectedMask, propNum, value, thof);
}

/*!
  This slot emits the fSlider released with the selected mask and the threhold or filter type
 */
void fSlider::reemitSliderReleased() {
    emit  fSliderReleased(selectedMask, thof);
}


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
