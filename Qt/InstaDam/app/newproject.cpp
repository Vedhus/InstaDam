#include "newproject.h"

#include <QInputDialog>
#include <QColorDialog>
#include <QTextStream>

#include "ui_newproject.h"
#include "../Selector/label.h"

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
    ui->setupUi(this);
    this->newPr = new Project();
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
  Processes a button click.
  */
void newproject::on_pushButton_clicked() {
    QDialog *dialog = new QDialog(this);
    labelDialog = new Ui::labelDialog;
    labelDialog->setupUi(dialog);
#ifdef WASM_BUILD
    connect(dialog, SIGNAL(accepted()), this, SLOT(nameAcceptClicked()));
#endif
    dialog->exec();
#ifndef WASM_BUILD
    if (dialog->result() == Accepted) {
        tempName = labelDialog->labelName->text();
        QColor color = QColorDialog::getColor(Qt::black, this,
                                              "Pick label color",
                                              QColorDialog::DontUseNativeDialog);
#else
}

void newproject::colorPicked(const QColor &oldcolor) {
    QColor color = colorDialog->selectedColor();
#endif
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
Project* newproject::on_buttonBox_accepted() {
    return this->newPr;
}
