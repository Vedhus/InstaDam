#include "testSelect.h"
#include "../appTest/project.h"

void TestSelect::testProject(){
    Project *prj = new Project();
    myLabel->setText("Label");
    prj->addLabel(myLabel);
    QCOMPARE(prj->numLabels(), 1);
    prj->resetLabels();
    QCOMPARE(prj->numLabels(), 0);
    prj->setId(2);
    QCOMPARE(prj->getId(), 2);
    prj->setName("Project");
    QCOMPARE(prj->getName(), "Project");
    prj->addLabel(myLabel);
    QSharedPointer<Label> lab = QSharedPointer<Label>::create();
    lab->setText("L2");
    prj->setLabel(0, lab);
    QCOMPARE(prj->getLabel(0)->getText(), "L2");
    delete prj;
}
