#include "project.h"
#include "pixmapops.h"
#include "cnpy.h"
#include "QProgressDialog"
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

/*! Exports labels of files in the images list to .npz files
 * */

void Project::exportNpzLocal(QVector<int> originalLabelIds,\
                    QVector<int> saveLabelIds,
                    QStringList classNames,\
                    InstaDam* id){
    QProgressDialog progress("Exporting to NPZ...", "Abort", 0, id->imagesList.size() );
    progress.setWindowModality(Qt::WindowModal);

    for (int fileNum = 0; fileNum<id->imagesList.size(); fileNum++){
        progress.setValue(fileNum);
        if (progress.wasCanceled())
            break;

        QString filename = id->path.absolutePath()+"/"+id->imagesList[id->fileId];
        QFileInfo file = QFileInfo(filename);

        QString baseName = file.baseName();

        QString aPath = id->path.absolutePath()+"/annotations/"+baseName+"/";
        QString ePath = id->path.absolutePath()+"/exportNPZ/";


        if (!QDir(aPath).exists()) {
            continue;
        }


        if (!QDir(ePath).exists()) {
            QDir().mkpath(ePath);
        }

        QString annotationPath = aPath+baseName+".idantn";
        QString exportPath = ePath+baseName+".npz";

        QPixmap photo = QPixmap(filename);
        SelectItem::myBounds = photo.size();
        qInfo()<<annotationPath;

        if (QFileInfo(annotationPath).isFile()) {
                exportNpz(originalLabelIds, saveLabelIds,
                          exportPath.toStdString(), classNames,
                          photo);
        }
    id->saveAndProgress(1,false);


    }

}

/*!
 Saves the current annotations as Npz or as a buffer depending on the value of
 \a asBuffers
*/
void Project::exportNpz(QVector<int> originalLabels, QVector<int> newLabels,
                        std::string saveName, QStringList classNames,
                        QPixmap photo, int imbool) {
    int max = *std::max_element(newLabels.constBegin(), newLabels.constEnd());
    QVector<QPixmap> exportPixmaps(max);
    exportPixmaps.reserve(max);

    for (int i = 0; i<exportPixmaps.size(); i++){
        exportPixmaps[i] = QPixmap(SelectItem::myBounds);
        exportPixmaps[i].fill(Qt::transparent);
    }

    for (int i = 0; i <numLabels(); i++) {

        if(newLabels[i]>0){
            QPixmap exportedChannel = labels[i]->exportLabel(SelectItem::myBounds);
            exportPixmaps[newLabels[i]-1] = joinPixmaps(exportPixmaps[newLabels[i]-1],
                    exportedChannel);
        }

    }
    //std::vector<uchar> dataBlock;

    for (int i = 0; i<exportPixmaps.size()+imbool; i++){
        int k;
        QImage image_pixmap;

        if (i<exportPixmaps.size())
        {
            image_pixmap = QImage(exportPixmaps[i].toImage().convertToFormat(QImage::Format_RGB888));
        }
        else {
            image_pixmap = QImage(photo.toImage().convertToFormat(QImage::Format_RGB888));
        }

        cv::Mat mat3(image_pixmap.height(), image_pixmap.width(),
                    CV_8UC3, (uchar*)image_pixmap.bits(),image_pixmap.bytesPerLine());
        cv::Mat mat;
        cv::cvtColor(mat3, mat, cv::COLOR_RGB2GRAY);
        cv::threshold(mat, mat, 1,255, cv::THRESH_BINARY);
        std::vector<uchar> array;
        if (mat.isContinuous()) {
            qInfo()<<"Continuous";
          array.assign(mat.data, mat.data + mat.total());
        } else {
          for (int i = 0; i < mat.rows; ++i) {
            array.insert(array.end(), mat.ptr<uchar>(i), mat.ptr<uchar>(i)+mat.cols);
          }
        }




    std::string writeOrAppend;
    if (i==0){
        writeOrAppend = "w"    ;
    }
    else {
        writeOrAppend = "a";
    }

    cnpy::npz_save(saveName,classNames[i].toStdString(),\
                   &array[0],{SelectItem::myBounds.height(),
                SelectItem::myBounds.width()},writeOrAppend);


    }

}






/*!
  \fn void Project::setLabels(QVector<QSharedPointer<Label> > lab)

  Set all of the labels in the Project with the given \a lab.
*/

/*!
  \fn QVector<QSharedPointer<Label> > Project::getLabels() const

  Returns all the Labels in the Project as a QVector.
*/

