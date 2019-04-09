#include "integrationTest.h"

void IntegrationTest::initTestCase() {
    idm = new InstaDam();
    idm->addLabelHash("Label1", Qt::red);
    idm->addLabelHash("Label2", Qt::blue);
    idm->addLabelHash("Label3", Qt::green);
    idm->on_actionNew_triggered();
    QCOMPARE(idm->currentProject->labels.size(), 3);
}

void IntegrationTest::cleanupTestCase() {
    delete idm;
}

QTEST_MAIN(IntegrationTest)
#include "moc_integrationTest.cpp"
