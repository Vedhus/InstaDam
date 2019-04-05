#include "labelButton.h"

/*!
  \class LabelButton
  \ingroup InstaDam
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
  \class LabelButtonFilter
  \ingroup InstaDam
  \inmodule InstaDam
  \inherits QPushButton
  \brief Button for filters
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
