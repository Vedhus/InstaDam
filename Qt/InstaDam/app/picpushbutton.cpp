#include "picpushbutton.h"
#include "pixmapops.h"

PicPushButton::PicPushButton(QWidget *parent):QLabel(parent) {
    pixmap = testPixmap(255, 0, 0, 255);
    this->setPixmap(pixmap);
    updatePixmap();
}

void PicPushButton::updatePixmap() {
    this->setPixmap(pixmap);
    resetPixmaps(pixmap);
}

void PicPushButton::setMaskType(maskTypes maskType) {
    filter = maskType;
}

void PicPushButton::manualSelect() {
    check = true;
    repaint();
}

void PicPushButton::resetPixmaps(QPixmap pixmap_input) {
    pixmap_hover = testPixmap(20, 125, 200, 128);
    pixmap_pressed = testPixmap(30, 180, 200, 128);
    pixmap = pixmap_input;
    pixmap_hover = joinPixmaps(pixmap, pixmap_hover,
                               QPainter::CompositionMode_SourceOver);
    pixmap_pressed = joinPixmaps(pixmap, pixmap_pressed,
                                 QPainter::CompositionMode_SourceOver);
}

QPixmap PicPushButton::addText(QPixmap pixmap_input, QString text) {
    QPainter painter_temp(&pixmap_input);
    QFont font = painter_temp.font();
    font.setPointSize(24);
    QRect position  = QRect(0, 0, w, h);
    painter_temp.setFont(font);
    painter_temp.drawText(position, Qt::AlignCenter, "text");
    painter_temp.end();
    return pixmap_input;
}

void PicPushButton::paintEvent(QPaintEvent *event) {
    QPixmap pix;
    if (underMouse())
        pix = pixmap_hover;
    else
        pix = pixmap;

    if (check) {
        pix = pixmap_pressed;
    }
    this->setPixmap(pix);

    QLabel::paintEvent(event);
}


void PicPushButton::mousePressEvent(QMouseEvent* event) {
    check = true;
    emit checked(filter);
}

void PicPushButton::otherBoxChecked(maskTypes type) {
    if (check) {
        check = false;
        this->setPixmap(pixmap);
    }
}
