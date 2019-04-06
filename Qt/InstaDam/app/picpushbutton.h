#ifndef PICPUSHBUTTON_H
#define PICPUSHBUTTON_H

#include<QAbstractButton>
#include<QPushButton>
#include<QLabel>
#include<QPixmap>

#include "enumconstants.h"

class PicPushButton : public QLabel {
    Q_OBJECT

 public:
    int w;
    int h;
    EnumConstants::maskTypes filter = EnumConstants::CANNY;
    QPixmap pixmap;
    QPixmap pixmap_hover;
    QPixmap pixmap_pressed;
    explicit PicPushButton(QWidget *parent);
    bool check;
    void setMaskType(EnumConstants::maskTypes maskType);
    void updatePixmap();
    void resetPixmaps(QPixmap pixmap_input);
    QPixmap addText(QPixmap pixmap_input, QString text);
    void paintEvent(QPaintEvent *event) override;
    void manualSelect();
    void mousePressEvent(QMouseEvent *event) override;

 signals:
    void checked(EnumConstants::maskTypes type);

 public slots:
    void otherBoxChecked(EnumConstants::maskTypes type);
};

#endif  // PICPUSHBUTTON_H
