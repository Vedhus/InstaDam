#include "testSelect.h"
#include "../appTest/labelButton.h"

void TestSelect::testLabelButton(){
    LabelButton *lb = new LabelButton(myLabel);
    lb->wasClicked();
    lb->reemitValueChanged(5);

    LabelButtonFilter *lbf = new LabelButtonFilter(myLabel);
    lbf->wasClicked();

    delete lb;
    delete lbf;
}
