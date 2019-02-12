#ifndef PROJECT_H
#define PROJECT_H
#include "label.h"
#include <Qvector>

class Project{
    private:
        QVector<Label> labels;

    public:

        Project();

        ~Project();

        Label getLabel(int index);

        void setLabel(int index, Label lb);

        void addLabel(Label lb);

        int numLabels();

};

#endif // PROJECT_H
