#include "labelButton.h"

/*!
  \class LabelButton
  \ingroup app
  \inmodule InstaDam
  \inherits QPushButton
  \brief A button for seleting the Label for annotation.

*/

/*!
  Creates an instance, associated with \a label.
*/
LabelButton::LabelButton(QSharedPointer<Label> label) : QPushButton() {
    myLabel = label;
    setCheckable(true);
    connect(this, &QPushButton::clicked, this, &LabelButton::wasClicked);
    slider = new QSlider;
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    connect(slider, SIGNAL(valueChanged(int)), this,
            SLOT(reemitValueChanged(int)));
}

/*!
  Signals the button was clicked.
*/
void LabelButton::wasClicked() {
    emit cclicked(myLabel);
}

/*!
  Signals the value changed to \a value.
*/
void LabelButton::reemitValueChanged(int value) {
    emit opacity(myLabel, value);
}

/*!
  \fn void LabelButton::cclicked(QSharedPointer<Label> label)

  Emitted when the button is clicked, giving \a label.
*/

/*!
  \fn void LabelButton::opacity(QSharedPointer<Label> label, int op)

  Emitted when the opacity slider changes, giving \a label and \a op.
*/

/*!
  \class LabelButtonFilter
  \ingroup app
  \inmodule InstaDam
  \inherits QPushButton
  \brief Button for filters.

*/

/*!
  Creates an instance based on \a label.
*/
LabelButtonFilter::LabelButtonFilter(QSharedPointer<Label> label)
    : QPushButton() {
    myLabel = label;
    setCheckable(true);
    connect(this, &QPushButton::clicked, this, &LabelButtonFilter::wasClicked);
}

/*!
  Signals the button was clicked.
*/
void LabelButtonFilter::wasClicked() {
    emit cclicked(myLabel);
}

/*!
  \fn LabelButtonFilter::cclicked(QSharedPointer<Label> label)

  This signal is emitted when a filter button is clicked giving \a label.
*/
