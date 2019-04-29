#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <QVector>
#include "project.h"
#include "ui_labeldialog.h"
#include <QNetworkAccessManager>
#ifdef WASM_BUILD
#include "colordialog.h"
#endif

namespace Ui {
class newproject;
}

class newproject : public QDialog {
    Q_OBJECT

 public:
    explicit newproject(QWidget *parent = nullptr);
    ~newproject();

    Project *newPr;
    bool runningLocally = true;
    QString databaseURL;
    QString accessToken;
#ifdef TEST
    friend class IntegrationTest;
    friend class InstaDam;
    QColor mycolor;
    void setLabel(QString name, QColor color){
        tempName = name;
        mycolor = color;
        on_pushButton_clicked();
    }

#endif

 private slots:
    void on_pushButton_clicked();

    Project* on_buttonBox_accepted();
#ifdef WASM_BUILD
    void nameAcceptClicked();
    void colorPicked(const QColor &color);
#endif

    void on_buttonBox_rejected();

signals:
    void sendProject();

 private:
    Ui::newproject *ui;
    Ui::labelDialog *labelDialog;
    QString tempName;

#ifdef WASM_BUILD
    ColorDialog *colorDialog;
#endif
};

#endif  // NEWPROJECT_H
