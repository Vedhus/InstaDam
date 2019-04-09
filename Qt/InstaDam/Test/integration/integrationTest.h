#ifndef INTEGRATIONTEST_H
#define INTEGRATIONTEST_H
#include <QtTest/QtTest>
#include "../appTest/instadam.h"

class IntegrationTest: public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
    InstaDam *idm;
};

#endif // INTEGRATIONTEST_H
