///////////////////////////////////////////////////////////////////
#include "startingwidget.h"
#include "login.h"
#include "instadam.h"
#include "ui_startingwidget.h"
#include<QDebug>
#include <QCoreApplication>

StartingWidget::StartingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartingWidget)
{
    ui->setupUi(this);
}

StartingWidget::~StartingWidget()
{
    delete ui;
}

void StartingWidget::runLocalVersion()
{
    qInfo() << "running locally";
    InstaDam *w = new InstaDam;
    w->runningLocally = true;
    w->show();
    on_pushButton_3_clicked();
}

void StartingWidget::runServerVersion()
{
    qInfo() << "running on a server";
    Login *log = new Login;
    log->show();
    on_pushButton_3_clicked();
}

void StartingWidget::on_pushButton_clicked()
{
    runLocalVersion();
}

void StartingWidget::on_pushButton_2_clicked()
{
    runServerVersion();
}

void StartingWidget::on_pushButton_3_clicked()
{
    close();
}
