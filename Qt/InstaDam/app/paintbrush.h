#ifndef PAINTBRUSH_H
#define PAINTBRUSH_H
#include <QColor>

enum toolTypes {PAINTBRUSH, ERASER, PAN};

class paintBrush
{
public:
    paintBrush();
    int size;
    Qt::PenCapStyle capStyle;
    QColor color;
    toolTypes brushType;
    int selectedLabelClass ;
};

#endif // PAINTBRUSH_H
