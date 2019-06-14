#include "startingwidget.h"
#include <QDebug>
#include <QCoreApplication>
#include "login.h"
#include "instadam.h"
#include "ui_startingwidget.h"
/*!
  \class StartingWidget
  \ingroup app
  \inmodule InstaDam
  \inherits QWidget
  \brief Starting widget for selecting user mode, either Local or Server.

 */

/*!
  Constructs a Qwidget with parent QWidget \a parent, if any.
 */
StartingWidget::StartingWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::StartingWidget) {
    ui->setupUi(this);
}

void StartingWidget::setInstaDam(InstaDam * id)
{
    w = id;
}

/*!
  Destructor
 */
StartingWidget::~StartingWidget() {
    delete ui;
}

/*!
  Sets the mode to run locally.
 */
void StartingWidget::runLocalVersion() {
    qInfo() << "running locally";
    InstaDam *w = new InstaDam();
    qInfo() << "InstaDam started";
    w->runningLocally = true;
    w->setButtonsConfiguration();

    w->show();
    close();

}

/*!
  Sets the mode to run of a server.
 */
void StartingWidget::runServerVersion() {

    Login *log = new Login;
    log->show();
    close();
}

/*!
  Responds to the Local Version button being clicked.
 */
void StartingWidget::on_pushButton_clicked() {
    qInfo()<<"Going to run the local version";
    runLocalVersion();
}

/*!
  Responds to the Run from Server button being clicked.
 */
void StartingWidget::on_pushButton_2_clicked() {
    runServerVersion();
}


/*!
  Exit the platform when Exit button being clicked.
 */
void StartingWidget::on_pushButton_3_clicked() {
    close();
}
