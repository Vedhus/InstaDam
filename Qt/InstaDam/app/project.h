#ifndef PROJECT_H
#define PROJECT_H
#include "../Selector/label.h"
#include <QVector>

class Project{
    private:
        QVector<QSharedPointer<Label> > labels;

    public:

        Project();

        ~Project();

        QSharedPointer<Label> getLabel(int index);

        void setLabel(int index, QSharedPointer<Label> lb);

        void addLabel(QSharedPointer<Label> lb);

        void resetLabels();

        int numLabels();

};

#endif // PROJECT_H
