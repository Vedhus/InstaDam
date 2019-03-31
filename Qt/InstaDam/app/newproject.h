#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <QVector>
#include "project.h"
#include "ui_labeldialog.h"
#ifdef WASM_BUILD
#include "colordialog.h"
#endif

namespace Ui {
class newproject;
}

class newproject : public QDialog
{
    Q_OBJECT

public:
    explicit newproject(QWidget *parent = nullptr);
    ~newproject();

    Project *newPr;

private slots:
    void on_pushButton_clicked();

    Project* on_buttonBox_accepted();
#ifdef WASM_BUILD
    void nameAcceptClicked();
    void colorPicked(const QColor &color);
#endif

private:
    Ui::newproject *ui;
    Ui::labelDialog *labelDialog;
    QString tempName;
#ifdef WASM_BUILD
    ColorDialog *colorDialog;
#endif
};

#endif // NEWPROJECT_H
