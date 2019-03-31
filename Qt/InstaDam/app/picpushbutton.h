#ifndef PICPUSHBUTTON_H
#define PICPUSHBUTTON_H

#include "instadam.h"
#include "enumconstants.h"
#include<QAbstractButton>
#include<QPushButton>
#include<QLabel>
#include<QPixmap>

class PicPushButton : public QLabel
{
    Q_OBJECT
public:
    int w;
    int h;
    maskTypes filter = CANNY;
    QPixmap pixmap;
    QPixmap pixmap_hover;
    QPixmap pixmap_pressed;
    PicPushButton(QWidget *parent);
    bool check;
    void setMaskType(maskTypes);
    void updatePixmap();
    void resetPixmaps(QPixmap );
    QPixmap addText(QPixmap, QString);
    void paintEvent(QPaintEvent *) override;
    void manualSelect();
    virtual void mousePressEvent(QMouseEvent*) override;
    

signals:
    void checked(maskTypes);

public slots:
    void otherBoxChecked(maskTypes);
};


#endif // PICPUSHBUTTON_H
