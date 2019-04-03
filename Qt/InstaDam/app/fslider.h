#ifndef FSLIDER_H
#define FSLIDER_H

#include <QObject>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>

#include "enumconstants.h"

class fSlider : public QSlider {
    Q_OBJECT

 public:
    fSlider(maskTypes maskType, int propNums, threshold_or_filter tf,
            QWidget *parent);
    maskTypes selectedMask;
    int propNum;
    threshold_or_filter thof;

 signals:
    void filterValueChanged(maskTypes selectedMask, int propNum, int value,
                            threshold_or_filter thof);
    void fSliderReleased(maskTypes selectedMask, threshold_or_filter tf);

 private slots:
    void reemitValueChanged(int value);
    void reemitSliderReleased();
};

class fSpinBox : public QSpinBox {
    Q_OBJECT

 public:
    fSpinBox(maskTypes maskType, int propNums, threshold_or_filter tf,
             QWidget *parent);
    maskTypes selectedMask;
    int propNum;
    threshold_or_filter thof;


 signals:
    void filterValueChanged(maskTypes selectedMask, int propNum, int value,
                            threshold_or_filter tf);
    void fSlotChanged(maskTypes selectedMask, threshold_or_filter tf);

 private slots:
    void reemitValueChanged(int value);
};


class fCheckBox : public QCheckBox {
    Q_OBJECT

 public:
    fCheckBox(maskTypes maskType,  int propNums, threshold_or_filter tf,
              QWidget *parent);
    maskTypes selectedMask;
    int propNum;
    threshold_or_filter thof;

 signals:
    void filterValueChanged(maskTypes selectedMask, int propNum, int value,
                            threshold_or_filter tf);
    void fStateChanged(maskTypes selectedMask, threshold_or_filter tf);

 private slots:
    void reemitStateChanged(int value);
};

#endif  // FSLIDER_H
