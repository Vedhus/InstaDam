#include "serverprojectname.h"
#include "ui_serverprojectname.h"

/*!
  Creates a serverProjectName instance with parent QWidget \a parent.
  */
serverProjectName::serverProjectName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serverProjectName)
{
    ui->setupUi(this);
}

/*!
  Destructor.
  */
serverProjectName::~serverProjectName()
{
    delete ui;
}

/*!
  Hides the widget upon pressing cancel.
  */
void serverProjectName::on_pushButton_2_clicked()
{
    hide();
}
