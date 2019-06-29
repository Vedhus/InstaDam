#include "project.h"

/*!
  \class Project
  \ingroup app
  \inmodule InstaDam
  \brief Holds project related information.
*/

/*!
  Creates an instance of Project.
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

/*!
  Set the name of the Project to \a name.
*/
void Project::setName(QString name) {
    this->name = name;
}

/*!
  Returns the name of the Project.
*/
QString Project::getName() {
    return this->name;
}

/*!
  Set the image_id of the Project to \a id.
*/
void Project::setImageId(int id) {
    image_id = id;
}

/*!
  Returns the image_id of the Project.
*/
int Project::getImageId() {
    return image_id;
}

/*!
  Clear all the labels in the Project.
*/
void Project::clearAllLabels() {
    for (int i = 0; i < this->getLabels().size(); i++) {
        this->getLabel(i)->clear();
    }
   // labels.clear();

}

/*!
  \fn void Project::setLabels(QVector<QSharedPointer<Label> > lab)

  Set all of the labels in the Project with the given \a lab.
*/

/*!
  \fn QVector<QSharedPointer<Label> > Project::getLabels() const

  Returns all the Labels in the Project as a QVector.
*/
