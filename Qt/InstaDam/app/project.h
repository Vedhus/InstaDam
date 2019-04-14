#ifndef PROJECT_H
#define PROJECT_H

#include <QVector>

#include "../Selector/label.h"

class Project{
 private:
    QVector<QSharedPointer<Label> > labels;
    int id;
    QString name;
    int image_id = -1;

 public:
    Project();

    ~Project();

    QSharedPointer<Label> getLabel(int index) const;

    void setLabel(int index, QSharedPointer<Label> lb);

    void setLabels(QVector<QSharedPointer<Label> > lab) {labels = lab;}

    void addLabel(QSharedPointer<Label> lb);

    void setId(int id);

    void setImageId(int id);

    int getImageId();


    int getId();

    QVector<QSharedPointer<Label> > getLabels() const {return labels;}

    void resetLabels();

    int numLabels() const;

    void setName(QString);

    QString getName();

#ifdef TEST
    friend class IntegrationTest;
#endif
};

#endif  // PROJECT_H
