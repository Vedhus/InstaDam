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
    fSlider(EnumConstants::maskTypes maskType, int propNums,
            EnumConstants::threshold_or_filter tf, QWidget *parent);
    EnumConstants::maskTypes selectedMask;
    int propNum;
    EnumConstants::threshold_or_filter thof;

 signals:
    void filterValueChanged(EnumConstants::maskTypes selectedMask, int propNum,
                            int value, EnumConstants::threshold_or_filter thof);
    void fSliderReleased(EnumConstants::maskTypes selectedMask,
                         EnumConstants::threshold_or_filter tf);

 private slots:
    void reemitValueChanged(int value);
    void reemitSliderReleased();
};

class fSpinBox : public QSpinBox {
    Q_OBJECT

 public:
    fSpinBox(EnumConstants::maskTypes maskType, int propNums,
             EnumConstants::threshold_or_filter tf, QWidget *parent);
    EnumConstants::maskTypes selectedMask;
    int propNum;
    EnumConstants::threshold_or_filter thof;


 signals:
    void filterValueChanged(EnumConstants::maskTypes selectedMask, int propNum,
                            int value, EnumConstants::threshold_or_filter tf);
    void fSlotChanged(EnumConstants::maskTypes selectedMask,
                      EnumConstants::threshold_or_filter tf);

 private slots:
    void reemitValueChanged(int value);
};


class fCheckBox : public QCheckBox {
    Q_OBJECT

 public:
    fCheckBox(EnumConstants::maskTypes maskType,  int propNums,
              EnumConstants::threshold_or_filter tf, QWidget *parent);
    EnumConstants::maskTypes selectedMask;
    int propNum;
    EnumConstants::threshold_or_filter thof;

 signals:
    void filterValueChanged(EnumConstants::maskTypes selectedMask, int propNum,
                            int value, EnumConstants::threshold_or_filter tf);
    void fStateChanged(EnumConstants::maskTypes selectedMask,
                       EnumConstants::threshold_or_filter tf);

 private slots:
    void reemitStateChanged(int value);
};

#endif  // FSLIDER_H
