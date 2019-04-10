#include "userprivilege.h"
#include "ui_userprivilege.h"

UserPrivilege::UserPrivilege(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPrivilege)
{
    ui->setupUi(this);
}

UserPrivilege::~UserPrivilege()
{
    delete ui;
}
