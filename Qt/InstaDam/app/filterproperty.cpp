#include "filterproperty.h"

/*!
  \class filterProperty
  \ingroup app
  \inmodule InstaDam
  \brief Holds the filter properties.

  Initializes the filter properties to the provided parameter values
 */
filterProperty::filterProperty(std::string propertyName, btnTypes bt,
                               int propMin, int propVal, int propMax,
                               evenOdds eo, threshold_or_filter thof, bool sb) {
    this->name = QString::fromStdString(propertyName);
    this->btnType = bt;
    this->val = propVal;
    this->max = propMax;
    this->min = propMin;
    this->evenOdd = eo;
    this->threshold_filter = thof;
    this->signalBool = sb;
}

/*!
 * Assigns the slider value to the nearest odd or even numbber close to \a sliderVal
 */
void filterProperty::sliderAssign(int sliderVal) {
    val = sliderVal;
    if (evenOdd == ODD)
        val = (val / 2) * 2 + 1;
    else if (evenOdd == EVEN)
        val = (val / 2) * 2;
}

/*!
 * \class filterPropertiesMeta
 * Holds information about the particular maskType \a mt including the number of controls
 * and the list of properties \fp.
 */

filterPropertiesMeta::filterPropertiesMeta(std::vector<filterProperty*> fp,
                                           int nc, maskTypes mt) {
    numControls = nc;
    maskType = mt;
    propertylist = fp;
}
