#include "label.h"


Label::Label()
{

}

Label::~Label()
{

}

QColor Label::getColor(){
    return color;
}

void Label::setColor(QColor col){
    color = col;
}

QString Label::getText(){
    return text;
}

void Label::setText(QString tx){
    text = tx;
}
