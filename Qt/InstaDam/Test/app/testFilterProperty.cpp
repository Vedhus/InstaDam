#include "testSelect.h"
#include "../appTest/enumconstants.h"
#include "../appTest/filterproperty.h"

void TestSelect::testFilterProperty(){
    filterProperty *fp = new filterProperty("Name", SLIDER, 1, 5, 99, ODD,
                                            EnumConstants::FILTER, false);
    filterProperty *fpp = new filterProperty("Name", SLIDER, 0, 6, 100, EVEN,
                                             EnumConstants::THRESH, true);
    fp->sliderAssign(50);
    QCOMPARE(fp->val, 51);
    fp->sliderAssign(51);
    QCOMPARE(fp->val, 51);
    fpp->sliderAssign(50);
    QCOMPARE(fpp->val, 50);
    fpp->sliderAssign(51);
    QCOMPARE(fpp->val, 50);
    std::vector<filterProperty*> plist;
    plist.push_back(fp);
    plist.push_back(fpp);
    filterPropertiesMeta *fpm = new filterPropertiesMeta(plist, 2,
                                                         EnumConstants::LABELMASK);
    delete fp;
    delete fpp;
    delete fpm;
}
