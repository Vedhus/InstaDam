#ifndef PROJECT_H
#define PROJECT_H
#include "../Selector/label.h"
#include <QVector>

class Project{
    private:
        QVector<QSharedPointer<Label> > labels;
        int id;

    public:

        Project();

        ~Project();

        QSharedPointer<Label> getLabel(int index);

        void setLabel(int index, QSharedPointer<Label> lb);

        void setId(int id);

        int getId();

        void addLabel(QSharedPointer<Label> lb);

        void setLabels(QVector<QSharedPointer<Label> > &newLabels){labels = newLabels;}

        QVector<QSharedPointer<Label> > getLabels(){return labels;}

        void resetLabels();

        int numLabels();

};

#endif // PROJECT_H
