#ifndef FILTERPROPERTY_H
#define FILTERPROPERTY_H

#include <QString>

#include <string>
#include <vector>

#include "instadam.h"
#include "enumconstants.h"

enum btnTypes{SLIDER, CHECKBOX, LABELLIST};
enum evenOdds{EVEN, ODD, ANY};

class filterProperty {
 public:
    filterProperty(std::string propertyName, btnTypes bt, int propMin,
                   int propVal, int propMax, evenOdds eo,
                   EnumConstants::threshold_or_filter thof, bool sb);
    QString name;
    btnTypes btnType;
    int max;
    int val;
    int min;
    evenOdds evenOdd;
    EnumConstants::threshold_or_filter threshold_filter;
    bool signalBool;
    void sliderAssign(int sliderVal);
};

class filterPropertiesMeta {
 public:
    filterPropertiesMeta(std::vector<filterProperty*> fp, int nc,
                         EnumConstants::maskTypes mt);
    int numControls;
    EnumConstants::maskTypes maskType;
    std::vector<filterProperty*> propertylist;
};

#endif  // FILTERPROPERTY_H
