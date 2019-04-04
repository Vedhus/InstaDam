#include "colordialog.h"

/*!
 * \brief ColorDialog::ColorDialog creates a QCOlorDialog to select label colors
 * \param initial
 * \param parent
 */
ColorDialog::ColorDialog(const QColor &initial, QWidget *parent)
    : QColorDialog(initial, parent) {
    resize(600, 400);
}
