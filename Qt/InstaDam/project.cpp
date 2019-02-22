#include "project.h"

Project::Project()
{
}

Project::~Project()
{
}

Label Project::getLabel(int index)
{
    return labels[index];
}

void Project::setLabel(int index, Label lb)
{
    labels[index] = lb;
}

void Project::addLabel(Label lb)
{
    labels.append(lb);
}

void Project::resetLabels()
{
    QVector<Label> labels;
}

int Project::numLabels()
{
    return labels.size();
}

