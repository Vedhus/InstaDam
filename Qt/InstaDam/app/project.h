#ifndef PROJECT_H
#define PROJECT_H
#include "../Selector/label.h"
#include <QVector>

class Project{
    private:
        QVector<QSharedPointer<Label> > labels;
        int id;
        QString name;

    public:

        Project();

        ~Project();

        QSharedPointer<Label> getLabel(int index);

        void setLabel(int index, QSharedPointer<Label> lb);

        void setId(int id);

        int getId();

        void addLabel(QSharedPointer<Label> lb);

        void resetLabels();

        int numLabels();

        void setName(QString);

        QString getName();

};

#endif // PROJECT_H
