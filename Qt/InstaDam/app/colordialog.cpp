#include "colordialog.h"

/*!
  \class ColorDialog
  \ingroup InstaDam
  \inmodule InstaDam
  \inherits QColorDialog
  \brief Creates a QCOlorDialog to select label colors with an initial widget
         size.

  Creates a QCOlorDialog to select label colors with an initial widget size.
  This is specifically used for the QtWebAssembly build as that infrastructure
  requires a specified size for all QDialogs.
  */
/*!
  Constructs a color selection dialog with a hard coded size, initial QColor
  selection of \a color, and parent QWidget \a parent, if any.
 */
ColorDialog::ColorDialog(const QColor &initial, QWidget *parent)
    : QColorDialog(initial, parent) {
    resize(600, 400);
}
