#include "projectdeletionconfirmation.h"
#include "ui_projectdeletionconfirmation.h"

/*!
  Constructor
  */
projectDeletionConfirmation::projectDeletionConfirmation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::projectDeletionConfirmation)
{
    ui->setupUi(this);
}

/*!
  Destructor
  */
projectDeletionConfirmation::~projectDeletionConfirmation()
{
    delete ui;
}

/*!
  cancels
  */
void projectDeletionConfirmation::on_noCancel_clicked()
{
    close();
}

/*!
  emits a signal to delete the selected project.
  */
void projectDeletionConfirmation::on_yesDelete_clicked()
{
    emit projectDeleted(this->project_name);
    close();
}
