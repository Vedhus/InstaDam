#include "chooselabeldialog.h"
#include <QVBoxLayout>
#include <QMouseEvent>
/*!
  \class chooseLabelDialog
  \ingroup app
  \inmodule InstaDam
  \inherits QDialog
  \brief Defines a custom QDialog to pick labels from.

*/

/*!
  Creates a chooseLabelDialog instance with the given \a currentPro.
*/
chooseLabelDialog::chooseLabelDialog(Project *currentPro) : QDialog() {
    setWindowFlags(Qt::Popup);
    QVBoxLayout *VBlayout = new QVBoxLayout(this);
    Project* currentProject = currentPro;
    for (int i=0; i < currentProject->numLabels(); i++) {
        QSharedPointer<Label> label = currentProject->getLabel(i);
        LabelButtonFilter *button = new LabelButtonFilter(label);
        button->setText(label->getText());
        QPalette pal = button->palette();
        pal.setColor(QPalette::ButtonText, Qt::black);
        pal.setColor(QPalette::Button, label->getColor());
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        VBlayout->addWidget(button);

        connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), this,
                SLOT(checkLabel(QSharedPointer<Label>)));
        labelButtons.push_back(button);
    }
    QDialog::show();
}

/*!
  Called when a mouse button is pressed with the given \a event.
*/
void chooseLabelDialog::mousePressEvent(QMouseEvent *event){
    mpos = event->pos();
    QDialog::mousePressEvent(event);
}

/*!
  Called when the mouse moves with the given \a event.
*/
void chooseLabelDialog::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;

        this->move(newpos);
    }
    QDialog::mouseMoveEvent(event);

}

/*!
  Called when a label button is checked, given by \a label.
*/
void chooseLabelDialog::checkLabel(QSharedPointer<Label> label) {
    for (int i = 0; i < labelButtons.size(); i++) {
        if (label != labelButtons[i]->myLabel) {
            labelButtons[i]->setChecked(false);
        } else {
            labelButtons[i]->setChecked(true);
        }
    }
    emit labelPicked(label);
}

/*!
  \fn chooseLabelDialog::labelPicked(QSharedPointer<Label> label)

  This signal is emitted when \a label is picked.
*/
