#include "filterproperty.h"
#include "enumconstants.h"

/*!
  \class filterProperty
  \ingroup app
  \inmodule InstaDam
  \brief Holds the filter properties.

  Initializes the filter properties to the provided parameter values
*/

/*!
  Creates an instance based on \a propertyName, \a bt, \a propMin, \a propVal,
  \a propMax, \a eo, \a thof, \a sb, and \a show_flag.
*/
filterProperty::filterProperty(std::string propertyName, btnTypes bt, int propMin,
                               int propVal, int propMax, evenOdds eo,
                               EnumConstants::threshold_or_filter thof, bool sb,
                               bool show_flag ) {
    this->name = QString::fromStdString(propertyName);
    this->btnType = bt;
    this->val = propVal;
    this->max = propMax;
    this->min = propMin;
    this->evenOdd = eo;
    this->threshold_filter = thof;
    this->signalBool = sb;
    this->showProp = show_flag;
}

/*!
  Assigns the slider value to the nearest odd or even numbber close to \a sliderVal
*/
void filterProperty::sliderAssign(int sliderVal) {
    val = sliderVal;
    if (evenOdd == ODD)
        val = (val / 2) * 2 + 1;
    else if (evenOdd == EVEN)
        val = (val / 2) * 2;
}

/*!
  \class filterPropertiesMeta
  \ingroup app
  \inmodule InstaDam

  Holds information about the particular maskType \a mt including the number of controls
  and the list of properties \a fp.
*/

/*!
  Creates an instance based on \a fp, \a nc, and \a mt.
*/
filterPropertiesMeta::filterPropertiesMeta(std::vector<filterProperty*> fp, int nc,
                                           EnumConstants::maskTypes mt) {
    numControls = nc;
    maskType = mt;
    propertylist = fp;
}
