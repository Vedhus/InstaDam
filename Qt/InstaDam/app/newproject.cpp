#include "newproject.h"
#include "ui_newproject.h"
#include "../Selector/label.h"
#include <QInputDialog>
#include <QColorDialog>
#include "QTextStream"
#include "login.h"
#include "instadam.h"

newproject::newproject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newproject)
{
    ui->setupUi(this);
    this->newPr = new Project();
}

newproject::~newproject()
{
    delete ui;
}
#ifdef WASM_BUILD
void newproject::nameAcceptClicked(){
    tempName = labelDialog->labelName->text();
    colorDialog = new ColorDialog(Qt::black, this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog);
    connect(colorDialog, SIGNAL(colorSelected(const QColor)), this, SLOT(colorPicked(const QColor)));
    colorDialog->exec();
}
#endif

//void newproject::colorPicked(const QColor &color){
//    if(color.isValid()){

//        Label lb;
//        lb.setText(tempName);
//        lb.setColor(colorDialog->selectedColor());
//        this->newPr->addLabel(lb);

//        QTextStream(stdout) << this->newPr->getLabel(this->newPr->numLabels()-1)->getColor().name() << endl;
//        QTextStream(stdout) << this->newPr->getLabel(this->newPr->numLabels()-1)->getText() << endl;
//        QTextStream(stdout) << this->newPr->numLabels() << endl;
//    }
//}
void newproject::on_pushButton_clicked()
{
    QDialog *dialog = new QDialog(this);
    labelDialog = new Ui::labelDialog;
    labelDialog->setupUi(dialog);
#ifdef WASM_BUILD
    connect(dialog, SIGNAL(accepted()), this, SLOT(nameAcceptClicked()));
#endif
    dialog->exec();
#ifndef WASM_BUILD
    if(dialog->result() == Accepted){
        tempName = labelDialog->labelName->text();
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick label color",  QColorDialog::DontUseNativeDialog);
#else
}
void newproject::colorPicked(const QColor &oldcolor){
    QColor color = colorDialog->selectedColor();
#endif
        if(color.isValid()){
            QSharedPointer<Label> lb = QSharedPointer<Label>::create();
            lb->setText(tempName);
            lb->setColor(color);
            this->newPr->addLabel(lb);

            QTextStream(stdout) << this->newPr->getLabel(this->newPr->numLabels()-1)->getColor().name() << endl;
            QTextStream(stdout) << this->newPr->getLabel(this->newPr->numLabels()-1)->getText() << endl;
            QTextStream(stdout) << this->newPr->numLabels() << endl;
        }
#ifndef WASM_BUILD
    }
#endif

}

Project* newproject::on_buttonBox_accepted()
{
    if(this->runningLocally)
    {
        return this->newPr;
    }
    else
    {
        this->saveToServer();
        // start server instadam
        InstaDam *w = new InstaDam(nullptr, this->databaseURL, this->accessToken);
        w->show();
        hide();
        w->setCurrentProject(newPr);
        w->setLabels();
        hide();
        return this->newPr;

    }

}

void newproject::saveToServer(){
// create new project
qInfo() << "saving project to server";

// save labels

qInfo() << "saving labels to server";
}
