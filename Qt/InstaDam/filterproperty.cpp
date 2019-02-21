#include "filterproperty.h"


filterProperty::filterProperty(std::string propertyName, btnTypes bt, int propMin,
                               int propVal, int propMax, evenOdds eo, threshold_or_filter thof, bool sb)

{

    this->name = QString::fromStdString(propertyName);
    this->btnType = bt;
    this->val = propVal;
    this->max = propMax;
    this->min = propMin;
    this->evenOdd = eo;
    this->threshold_filter = thof;
    this->signalBool = sb;

}
void filterProperty::sliderAssign(int sliderVal)
{

    val = sliderVal;
    if (evenOdd == ODD)
        val = (int)((val/2)*2+1);
    else if (evenOdd == EVEN)
        val = (int)((val/2)*2);
}


//filterProperty::~filterProperty()
//{
//    delete this;
//}

filterPropertiesMeta::filterPropertiesMeta (std::vector<filterProperty*> fp,  int nc, maskTypes mt)
{
    numControls = nc;
    maskType = mt;
    propertylist = fp;

}


//float filterProperty::valForSlider()
//    return val;

