#include "filtercontrols.h"
#include "filters.h"
#include<vector>
#include<iostream>
#include<QVBoxLayout>
#include<QLabel>
#include<QString>


#include <QtGlobal>
#define CONNECTCAST(OBJECT,TYPE,FUNC) static_cast<void(OBJECT::*)(TYPE)>(&OBJECT::FUNC)







filterDialog::filterDialog(maskTypes selectedMask, filterControls* fc, PhotoViewer* photoViewer):QDialog()
{
    qInfo("1");
    int numControls = fc->properties[(int)selectedMask]->numControls;
    this->setWindowTitle("Filter Options");

    QVBoxLayout *VBlayout = new QVBoxLayout(this);

    for (int i = 0; i<numControls; i++)
    {
        QLabel *name = new QLabel();
        name->setText(QString(fc->properties[(int)selectedMask]->propertylist[i]->name));

        QHBoxLayout *HBlayout = new QHBoxLayout();
        HBlayout->addWidget(name);
        threshold_or_filter thof = fc->properties[(int)selectedMask]->propertylist[i]->threshold_filter;
        switch (fc->properties[(int)selectedMask]->propertylist[i]->btnType)
        {

        case SLIDER:
        {
                fSlider *itemSlider = new fSlider(selectedMask, i,  thof, this);
                fSpinBox* itemNumbox = new fSpinBox(selectedMask, i, thof, this);
                connect(itemSlider, SIGNAL(valueChanged(int)), itemNumbox, SLOT(setValue(int)));
                itemSlider->setOrientation(Qt::Horizontal);
                connect(itemNumbox, SIGNAL(valueChanged(int)), itemSlider, SLOT(setValue(int)));

                HBlayout->addWidget(itemSlider);
                HBlayout->addWidget(itemNumbox);

                qInfo("max = %d",fc->properties[(int)selectedMask]->propertylist[i]->max);
                qInfo("3");
                itemSlider->setMaximum(fc->properties[(int)selectedMask]->propertylist[i]->max);
                itemNumbox->setMaximum(fc->properties[(int)selectedMask]->propertylist[i]->max);
                itemSlider->setMinimum(fc->properties[(int)selectedMask]->propertylist[i]->min);
                itemNumbox->setMinimum(fc->properties[(int)selectedMask]->propertylist[i]->min);
                itemNumbox->setValue(fc->properties[(int)selectedMask]->propertylist[i]->val);
                itemSlider->setValue(fc->properties[(int)selectedMask]->propertylist[i]->val);


                connect(itemSlider, SIGNAL(filterValueChanged(maskTypes , int , int, threshold_or_filter )),fc,
                        SLOT(assignVal(maskTypes , int , int, threshold_or_filter )));
                connect(itemNumbox, SIGNAL(filterValueChanged(maskTypes , int , int, threshold_or_filter )),fc,
                        SLOT(assignVal(maskTypes , int , int , threshold_or_filter )));



                VBlayout->addLayout(HBlayout);
             break;
        }

        case CHECKBOX:
        {
             fCheckBox *itemCBox = new fCheckBox(selectedMask, i,  thof, this);
             itemCBox->setCheckState(Qt::CheckState(fc->properties[(int)selectedMask]->propertylist[i]->val));


             connect(itemCBox, SIGNAL(filterValueChanged(maskTypes , int , int, threshold_or_filter )),fc,
                     SLOT(assignVal(maskTypes , int , int, threshold_or_filter )));



            HBlayout->addWidget(itemCBox);
            VBlayout->addLayout(HBlayout);

            break;
        }


        }
        connect(fc, SIGNAL(valAssigned(maskTypes , threshold_or_filter )),photoViewer,
                SLOT(setImMask(maskTypes ,  threshold_or_filter )));


    }
    QDialog::show();


}

filterControls::filterControls():QObject()

{
    defineProperties();

}



void filterControls::assignVal(maskTypes maskType, int propNum, int value,  threshold_or_filter thof)
{
    this->properties[(int)maskType]->propertylist[propNum]->sliderAssign(value);
    emit valAssigned(maskType, thof);
    qInfo("%d", this->properties[(int)maskType]->propertylist[propNum]->val);



}
void filterControls::defineProperties()
{



    qInfo("B1");
    std::vector<filterProperty*> cannyProperties;

       cannyProperties.push_back( new   filterProperty("Invert",CHECKBOX, 0, 2,  1, ANY, THRESH, false)  );
       cannyProperties.push_back(  new  filterProperty("Threshold min",SLIDER, 0, 60,  255,  ANY, THRESH, false)  );
       cannyProperties.push_back( new  filterProperty("Low",SLIDER, 3,5,  801,  ODD, FILTER, false)  );
       cannyProperties.push_back( new   filterProperty("High",SLIDER, 3, 5,  801,  ODD, FILTER, false)  );
       cannyProperties.push_back( new   filterProperty("Kernal",SLIDER, 3, 5,  7,  ODD, FILTER, false)  );



    std::vector<filterProperty*> blurProperties;

        blurProperties.push_back( new   filterProperty("Invert",CHECKBOX, 0, 2,  1, ODD, THRESH, false)  );
        blurProperties.push_back(  new  filterProperty("Threshold min",SLIDER, 0, 60,  255,  ANY, THRESH, false)  );
        blurProperties.push_back( new  filterProperty("Kernel X",SLIDER, 3, 15, 51,  ODD, FILTER, false) );
        blurProperties.push_back( new   filterProperty("Kernel Y",SLIDER, 3, 15,  51,  ODD, FILTER, false)  );
        blurProperties.push_back( new   filterProperty("Sigma",SLIDER, 3, 5,  55,  ODD, FILTER, false)  );




    std::vector<filterProperty*> thresholdProperties;

        thresholdProperties.push_back( new   filterProperty("Invert",CHECKBOX, 0, 2,  1, ANY, THRESH, false)  );
        thresholdProperties.push_back(  new  filterProperty("Threshold min",SLIDER, 0, 60,  255,  ANY, THRESH, false)  );




    filterPropertiesMeta *cannyPropertiesMeta = new filterPropertiesMeta(cannyProperties, 5, CANNY);
    filterPropertiesMeta *blurPropertiesMeta = new filterPropertiesMeta(blurProperties, 5, BLUR);
    filterPropertiesMeta *thresholdPropertiesMeta = new filterPropertiesMeta(thresholdProperties, 2, THRESHOLD);


    //Order follows order of enum defined in instadam.h
    properties.push_back(cannyPropertiesMeta);
    properties.push_back(thresholdPropertiesMeta);
    properties.push_back(blurPropertiesMeta);






}


cv::Mat filterControls::filterFunc(cv::Mat image, maskTypes selectedFilter)
{
    cv::Mat edge_temp;
    switch(selectedFilter)
        {
        case CANNY:
            cv::Canny(image, edge_temp, properties[(int)CANNY]->propertylist[2]->val,
                                        properties[(int)CANNY]->propertylist[3]->val,
                                        properties[(int)CANNY]->propertylist[4]->val);

            break;
        case BLUR:
            cv::GaussianBlur(image, edge_temp,
                             cv::Size(properties[(int)BLUR]->propertylist[2]->val, properties[(int)BLUR]->propertylist[3]->val) ,
                                           properties[(int)BLUR]->propertylist[4]->val);

            break;
        case THRESHOLD:
            cv::cvtColor(image, edge_temp, cv::COLOR_RGB2GRAY);
            break;
        }

    return edge_temp;
}

//sets image for the object and stores and returns filtered edges. Also generated pixmaps
cv::Mat filterControls::filtAndGeneratePixmaps(cv::Mat image, maskTypes selectedFilter)
{

        img = image;
        edges = filterFunc(img, selectedFilter);
        im2pixmap(selectedFilter);
        return edges;
}

//Generates pixmap and is only called by filt() function
void filterControls::im2pixmap(maskTypes selectedFilter)
{
    cv::Mat binary;
    int invert = properties[(int)selectedFilter]->propertylist[0]->val;


    qInfo("%d", invert);
    if (invert==2)
        cv::threshold( edges, binary,  properties[(int)selectedFilter]->propertylist[1]->val,
                                        255, cv::THRESH_BINARY_INV );
    else
        cv::threshold( edges, binary,  properties[(int)selectedFilter]->propertylist[1]->val,
                                       255, cv::THRESH_BINARY);

    QImage qImgImg = QImage((uchar*) binary.data, binary.cols, binary.rows, binary.step, QImage::Format_Grayscale8);
    this->qImg = QPixmap::fromImage(qImgImg);
    QImage qAlphaImg = QImage((uchar*) binary.data, binary.cols, binary.rows, binary.step, QImage::Format_Alpha8);
    this->qAlpha = QPixmap::fromImage(qAlphaImg);

}





//Returns a thubnail pixmap for the filter selection bar at the bottom
QPixmap filterControls::thumb2pixmap(cv::Mat thumb, maskTypes selectedFilter)
{
    qInfo("Enter Thumb");
    cv::Mat thumbEdges = this->filterFunc(thumb, selectedFilter);
    qInfo("Filtered thumbnail!");
    cv::Mat binaryThumb;
    int invert = properties[(int)selectedFilter]->propertylist[0]->val;

    if (invert==2)
        cv::threshold( thumbEdges, binaryThumb,  properties[(int)selectedFilter]->propertylist[1]->val,
                                       255, cv::THRESH_BINARY_INV );
    else
        cv::threshold( thumbEdges, binaryThumb,  properties[(int)selectedFilter]->propertylist[1]->val,
                                       255, cv::THRESH_BINARY );
    QImage qImgImg_temp = QImage((uchar*) binaryThumb.data, binaryThumb.cols, binaryThumb.rows,binaryThumb.step, QImage::Format_Grayscale8);
    qImgThumb = QPixmap::fromImage(qImgImg_temp);
    return qImgThumb;
}







//            for key, value in filterObj.properties.items():
//                name = QtWidgets.QLabel()
//                name.setText(value.name)
//                if value.btnType == 'slider':
//                    #item = QtWidgets.QSlider()
//                    itemSlider = QtWidgets.QSlider()
//                    itemNumbox = QtWidgets.QSpinBox()
//                    itemSlider->valueChanged.connect(itemNumbox.setValue)
//                    itemSlider.setOrientation(Qt.Horizontal)
//                    itemNumbox->valueChanged.connect(itemSlider.setValue)
//                    HBlayout = QtWidgets.QHBoxLayout()
//                    HBlayout.addWidget(name)
//                    for item in [itemSlider, itemNumbox]:
//                        item.setMaximum(value.max)
//                        item.setMinimum(value.min)
//                        item.setValue(value->val)
//                        if value.signalBool:
//                            value->valchanged.connect(item.setValue)
//                        item->valueChanged.connect(eval('filterObj.update{0}'.format(key)))
//                        item.valueChanged.connect(eval('viewer.modifyMask{0}'.format(value.thof)))
//                        HBlayout.addWidget(item)


//                elif value.btnType == 'checkBox':
//                    item = QtWidgets.QCheckBox()
//                    item.setChecked = 'False'
//                    item.stateChanged.connect(eval('filterObj.update{0}'.format(key)))
//                    item.stateChanged.connect(eval('viewer.modifyMask{0}'.format(value.thof)))
//                    HBlayout = QtWidgets.QHBoxLayout()
//                    HBlayout.addWidget(name)
//                    HBlayout.addWidget(item)
//                VBlayout.addLayout(HBlayout)


//}
