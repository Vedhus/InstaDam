#ifndef FSLIDER_H
#define FSLIDER_H
#include <QObject>
#include "instadam.h"
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>

class fSlider : public QSlider
{
    Q_OBJECT
public:
    fSlider(maskTypes,  int,threshold_or_filter, QWidget *);
    maskTypes selectedMask;
    int propNum;
    threshold_or_filter thof;

signals:
    void filterValueChanged(maskTypes selectedMask, int propNum, int value, threshold_or_filter);
    void fSliderReleased(int);
private slots:
    void reemitValueChanged(int value);
    void reemitSliderReleased();


};

class fSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    fSpinBox(maskTypes,  int ,threshold_or_filter, QWidget*);
    maskTypes selectedMask;
    int propNum;
    threshold_or_filter thof;


signals:
    void filterValueChanged(maskTypes selectedMask, int propNum, int value, threshold_or_filter);
    void fSlotChanged(maskTypes, threshold_or_filter);

private slots:
    void reemitValueChanged(int value);



};


class fCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    fCheckBox(maskTypes,  int ,threshold_or_filter, QWidget*);
    maskTypes selectedMask;
    int propNum;
    threshold_or_filter thof;


signals:
    void filterValueChanged(maskTypes selectedMask, int propNum, int value, threshold_or_filter);
    void fStateChanged(maskTypes, threshold_or_filter);

private slots:
    void reemitStateChanged(int value);



};



#endif // FSLIDER_H





