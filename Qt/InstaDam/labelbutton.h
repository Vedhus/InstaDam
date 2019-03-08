#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QSlider>
#include <QObject>
#include <QPushButton>

class labelButton: public QPushButton
{
    Q_OBJECT
public:
    labelButton(int);
    int labelNum;
    QSlider *slider;
signals:
    void lClicked(int, QColor);
    void opacity(int, int);
private slots:
    void reemitClicked();
    void reemitValueChanged(int);
};




#endif // LABELBUTTON_H
