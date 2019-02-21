#ifndef FILTERCONTROLS_H
#define FILTERCONTROLS_H
#include "photoViewer.h"
#include<QDialog>
#include "fslider.h"
#include "filterproperty.h"

class PhotoViewer;

class filterDialog: public QDialog

{
public:
    filterDialog(maskTypes, filterControls*, PhotoViewer* );
};

class filterControls: public QObject
{
    Q_OBJECT
public:
    filterControls();
    void defineProperties();

    cv::Mat img;
    cv::Mat edges;
    cv::Mat filterFunc(cv::Mat, maskTypes);
    QPixmap qImg;
    QPixmap qAlpha;
    QPixmap qImgThumb;
    cv::Mat filtAndGeneratePixmaps(cv::Mat, maskTypes);
    void im2pixmap(maskTypes selectedFilter);
    std::vector<filterPropertiesMeta* > properties;
    QPixmap thumb2pixmap(cv::Mat, maskTypes);
    void show(maskTypes);
    PhotoViewer *photoViewer;
public slots:
    void assignVal(maskTypes maskType, int propNum, int value,  threshold_or_filter thof);
signals:
    void valAssigned(maskTypes maskType,  threshold_or_filter thof);

};

#endif // FILTERCONTROLS_H
