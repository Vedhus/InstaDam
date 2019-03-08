#include "paintbrush.h"

paintBrush::paintBrush()
{

    size = 10;
    capStyle = Qt::RoundCap;
    color = QColor(255,255,255,255);
    brushType = ERASER;
    selectedLabelClass = 0;

}
