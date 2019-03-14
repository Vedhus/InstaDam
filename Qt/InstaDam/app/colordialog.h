#ifndef COLORDIALOG_H
#define COLORDIALOG_H
#include <QColorDialog>

class ColorDialog : public QColorDialog{
public:
    ColorDialog(const QColor &initial, QWidget *parent = nullptr);
};

#endif // COLORDIALOG_H
