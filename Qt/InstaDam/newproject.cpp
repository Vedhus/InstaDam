#include "newproject.h"
#include "ui_newproject.h"
#include "label.h"
#include <QInputDialog>
#include <QColorDialog>
#include "QTextStream"

newproject::newproject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newproject)
{
    ui->setupUi(this);
}

newproject::~newproject()
{
    delete ui;
}


void newproject::on_pushButton_clicked()
{
    QString text = QInputDialog::getText(this, tr("Add a new label"),
                                         tr("New Label"), QLineEdit::Normal, "enter label name");
    QColor color = QColorDialog::getColor(Qt::black, this, "Pick label color",  QColorDialog::DontUseNativeDialog);

    Label lb;
    lb.setText(text);
    lb.setColor(color);
    this->newPr.addLabel(lb);

    QTextStream(stdout) << this->newPr.getLabel(this->newPr.numLabels()-1).getColor().name() << endl;
    QTextStream(stdout) << this->newPr.getLabel(this->newPr.numLabels()-1).getText() << endl;
    QTextStream(stdout) << this->newPr.numLabels() << endl;
}

Project newproject::on_buttonBox_accepted()
{
    return this->newPr;
}