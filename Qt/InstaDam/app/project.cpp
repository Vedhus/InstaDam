#include "project.h"

/*!
  \class Project
  \ingroup InstaDam
  \inmodule InstaDam
  \brief Holds project related information.
  */

/*!
  Creates an instance
  */
Project::Project() {
}

/*!
  Destructor.
  */
Project::~Project() {
}

/*!
  Returns the Label at index \a index.
  */
QSharedPointer<Label> Project::getLabel(int index) const {
    return labels[index];
}

/*!
  Sets the label at indes \a index to \a lb.
  */
void Project::setLabel(int index, QSharedPointer<Label> lb) {
    labels[index] = lb;
}

/*!
  Adds Label \a lb to the internal list.
  */
void Project::addLabel(QSharedPointer<Label> lb) {
    labels.append(lb);
}

/*!
  Clears the internal list of Labels.
  */
void Project::resetLabels() {
    labels.clear();
}

/*!
  Returns the current number of Labels in the internal list.
  */
int Project::numLabels() const {
    return labels.size();
}

/*!
  Sets the id of this instance to \a id.
  */
void Project::setId(int id) {
    this->id = id;
}

/*!
  Returns the id of this instance.
  */
int Project::getId() {
    return this->id;
}
