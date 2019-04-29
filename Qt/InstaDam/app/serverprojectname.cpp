#include "serverprojectname.h"
#include "ui_serverprojectname.h"

/*!
  \class serverProjectName
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief Something.
 */

/*!
  Constructs a serverProjectName instance with parent \a parent, if any.
 */
serverProjectName::serverProjectName(QWidget *parent) :
    QWidget(parent), ui(new Ui::serverProjectName) {
    ui->setupUi(this);
}

/*!
  Destructor.
 */
serverProjectName::~serverProjectName() {
    delete ui;
}

/*!
  Hides the widget upon pressing cancel.
 */
void serverProjectName::on_pushButton_2_clicked() {
    hide();
}

/*!
  \fn serverProjectName::on_pushButton_clicked()

  This signal is emitted when button is clicked.
 */
