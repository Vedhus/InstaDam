#include "colordialog.h"

ColorDialog::ColorDialog(const QColor &initial, QWidget *parent)
    : QColorDialog(initial, parent) {
    resize(600, 400);
}
