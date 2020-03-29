#ifndef FILTERCONTROLS_H
#define FILTERCONTROLS_H

#include <QDialog>

#include <vector>
#include <QElapsedTimer>
#include <QTimer>
#include "opencv2/core/mat.hpp"

#include "fslider.h"
#include "filterproperty.h"
#include "label.h"
#include "labelButton.h"
#include "project.h"
#include "enumconstants.h"

class PhotoViewer;

class filterDialog: public QDialog {
    Q_OBJECT
 public:
    filterDialog(EnumConstants::maskTypes selectedMask, filterControls* fc,
                 PhotoViewer* photoViewer, Project *currentPro, bool);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint mpos;
    filterControls *fc;
    QElapsedTimer *timer;
    QTimer *displayTimer;
    bool measure = false;

    QVector<LabelButtonFilter *> labelButtons;
public slots:
    void checkLabel(QSharedPointer<Label> label);
    void changeColor(cv::Scalar);
    void checkTime();
    void updateMeasureFlag(bool measureFlag);
signals:
    void changeSlider(EnumConstants::maskTypes maskType, int propNum, int value);
    void newTime(float);

protected:
   void enterEvent(QEvent *event);
   void leaveEvent(QEvent *event);



};

class filterControls: public QObject {
    Q_OBJECT

 public:
    filterControls();
    void defineProperties();
    cv::Mat conductThreshold(cv::Mat, EnumConstants::maskTypes);

    cv::Mat img;
    cv::Mat edges;
    cv::Mat filterFunc(cv::Mat image, EnumConstants::maskTypes selectedFilter);
    QPixmap qImg;
    QPixmap qAlpha;
    QPixmap qImgThumb;
    cv::Mat filtAndGeneratePixmaps(cv::Mat image, EnumConstants::maskTypes selectedFilter);
    void im2pixmap(EnumConstants::maskTypes selectedFilter);
    std::vector<filterPropertiesMeta* > properties;
    std::vector<QMap<QString, int> > maps;
    QPixmap thumb2pixmap(cv::Mat thumb, EnumConstants::maskTypes selectedFilter);
    //void show(maskTypes);
    PhotoViewer *photoViewer;
    QPixmap labelMask;


 public slots:
    void assignVal(EnumConstants::maskTypes maskType, int propNum, int value,
                   EnumConstants::threshold_or_filter thof);

    void setLabelMask(QSharedPointer<Label> label);

 signals:
    void valAssigned(EnumConstants::maskTypes maskType,
                     EnumConstants::threshold_or_filter thof);



};

#endif  // FILTERCONTROLS_H
