#ifndef FILTERPROPERTY_H
#define FILTERPROPERTY_H
#include <string>
#include <QString>
//#include <QWidget>


#include "instadam.h"



enum btnTypes{SLIDER, CHECKBOX, LABELLIST};
enum evenOdds{EVEN, ODD, ANY};


class filterProperty
{


public:
    filterProperty(std::string, btnTypes, int, int, int, evenOdds , threshold_or_filter , bool);
    QString name;
   // ~filterProperty();
    btnTypes btnType;
    int max;
    int val;
    int min;
    evenOdds evenOdd;
    threshold_or_filter threshold_filter;
    bool signalBool;
    void sliderAssign(int sliderVal);



};

class filterPropertiesMeta
{
public:
    filterPropertiesMeta(std::vector<filterProperty*>, int, maskTypes);
    int numControls;
    maskTypes maskType;
    std::vector<filterProperty*> propertylist;
};

#endif // FILTERPROPERTY_H
