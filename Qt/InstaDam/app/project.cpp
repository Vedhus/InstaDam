#include "project.h"

Project::Project() {
}

Project::~Project() {
}

QSharedPointer<Label> Project::getLabel(int index) const {
    return labels[index];
}

void Project::setLabel(int index, QSharedPointer<Label> lb) {
    labels[index] = lb;
}

void Project::addLabel(QSharedPointer<Label> lb) {
    labels.append(lb);
}

void Project::resetLabels() {
    labels.clear();
}

int Project::numLabels() const {
    return labels.size();
}

void Project::setId(int id)
{
    this->id = id;
}

int Project::getId()
{
    return this->id;
}

