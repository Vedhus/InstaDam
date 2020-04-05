#include "filtercontrols.h"
#include <QtGlobal>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <vector>
#include <iostream>
#include "pixmapops.h"
#include "enumconstants.h"
#include "opencv2/ximgproc/edge_filter.hpp"

#define CONNECTCAST(OBJECT, TYPE, FUNC) static_cast<void(OBJECT::*)(TYPE)>(&OBJECT::FUNC)

/*!
  \class filterDialog
  \ingroup app
  \inmodule InstaDam
  \brief Defines a custom QDIalog based on the selected mask.
  Creates an instance based on \a selectedMask, \a fc, \a photoViewer, and
  \a currentPro.
*/

filterDialog::filterDialog(EnumConstants::maskTypes selectedMask,
                           filterControls* fc, PhotoViewer* photoViewer,
                           Project *currentPro, bool measureFlag): QDialog() {
    this->fc = fc;
    measure = measureFlag;
    size_t numControls = static_cast<size_t>(fc->properties[selectedMask]->numControls);
    this->setWindowTitle("Filter Options");
     QVBoxLayout *VBlayout = new QVBoxLayout(this);
    if (selectedMask != EnumConstants::COLORTHRESHOLD){
        setWindowFlags(Qt::Popup);
    }
    else {
        QLabel *name = new QLabel();
        name->setText("Click on the image to select a color");
        VBlayout->addWidget(name);
        setWindowFlags(Qt::WindowStaysOnTopHint);
    }

    Project* currentProject = currentPro;
    for (size_t i = 0; i < numControls; i++) {
        QLabel *name = new QLabel();
        name->setText(QString(fc->properties[selectedMask]->propertylist[i]->name));
        QHBoxLayout *HBlayout = new QHBoxLayout();
        HBlayout->addWidget(name);
        EnumConstants::threshold_or_filter thof =
        fc->properties[selectedMask]->propertylist[i]->threshold_filter;
        if (fc->properties[selectedMask]->propertylist[i]->showProp == true){
            switch (fc->properties[selectedMask]->propertylist[i]->btnType) {
                case SLIDER:
                {
                    fSlider *itemSlider = new fSlider(selectedMask,
                                                      static_cast<int>(i), thof, this);
                    fSpinBox* itemNumbox = new fSpinBox(selectedMask,
                                                        static_cast<int>(i), thof, this);
                    connect(itemSlider, SIGNAL(valueChanged(int)), itemNumbox,
                            SLOT(displayValue(int)));
                    itemSlider->setOrientation(Qt::Horizontal);
                    connect(itemNumbox, SIGNAL(valueChanged(int)), itemSlider,
                            SLOT(setValue(int)));

                    HBlayout->addWidget(itemSlider);
                    HBlayout->addWidget(itemNumbox);

                    itemSlider->setMaximum(fc->properties[selectedMask]->propertylist[i]->max);
                    itemNumbox->setMaximum(fc->properties[selectedMask]->propertylist[i]->max);
                    itemSlider->setMinimum(fc->properties[selectedMask]->propertylist[i]->min);
                    itemNumbox->setMinimum(fc->properties[selectedMask]->propertylist[i]->min);
                    itemNumbox->setValue(fc->properties[selectedMask]->propertylist[i]->val);
                    itemSlider->setValue(fc->properties[selectedMask]->propertylist[i]->val);


                    connect(itemSlider, SIGNAL(filterValueChanged(EnumConstants::maskTypes, int, int,
                                                                  EnumConstants::threshold_or_filter)),
                            fc, SLOT(assignVal(EnumConstants::maskTypes, int, int,\
                                               EnumConstants::threshold_or_filter)));

                    connect(this, SIGNAL(changeSlider(EnumConstants::maskTypes, int, int)),
                            itemSlider, SLOT(sliderChangedByCall(EnumConstants::maskTypes, int, int)));

                    connect(itemNumbox, SIGNAL(filterValueChanged(EnumConstants::maskTypes, int, int,
                                                                  EnumConstants::threshold_or_filter)),
                            fc, SLOT(assignVal(EnumConstants::maskTypes, int, int,
                                               EnumConstants::threshold_or_filter)));

                    VBlayout->addLayout(HBlayout);
                    break;
                }

                case CHECKBOX:
                {
                    fCheckBox *itemCBox = new fCheckBox(selectedMask,
                                                        static_cast<int>(i), thof, this);
                    itemCBox->setCheckState(Qt::CheckState(fc->properties[selectedMask]->propertylist[i]->val));
                    connect(itemCBox, SIGNAL(filterValueChanged(EnumConstants::maskTypes, int, int,
                                                                EnumConstants::threshold_or_filter)),
                            fc, SLOT(assignVal(EnumConstants::maskTypes, int, int,
                                               EnumConstants::threshold_or_filter)));
                    HBlayout->addWidget(itemCBox);
                    VBlayout->addLayout(HBlayout);

                    break;
                }
                case LABELLIST:
                {
                    labelButtons.clear();
                    VBlayout->addLayout(HBlayout);
                    for (int i=0; i < currentProject->numLabels(); i++) {
                        QSharedPointer<Label> label = currentProject->getLabel(i);
                        LabelButtonFilter *button = new LabelButtonFilter(label);
                        button->setText(label->getText());
                        QPalette pal = button->palette();
                        pal.setColor(QPalette::ButtonText, Qt::black);
                        pal.setColor(QPalette::Button, label->getColor());
                        button->setAutoFillBackground(true);
                        button->setPalette(pal);
                        button->update();
                        VBlayout->addWidget(button);
                        connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), fc,
                                SLOT(setLabelMask(QSharedPointer<Label>)));
                        connect(button, SIGNAL(cclicked(QSharedPointer<Label>)), this,
                                SLOT(checkLabel(QSharedPointer<Label>)));
                        labelButtons.push_back(button);
                    }
                    break;

                }
            }
            connect(fc, SIGNAL(valAssigned(EnumConstants::maskTypes, EnumConstants::threshold_or_filter)),
                    photoViewer, SLOT(setImMask(EnumConstants::maskTypes,
                                                EnumConstants::threshold_or_filter)));
        }
    }
    timer = new QElapsedTimer();
    displayTimer = new QTimer();
    connect(displayTimer, SIGNAL(timeout()), this, SLOT(checkTime()));

    QDialog::show();

}

/*!
  Called when a mouse button is pressed given by \a event.
*/
void filterDialog::mousePressEvent(QMouseEvent *event){
    mpos = event->pos();
    QDialog::mousePressEvent(event);
}

/*!
  Called when the mouse moves with \a event.
*/
void filterDialog::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
    QDialog::mouseMoveEvent(event);

}

/*!
 * \brief PhotoViewer::checkTime
 */

void filterDialog::checkTime(){
    if (measure){
        emit newTime(timer->elapsed());
        timer->restart();
    }
}

void filterDialog::updateMeasureFlag(bool measureFlag){
    measure = measureFlag;
}

void filterDialog::enterEvent(QEvent *event){
    QDialog::enterEvent(event);
    timer->restart();
    displayTimer->start(250);
}

void filterDialog::leaveEvent(QEvent *event){
    QDialog::leaveEvent(event);
    checkTime();
    displayTimer->stop();
}

/*!
  Called when a Label button is checked given by \a label.
*/
void filterDialog::checkLabel(QSharedPointer<Label> label) {
    for (int i = 0; i < labelButtons.size(); i++) {
        if (label != labelButtons[i]->myLabel) {
            labelButtons[i]->setChecked(false);
        } else {
            labelButtons[i]->setChecked(true);
        }
    }
}

/*!
  Called when a point is clicked on the screen when the filter dialog box is open
  */
void filterDialog::changeColor(cv::Scalar col){
    for (int i = 0; i<3; i++)
        emit changeSlider(EnumConstants::COLORTHRESHOLD, 2+i, col.val[i]);


}

/*!
  \class filterControls
  \ingroup app
  \inmodule InstaDam
  \brief Defines the properties of the mask and conducts the filtering operations.
*/
filterControls::filterControls():QObject(){
    defineProperties();
}


/*!
  Slot that sets the int \a value
  to the appropriate property indexed by \a maskType, \a propNum and \a thof.
*/
void filterControls::assignVal(EnumConstants::maskTypes maskType, int propNum, int value,
                               EnumConstants::threshold_or_filter thof) {
    this->properties[maskType]->propertylist[static_cast<size_t>(propNum)]->sliderAssign(value);
    emit valAssigned(maskType, thof);

}

/*!
 Obtains the label mask from \a label and sets it to be used as a mask
 for the LABELMASK filter operation.
*/
void filterControls::setLabelMask(QSharedPointer<Label> label) {
    this->labelMask = label->exportLabel(SelectItem::myBounds);


    emit valAssigned(EnumConstants::LABELMASK, EnumConstants::FILTER);
}

/*!
 Defines the properties of the different masks.
*/
void filterControls::defineProperties() {
    /* To define a new filter, define the filter properties, map and fitlermetaproperties in this function */

    /* Define canny filter default properties */
    std::vector<filterProperty*> cannyProperties;
    cannyProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2, 1,
                                                 ANY, EnumConstants::THRESH,
                                                 false));
    cannyProperties.push_back(new filterProperty("Threshold min", SLIDER, 0, 60,
                                                 255, ANY, EnumConstants::THRESH,
                                                 false));
    cannyProperties.push_back(new filterProperty("Threshold max", SLIDER, 0, 255,
                                                 255, ANY, EnumConstants::THRESH,
                                                 false));
    cannyProperties.push_back(new filterProperty("Low", SLIDER, 3, 5, 801, ODD,
                                                 EnumConstants::FILTER, false));
    cannyProperties.push_back(new filterProperty("High", SLIDER, 3, 5, 801, ODD,
                                                 EnumConstants::FILTER, false));
    cannyProperties.push_back(new filterProperty("Kernal", SLIDER, 3, 5, 7, ODD,
                                                 EnumConstants::FILTER, false));
    QMap<QString, int> cannyMap;
    cannyMap["invert"] = 0;
    cannyMap["threshMin"] = 1;
    cannyMap["threshMax"] = 2;
    cannyMap["low"] = 3;
    cannyMap["high"] = 4;
    cannyMap["K"] = 5;

    /* Define blur filter default properties */
    std::vector<filterProperty*> blurProperties;

    blurProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2, 1,
                                                ANY, EnumConstants::THRESH,
                                                false));
    blurProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                60, 255, ANY,
                                                EnumConstants::THRESH, false));
    blurProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                255, 255, ANY,
                                                EnumConstants::THRESH, false));
    blurProperties.push_back(new filterProperty("Blur amount", SLIDER, 3, 15, 51,
                                                ODD, EnumConstants::FILTER,
                                                false));
//    blurProperties.push_back(new filterProperty("Kernel X", SLIDER, 3, 15, 51,
//                                                ODD, EnumConstants::FILTER,
//                                                false));
//    blurProperties.push_back(new filterProperty("Kernel Y", SLIDER, 3, 15, 51,
//                                                ODD, EnumConstants::FILTER,
//                                                false));
//    blurProperties.push_back(new filterProperty("Sigma", SLIDER, 3, 5, 55,
//                                                ODD, EnumConstants::FILTER,
//                                                false));
    QMap<QString, int> blurMap;
    blurMap["invert"] = 0;
    blurMap["threshMin"] = 1;
    blurMap["threshMax"] = 2;
    blurMap["amount"] = 3;
//    blurMap["KX"] = 3;
//    blurMap["KY"] = 4;
//    blurMap["sigma"] = 5;

    /* Define thresholdfilter default properties */
    std::vector<filterProperty*> thresholdProperties;
    thresholdProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    thresholdProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     60, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    thresholdProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false));

    QMap<QString, int> threshMap;
    threshMap["invert"] = 0;
    threshMap["threshMin"] = 1;
    threshMap["threshMax"] = 2;

    /* Define otsufilter default properties */
    std::vector<filterProperty*> otsuProperties;
    otsuProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    otsuProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     60, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    otsuProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    QMap<QString, int> otsuMap;
    otsuMap["invert"] = 0;
    otsuMap["threshMin"] = 1;
    otsuMap["threshMax"] = 2;

    /* Define ridge filter default properties */
    std::vector<filterProperty*> ridgeProperties;
    ridgeProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    ridgeProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     60, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    ridgeProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    ridgeProperties.push_back(new filterProperty("Scale", SLIDER, 1,25, 100, ANY,
                                                 EnumConstants::FILTER, false));


    QMap<QString, int> ridgeMap;
    ridgeMap["invert"] = 0;
    ridgeMap["threshMin"] = 1;
    ridgeMap["threshMax"] = 2;
    ridgeMap["scale"] = 3;


    /* Define morph filter default properties */
    std::vector<filterProperty*> morphProperties;
    morphProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    morphProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     60, 255, ANY,
                                                     EnumConstants::FILTER,
                                                     false));
    morphProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::FILTER,
                                                     false));
    morphProperties.push_back(new filterProperty("Kernel", SLIDER, 1,
                                                     1, 11, ODD,
                                                     EnumConstants::FILTER,false));

    morphProperties.push_back(new filterProperty("Erode", CHECKBOX, 0, 2, 1, ANY,
                                                           EnumConstants::FILTER,
                                                           false));
    morphProperties.push_back(new filterProperty("Dilate", CHECKBOX, 0, 2,
                                                             1, ANY,
                                                             EnumConstants::FILTER,
                                                             false));
    morphProperties.push_back(new filterProperty("Open", CHECKBOX, 0, 2,
                                                             1, ANY,
                                                             EnumConstants::FILTER,
                                                             false));
    morphProperties.push_back(new filterProperty("Close", CHECKBOX, 0, 2,
                                                             1, ANY,
                                                             EnumConstants::FILTER,
                                                             false));
    QMap<QString, int> morphMap;
    morphMap["invert"] = 0;
    morphMap["threshMin"] = 1;
    morphMap["threshMax"] = 2;
    morphMap["K1"] = 3;
    morphMap["Erode"] = 4;
    morphMap["Dilate"] = 5;
    morphMap["Open"] = 6;
    morphMap["Close"] = 7;


    /* Define guided filter default properties */
    std::vector<filterProperty*> guidedProperties;
    guidedProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    guidedProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     60, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    guidedProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
//    guidedProperties.push_back(new filterProperty("Radius", SLIDER, 0,
//                                                     4, 24, ANY,
//                                                     EnumConstants::FILTER,
//                                                     false));
//    guidedProperties.push_back(new filterProperty("Eps", SLIDER, 1,25, 100, ANY,
//                                                 EnumConstants::FILTER, false));

    guidedProperties.push_back(new filterProperty("Diameter", SLIDER, 3, 7, 25,
                                                ODD, EnumConstants::FILTER,
                                                false));
    guidedProperties.push_back(new filterProperty("Sigma", SLIDER, 1, 51, 100,
                                                ODD, EnumConstants::FILTER,
                                                false));
    QMap<QString, int> guidedMap;
    guidedMap["invert"] = 0;
    guidedMap["threshMin"] = 1;
    guidedMap["threshMax"] = 2;
    guidedMap["d"] = 3;
    guidedMap["sigma"] = 4;




    /* Define local adaptive threishol filter default properties */
    std::vector<filterProperty*> latProperties;
    latProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    latProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     60, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    latProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    latProperties.push_back(new filterProperty("Gaussian", CHECKBOX, 0, 2,
                                                     1, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    latProperties.push_back(new filterProperty("Strength", SLIDER, 3, 3, 25,
                                                ODD, EnumConstants::FILTER,
                                                false));
    latProperties.push_back(new filterProperty("Detail", SLIDER, 1, 5, 25,
                                                ODD, EnumConstants::FILTER,
                                                false));

    QMap<QString, int> latMap;
    latMap["invert"] = 0;
    latMap["threshMin"] = 1;
    latMap["threshMax"] = 2;
    latMap["gauss"] = 3;
    latMap["K"] = 4;
    latMap["C"] = 5;

    /* Define color filter default properties */
    std::vector<filterProperty*> colorThresholdProperties;

    colorThresholdProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2,
                                                          1, ANY,
                                                          EnumConstants::THRESH,
                                                          false));
    colorThresholdProperties.push_back(new filterProperty("Fuzziness", SLIDER, 0,
                                                     20, 100, ANY,
                                                     EnumConstants::THRESH,
                                                     false));
    colorThresholdProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255,255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));

    colorThresholdProperties.push_back(new filterProperty("Red", SLIDER, 0,
                                                60, 255, ANY, EnumConstants::FILTER, false));
    colorThresholdProperties.push_back(new filterProperty("Blue", SLIDER, 0,
                                                          60, 255,
                                                ANY, EnumConstants::FILTER,
                                                false));
    colorThresholdProperties.push_back(new filterProperty("Green", SLIDER, 0,
                                                          60, 255,
                                                ANY, EnumConstants::FILTER,
                                                false));
    QMap<QString, int> colorMap;
    colorMap["invert"] = 0;
    colorMap["threshMin"] = 1;
    colorMap["threshMax"] = 2;
    colorMap["R"] = 3;
    colorMap["B"] = 4;
    colorMap["G"] = 5;



    /* Define labelmask default properties */
    std::vector<filterProperty*> labelmaskProperties;
    labelmaskProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2, 1,
                                                     ANY, EnumConstants:: THRESH,
                                                     false));
    labelmaskProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     1, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    labelmaskProperties.push_back(new filterProperty("Threshold max", SLIDER, 0,
                                                     255, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    labelmaskProperties.push_back(new filterProperty("Label List", LABELLIST, 0,
                                                     2, 1, ANY,
                                                     EnumConstants::FILTER,
                                                     false));

    QMap<QString, int> labelMap;
    labelMap["invert"] = 0;
    labelMap["threshMin"] = 1;
    labelMap["threshMax"] = 2;
    labelMap["LL"] = 3;



    filterPropertiesMeta *cannyPropertiesMeta =
            new filterPropertiesMeta(cannyProperties, 6, EnumConstants::CANNY);
    filterPropertiesMeta *blurPropertiesMeta =
            new filterPropertiesMeta(blurProperties, 4, EnumConstants::BLUR);
    filterPropertiesMeta *thresholdPropertiesMeta =
            new filterPropertiesMeta(thresholdProperties, 3,
                                     EnumConstants::THRESHOLD);

    filterPropertiesMeta *colorThresholdPropertiesMeta =
            new filterPropertiesMeta(colorThresholdProperties, 6,
                                     EnumConstants::COLORTHRESHOLD);

    filterPropertiesMeta *latPropertiesMeta =
            new filterPropertiesMeta(latProperties, 6,
                                     EnumConstants::LAT);

    filterPropertiesMeta *otsuPropertiesMeta =
            new filterPropertiesMeta(otsuProperties, 3,
                                     EnumConstants::OTSU);
    filterPropertiesMeta *ridgePropertiesMeta =
            new filterPropertiesMeta(ridgeProperties, ridgeMap.size(),
                                     EnumConstants::RIDGE);
    filterPropertiesMeta *morphPropertiesMeta =
            new filterPropertiesMeta(morphProperties, morphMap.size(),
                                     EnumConstants::MORPH);
    filterPropertiesMeta *guidedPropertiesMeta =
            new filterPropertiesMeta(guidedProperties, guidedMap.size(),
                                     EnumConstants::GUIDED);

    filterPropertiesMeta *labelmaskPropertiesMeta =
            new filterPropertiesMeta(labelmaskProperties, 4,
                                     EnumConstants::LABELMASK);


    /* Order follows order of enum defined in enumconstants.h */


    properties.push_back(blurPropertiesMeta);
    properties.push_back(cannyPropertiesMeta);
    properties.push_back(thresholdPropertiesMeta);
    properties.push_back(colorThresholdPropertiesMeta);
    properties.push_back(otsuPropertiesMeta);
    properties.push_back(latPropertiesMeta);
    properties.push_back(ridgePropertiesMeta);
    properties.push_back(morphPropertiesMeta);
    properties.push_back(guidedPropertiesMeta);
    properties.push_back(labelmaskPropertiesMeta);


    maps.push_back(blurMap);
    maps.push_back(cannyMap);
    maps.push_back(threshMap);
    maps.push_back(colorMap);
    maps.push_back(otsuMap);
    maps.push_back(latMap);
    maps.push_back(ridgeMap);
    maps.push_back(morphMap);
    maps.push_back(guidedMap);
    maps.push_back(labelMap);

}


/*!
 Filters the cv::Mat \a image based on the selected maskTypes \a selectedFilter
 and returns a binary image cv::Mat.
*/
cv::Mat filterControls::filterFunc(cv::Mat image,
                                   EnumConstants::maskTypes selectedFilter) {
    cv::Mat edge_temp;
    switch (selectedFilter) {
        case EnumConstants::CANNY:
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            cv::GaussianBlur(edge_temp, edge_temp,
                             cv::Size(properties[EnumConstants::BLUR]->propertylist[maps[EnumConstants::BLUR]["amount"]]->val,
                                      properties[EnumConstants::BLUR]->propertylist[maps[EnumConstants::BLUR]["amount"]]->val),
                                        properties[EnumConstants::BLUR]->propertylist[maps[EnumConstants::BLUR]["amount"]]->val,
                                        properties[EnumConstants::BLUR]->propertylist[maps[EnumConstants::BLUR]["amount"]]->val);
            cv::Canny(edge_temp, edge_temp, properties[selectedFilter]->propertylist[maps[selectedFilter]["low"]]->val,
                                        properties[selectedFilter]->propertylist[maps[selectedFilter]["high"]]->val,
                                        properties[selectedFilter]->propertylist[maps[selectedFilter]["K"]]->val);

            break;
        case EnumConstants::BLUR:
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            cv::GaussianBlur(edge_temp, edge_temp,
                             cv::Size(properties[selectedFilter]->propertylist[maps[selectedFilter]["amount"]]->val,
                                      properties[selectedFilter]->propertylist[maps[selectedFilter]["amount"]]->val),
                                        properties[selectedFilter]->propertylist[maps[selectedFilter]["amount"]]->val,
                                        properties[selectedFilter]->propertylist[maps[selectedFilter]["amount"]]->val);
            break;
        case EnumConstants::THRESHOLD:

            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            break;
        case EnumConstants::LABELMASK:
            if (!this->labelMask.isNull()) {
                QImage image_pixmap(this->labelMask.toImage().convertToFormat(QImage::Format_RGB888
                                                                              ));
                cv::Mat mat(image_pixmap.height(), image_pixmap.width(),
                            CV_8UC3, (uchar*)image_pixmap.bits(),image_pixmap.bytesPerLine());
                cv::resize(mat, mat, image.size());
                cv::cvtColor(mat, edge_temp, cv::COLOR_RGB2GRAY);
            }
            else {
                cv::Mat mat(image.size(), CV_8UC3, cv::Scalar(0,0,0));
                cv::cvtColor(mat, edge_temp, cv::COLOR_RGB2GRAY);
            }

            break;
        case EnumConstants::COLORTHRESHOLD:
            {
                cv::Scalar color =  cv::Scalar(properties[selectedFilter]->propertylist[(maps[selectedFilter].value("R"))]->val,
                                   properties[selectedFilter]->propertylist[maps[selectedFilter].value("G")]->val,
                                   properties[selectedFilter]->propertylist[maps[selectedFilter].value("B")]->val);
                cv::Mat colors(1,1, CV_8UC3, color);
                cv::Mat lab;
                cv::cvtColor(image, lab, cv::COLOR_RGB2Lab);
                lab.convertTo(lab, CV_16UC3);
                cv::cvtColor(colors, colors, cv::COLOR_RGB2Lab);
                cv::Scalar labPoint = cv::Scalar(colors.at<cv::Vec3b>(cv::Point(0,0)));
                cv::absdiff(lab, labPoint, lab);
                lab = lab.mul(lab);
                lab = lab/3;
                lab.convertTo(lab, CV_32FC3);
                cv::transform(lab, lab, cv::Matx13f(1,1,1));
                cv::sqrt(lab, edge_temp);
                edge_temp.convertTo(edge_temp, CV_8U);

            }
            break;
        case EnumConstants::OTSU:
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            cv::threshold(edge_temp, edge_temp,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            break;

        case EnumConstants::RIDGE:
            {
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            float scale = properties[selectedFilter]->propertylist[maps[selectedFilter]["scale"]]->val/25.0;
            cv::Ptr<cv::ximgproc::RidgeDetectionFilter> retval =
                    cv::ximgproc::RidgeDetectionFilter::create(CV_32FC1,
                                            1,1, 3, CV_8UC1, scale);
            qInfo()<<"Filter created";
                    qInfo()<<"Filter filtered";
            cv::Mat edges;
            cv::Mat3f fsrc;
            image.convertTo(fsrc, CV_32F, 1.0 / 255.0);

            //
            retval->getRidgeFilteredImage(edge_temp, edge_temp);
            qInfo()<<"Filter filtered";


            //cv::threshold(edge_temp, edge_temp,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            }
            break;

        case EnumConstants::MORPH:
            {
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            //cv::ximgproc::FastmorphSolverFilter::filter(image, edge_temp, 1);
            edge_temp = conductThreshold(edge_temp,selectedFilter);
            int K1 = properties[selectedFilter]->propertylist[maps[selectedFilter]["K1"]]->val;
            cv::Mat kernel = cv::Mat::ones(K1, K1, CV_32FC1);
            cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(K1, K1), cv::Point((K1-1)/2, (K1-1)/2) );

            if (properties[selectedFilter]->propertylist[maps[selectedFilter]["Open"]]->val == 2)
            {

               cv:: morphologyEx(edge_temp, edge_temp, cv::MORPH_OPEN, kernel);
            }
            if (properties[selectedFilter]->propertylist[maps[selectedFilter]["Close"]]->val == 2)
            {

                cv::morphologyEx(edge_temp, edge_temp, cv::MORPH_CLOSE, kernel);
            }
            if (properties[selectedFilter]->propertylist[maps[selectedFilter]["Erode"]]->val == 2)
            {

               cv::erode(edge_temp, edge_temp, element);
            }
            if (properties[selectedFilter]->propertylist[maps[selectedFilter]["Dilate"]]->val == 2)
            {

               cv::dilate(edge_temp, edge_temp, element);
            }


            }
            break;

        case EnumConstants::GUIDED:
            {
            qInfo()<<"Guided";

            int d =  properties[selectedFilter]->propertylist[maps[selectedFilter]["d"]]->val;
            int sigma =  properties[selectedFilter]->propertylist[maps[selectedFilter]["sigma"]]->val;


//            adaptiveThreshold(edge_temp, edge_temp, 200, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
//                              properties[selectedFilter]->propertylist[maps[selectedFilter]["K"]]->val,
//                              properties[selectedFilter]->propertylist[maps[selectedFilter]["C"]]->val);
//            cv::ximgproc::createFastGlobalSmootherFilter (edge_temp, double lambda, double sigma_color, double lambda_attenuation=0.25, int num_iter=3)
//            cv::ximgproc::guidedFilter(edge_temp,edge_temp, edge_temp, rad,eps);
            //image.convertTo(Temp, CV_8U)

            //edge_temp.convertTo(edge_temp, CV_8UC3);
            cv::bilateralFilter(image, edge_temp, d, sigma, sigma);
            cv::cvtColor(edge_temp, edge_temp, cv::COLOR_RGB2GRAY);


            }
            break;


        case EnumConstants::LAT:
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            qInfo()<<properties[selectedFilter]->propertylist[maps[selectedFilter]["gauss"]]->val;
            if (properties[selectedFilter]->propertylist[maps[selectedFilter]["gauss"]]->val == 2)
            {
                adaptiveThreshold(edge_temp, edge_temp, 200, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
                                  properties[selectedFilter]->propertylist[maps[selectedFilter]["K"]]->val,
                                  properties[selectedFilter]->propertylist[maps[selectedFilter]["C"]]->val);
            }
            else {
                adaptiveThreshold(edge_temp, edge_temp, 200, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,
                                  properties[selectedFilter]->propertylist[maps[selectedFilter]["K"]]->val,
                                  properties[selectedFilter]->propertylist[maps[selectedFilter]["C"]]->val);
            }
            break;
        case EnumConstants::OTHER:
            break;
    }
    return edge_temp;
}


/*!
  Returns a masked image based in the input \a image and \a selectedFilter.
*/
cv::Mat filterControls::filtAndGeneratePixmaps(cv::Mat image,
                                               EnumConstants::maskTypes selectedFilter) {
        img = image;
        edges = filterFunc(img, selectedFilter);
        im2pixmap(selectedFilter);
        return edges;
}

/* Compute Thresholded images */
cv::Mat filterControls::conductThreshold(cv::Mat edgImg, EnumConstants::maskTypes selectedFilter){
    cv::Mat binary1;
    cv::Mat binary2;
    cv::Mat binaryImg;


    cv::threshold(edgImg, binary1, properties[selectedFilter]->propertylist[maps[selectedFilter]["threshMin"]]->val,
            255, cv::THRESH_BINARY);
    cv::threshold(edgImg, binary2, properties[selectedFilter]->propertylist[maps[selectedFilter]["threshMax"]]->val,
            255, cv::THRESH_BINARY);
    cv::subtract(binary1, binary2, binaryImg);


    return binaryImg;

}

/*!
  Binarizes the image and converts it to a pixmap using \a selectedFilter.
*/
void filterControls::im2pixmap(EnumConstants::maskTypes selectedFilter) {

    cv::Mat binary;
    cv::Mat binary1;
    cv::Mat binary2;

    binary = conductThreshold(edges,selectedFilter );
    int invert = properties[selectedFilter]->propertylist[maps[selectedFilter]["invert"]]->val;
    if (invert == 2)
    {
        cv::subtract(255, binary, binary);
    }




    QImage qImgImg = QImage(reinterpret_cast<uchar*>(binary.data), binary.cols,
                            binary.rows, static_cast<int>(binary.step),
                            QImage::Format_Grayscale8);
    this->qImg = QPixmap::fromImage(qImgImg);
    QImage qAlphaImg = QImage(reinterpret_cast<uchar*>(binary.data), binary.cols,
                              binary.rows, static_cast<int>(binary.step),
                              QImage::Format_Alpha8);
    this->qAlpha = QPixmap::fromImage(qAlphaImg);
}

/*!
  Returns a thubnail pixmap for the filter selection bar at the bottom of
  InstaDam, based on \a thumb and \a selectedFilter.
*/
QPixmap filterControls::thumb2pixmap(cv::Mat thumb,
                                     EnumConstants::maskTypes selectedFilter) {
    qInfo("Enter Thumb");
    cv::Mat thumbEdges = this->filterFunc(thumb, selectedFilter);
    qInfo("Filtered thumbnail!");
    cv::Mat binaryThumb;
    binaryThumb =conductThreshold(thumbEdges,selectedFilter);
    int invert = properties[selectedFilter]->propertylist[maps[selectedFilter]["invert"]]->val;
    if (invert == 2)
    {
        cv::subtract(255, binaryThumb, binaryThumb);
    }

    QImage qImgImg_temp = QImage(reinterpret_cast<uchar*>(binaryThumb.data),
                                 binaryThumb.cols, binaryThumb.rows,
                                 static_cast<int>(binaryThumb.step),
                                 QImage::Format_Grayscale8);
    qImgThumb = QPixmap::fromImage(qImgImg_temp);
    return qImgThumb;
}

/*!
  \fn filterControls::valAssigned(EnumConstants::maskTypes maskType, EnumConstants::threshold_or_filter thof)
  This signal is emitted when a value is assigned with \a maskType and \a thof.
*/
