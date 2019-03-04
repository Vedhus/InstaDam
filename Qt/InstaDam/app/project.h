#ifndef PROJECT_H
#define PROJECT_H
#include "../Selector/label.h"
#include <QVector>

class Project{
    private:
        QVector<Label> labels;

    public:

        Project();

        ~Project();

        Label *getLabel(int index);

        void setLabel(int index, Label lb);

        void addLabel(Label lb);

        void resetLabels();

        int numLabels();

};

#endif // PROJECT_H
