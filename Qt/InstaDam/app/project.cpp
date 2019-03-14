#include "project.h"

Project::Project()
{
}

Project::~Project()
{
}

QSharedPointer<Label> Project::getLabel(int index)
{
    return labels[index];
}

void Project::setLabel(int index, QSharedPointer<Label> lb)
{
    labels[index] = lb;
}

void Project::addLabel(QSharedPointer<Label> lb)
{
    labels.append(lb);
}

void Project::resetLabels()
{
    QVector<QSharedPointer<Label> > labels;
}

int Project::numLabels()
{
    return labels.size();
}

