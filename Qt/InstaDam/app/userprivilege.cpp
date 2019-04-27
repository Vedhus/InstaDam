#include "userprivilege.h"
#include "ui_userprivilege.h"

/*!
  Creates a UserPrivilege instance with parent QWidget \a parent.
  */
UserPrivilege::UserPrivilege(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPrivilege)
{
    ui->setupUi(this);
}

/*!
  Destructor.
  */
UserPrivilege::~UserPrivilege()
{
    delete ui;
}
