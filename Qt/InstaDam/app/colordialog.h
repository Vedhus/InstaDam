#ifndef COLORDIALOG_H
#define COLORDIALOG_H
#include <QColorDialog>

class ColorDialog : public QColorDialog{
 public:
    explicit ColorDialog(const QColor &initial, QWidget *parent = nullptr);
};

#endif  // COLORDIALOG_H
