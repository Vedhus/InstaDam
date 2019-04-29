#ifndef CHOOSELABELDIALOG_H
#define CHOOSELABELDIALOG_H


#include <QDialog>

#include <vector>

#include "Selector/label.h"
#include "labelButton.h"
#include "project.h"
#include "enumconstants.h"

class PhotoViewer;

class chooseLabelDialog: public QDialog {
    Q_OBJECT
 public:
    chooseLabelDialog(Project *currentPro);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint mpos;
    QVector<LabelButtonFilter *> labelButtons;
public slots:
    void checkLabel(QSharedPointer<Label> label);
signals:
    void labelPicked(QSharedPointer<Label> label);
};



#endif // CHOOSELABELDIALOG_H
