#include "serverprojectname.h"
#include "ui_serverprojectname.h"
#include "login.h"
#include "newproject.h"
#include <QtDebug>

serverProjectName::serverProjectName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serverProjectName)
{
    ui->setupUi(this);
}

serverProjectName::~serverProjectName()
{
    delete ui;
}

void serverProjectName::on_pushButton_clicked()
{
    QString projectName = ui->projectName->toPlainText();
    newproject *np = new newproject();
    np->newPr->setName(projectName);
    np->runningLocally = false;
    np->databaseURL = this->databaseURL;
    np->accessToken = this->accessToken;
    np->show();
    hide();

}

void serverProjectName::on_pushButton_2_clicked()
{
    qInfo() << "Moving back to the login page";
    Login *log = new Login;
    log->show();
    hide();
}
