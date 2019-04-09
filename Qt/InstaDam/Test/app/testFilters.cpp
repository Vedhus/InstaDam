#include "testSelect.h"
#include "../appTest/enumconstants.h"
#include "../appTest/filterproperty.h"
#include "../appTest/filtercontrols.h"
#include "../appTest/fslider.h"
#include "../appTest/photoviewer.h"


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

void TestSelect::testFSlider(){
    QWidget *widget = new QWidget();
    fSlider *fs = new fSlider(EnumConstants::BLUR, 2, EnumConstants::FILTER,
                              widget);
    fs->valueChanged(3);
    fs->sliderReleased();

    fSpinBox *fsb = new fSpinBox(EnumConstants::BLUR, 2, EnumConstants::FILTER,
                                 widget);
    fsb->valueChanged(3);

    fCheckBox *fcb = new fCheckBox(EnumConstants::BLUR, 2, EnumConstants::FILTER,
                                   widget);
    fcb->stateChanged(0);
    delete widget;
}

void TestSelect::testFilterControls(){
    Project *prj = new Project();
    PhotoViewer *viewer = new PhotoViewer();
    filterControls *fc = new filterControls();
    fc->assignVal(EnumConstants::BLUR, 0, 1, EnumConstants::FILTER);
    fc->setLabelMask(myLabel);

    delete prj;
    delete viewer;
    delete fc;
}
