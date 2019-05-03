#include "filtercontrols.h"
#include <QtGlobal>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <vector>
#include <iostream>
#include "pixmapops.h"
#include "enumconstants.h"

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
                           Project *currentPro): QDialog() {
    size_t numControls = static_cast<size_t>(fc->properties[selectedMask]->numControls);
    this->setWindowTitle("Filter Options");
    setWindowFlags(Qt::Popup);
    QVBoxLayout *VBlayout = new QVBoxLayout(this);
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
                            fc, SLOT(assignVal(EnumConstants::maskTypes, int, int,
                                               EnumConstants::threshold_or_filter)));
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

    /* Define canny filter default properties */
    std::vector<filterProperty*> cannyProperties;
    cannyProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2, 1,
                                                 ANY, EnumConstants::THRESH,
                                                 false));
    cannyProperties.push_back(new filterProperty("Threshold min", SLIDER, 0, 60,
                                                 255, ANY, EnumConstants::THRESH,
                                                 false));
    cannyProperties.push_back(new filterProperty("Low", SLIDER, 3, 5, 801, ODD,
                                                 EnumConstants::FILTER, false));
    cannyProperties.push_back(new filterProperty("High", SLIDER, 3, 5, 801, ODD,
                                                 EnumConstants::FILTER, false));
    cannyProperties.push_back(new filterProperty("Kernal", SLIDER, 3, 5, 7, ODD,
                                                 EnumConstants::FILTER, false));

    /* Define blur filter default properties */
    std::vector<filterProperty*> blurProperties;

    blurProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2, 1,
                                                ODD, EnumConstants::THRESH,
                                                false));
    blurProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                60, 255, ANY,
                                                EnumConstants::THRESH, false));
    blurProperties.push_back(new filterProperty("Kernel X", SLIDER, 3, 15, 51,
                                                ODD, EnumConstants::FILTER,
                                                false));
    blurProperties.push_back(new filterProperty("Kernel Y", SLIDER, 3, 15, 51,
                                                ODD, EnumConstants::FILTER,
                                                false));
    blurProperties.push_back(new filterProperty("Sigma", SLIDER, 3, 5, 55,
                                                ODD, EnumConstants::FILTER,
                                                false));

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

    std::vector<filterProperty*> labelmaskProperties;
    labelmaskProperties.push_back(new filterProperty("Invert", CHECKBOX, 0, 2, 1,
                                                     ANY, EnumConstants:: THRESH,
                                                     false));
    labelmaskProperties.push_back(new filterProperty("Threshold min", SLIDER, 0,
                                                     1, 255, ANY,
                                                     EnumConstants::THRESH,
                                                     false, false));
    labelmaskProperties.push_back(new filterProperty("Label List", LABELLIST, 0,
                                                     2, 1, ANY,
                                                     EnumConstants::FILTER,
                                                     false));

    filterPropertiesMeta *cannyPropertiesMeta =
            new filterPropertiesMeta(cannyProperties, 5, EnumConstants::CANNY);
    filterPropertiesMeta *blurPropertiesMeta =
            new filterPropertiesMeta(blurProperties, 5, EnumConstants::BLUR);
    filterPropertiesMeta *thresholdPropertiesMeta =
            new filterPropertiesMeta(thresholdProperties, 2,
                                     EnumConstants::THRESHOLD);
    filterPropertiesMeta *labelmaskPropertiesMeta =
            new filterPropertiesMeta(labelmaskProperties, 3,
                                     EnumConstants::LABELMASK);

    /* Order follows order of enum defined in instadam.h */
    properties.push_back(cannyPropertiesMeta);
    properties.push_back(thresholdPropertiesMeta);
    properties.push_back(blurPropertiesMeta);
    properties.push_back(labelmaskPropertiesMeta);
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
            cv::Canny(image, edge_temp, properties[EnumConstants::CANNY]->propertylist[2]->val,
                                        properties[EnumConstants::CANNY]->propertylist[3]->val,
                                        properties[EnumConstants::CANNY]->propertylist[4]->val);
            break;
        case EnumConstants::BLUR:
            cv::GaussianBlur(image, edge_temp,
                             cv::Size(properties[EnumConstants::BLUR]->propertylist[2]->val,
                                      properties[EnumConstants::BLUR]->propertylist[3]->val),
                                      properties[EnumConstants::BLUR]->propertylist[4]->val);
            break;
        case EnumConstants::THRESHOLD:
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            break;
        case EnumConstants::LABELMASK:
            if (!this->labelMask.isNull()) {
                QImage image_pixmap(this->labelMask.toImage().convertToFormat(QImage::Format_RGB888));
                cv::Mat mat(image_pixmap.height(), image_pixmap.width(),
                            CV_8UC3, image_pixmap.bits());
                cv::resize(mat, mat, image.size());
                cv::cvtColor(mat, edge_temp, cv::COLOR_RGB2GRAY);
            }
            else {
                cv::Mat mat(image.size(), CV_8UC3, cv::Scalar(0,0,0));
                cv::cvtColor(mat, edge_temp, cv::COLOR_RGB2GRAY);
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

/*!
  Binarizes the image and converts it to a pixmap using \a selectedFilter.
*/
void filterControls::im2pixmap(EnumConstants::maskTypes selectedFilter) {
    cv::Mat binary;
    int invert = properties[selectedFilter]->propertylist[0]->val;


    qInfo("%d", invert);
    if (invert == 2)
        cv::threshold(edges, binary, properties[selectedFilter]->propertylist[1]->val,
                255, cv::THRESH_BINARY_INV);
    else
        cv::threshold(edges, binary, properties[selectedFilter]->propertylist[1]->val,
                255, cv::THRESH_BINARY);

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
    int invert = properties[selectedFilter]->propertylist[0]->val;

    if (invert == 2)
        cv::threshold(thumbEdges, binaryThumb, properties[selectedFilter]->propertylist[1]->val,
                255, cv::THRESH_BINARY_INV);
    else
        cv::threshold(thumbEdges, binaryThumb, properties[selectedFilter]->propertylist[1]->val,
                255, cv::THRESH_BINARY);
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
