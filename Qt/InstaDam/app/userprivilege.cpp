#include "userprivilege.h"
#include "ui_userprivilege.h"

/*!
  \class UserPrivilege
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief The UserPrivilege provedes a class for setting user privileges.
 */

/*!
  Constructs a UserPrivilege instance with parent \a parent, if any.
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

/*!
  \fn void UserPrivilege::on_pushButton_clicked()

  Emitted when button is pushed.
 */

/*!
  \fn void UserPrivilege::on_pushButton_2_clicked()

  Emitted when button is pushed.
 */
