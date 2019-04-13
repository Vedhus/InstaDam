#include "serverprojectname.h"
#include "ui_serverprojectname.h"

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

void serverProjectName::on_pushButton_2_clicked()
{
    hide();
}
