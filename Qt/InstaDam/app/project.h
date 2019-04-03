#ifndef PROJECT_H
#define PROJECT_H

#include <QVector>

#include "../Selector/label.h"

class Project{
 private:
    QVector<QSharedPointer<Label> > labels;

 public:
    Project();

    ~Project();

    QSharedPointer<Label> getLabel(int index) const;

    void setLabel(int index, QSharedPointer<Label> lb);

    void addLabel(QSharedPointer<Label> lb);

    void setLabels(QVector<QSharedPointer<Label> > &newLabels) {
        labels = newLabels;
    }

    QVector<QSharedPointer<Label> > getLabels() const {return labels;}

    void resetLabels();

    int numLabels() const;

};

#endif  // PROJECT_H
