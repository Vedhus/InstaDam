#include "newproject.h"
#include <QInputDialog>
#include <QColorDialog>
#include <QTextStream>
#include <QJsonDocument>
#include "ui_newproject.h"
#include "../Selector/label.h"
#include "login.h"
#include "instadam.h"

/*!
  \class newproject
  \ingroup app
  \inmodule InstaDam
  \inherits QDialog
  \brief Creates a dialog for starting a new project.
  */

/*!
  Creates a new instance with parent QWidget \a parent, if any
  */
newproject::newproject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newproject) {
#ifndef TEST
    ui->setupUi(this);
    this->newPr = new Project();
#else
    this->newPr = new Project();

#endif
}

/*!
  Destructor
  */
newproject::~newproject() {
    delete ui;
}
#ifdef WASM_BUILD
void newproject::nameAcceptClicked() {
    tempName = labelDialog->labelName->text();
    colorDialog = new ColorDialog(Qt::black, this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog);
    connect(colorDialog, SIGNAL(colorSelected(const QColor)), this,
            SLOT(colorPicked(const QColor)));
    colorDialog->exec();
}
#endif

/*!
  Processes the add label (OK) button click.
  */
void newproject::on_pushButton_clicked() {
#ifndef TEST
    QDialog *dialog = new QDialog(this);
    labelDialog = new Ui::labelDialog;
    labelDialog->setupUi(dialog);
#ifdef WASM_BUILD
    connect(dialog, SIGNAL(accepted()), this, SLOT(nameAcceptClicked()));
#endif  // WASM_BUILD
    dialog->exec();
#ifndef WASM_BUILD
    if (dialog->result() == Accepted) {
        tempName = labelDialog->labelName->text();
        QColor color = QColorDialog::getColor(Qt::black, this,
                                              "Pick label color",
                                              QColorDialog::DontUseNativeDialog);
#else  // WASM_BUILD
}

void newproject::colorPicked(const QColor &oldcolor) {
    QColor color = colorDialog->selectedColor();
#endif  // WASM_BUILD
#else  // TEST
    QColor color = mycolor;
    {
#endif  //TEST
        if (color.isValid()) {
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

/*!
  Processes the "OK" button being clicked.
  */
Project* newproject::on_buttonBox_accepted()
{
    emit sendProject();
    return this->newPr;
}

/*!
  Processes the "Cancel" button being clicked.
  */
void newproject::on_buttonBox_rejected()
{
    this->close();
}
