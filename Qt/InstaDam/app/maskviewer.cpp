#include "maskviewer.h"

/*!
  \class MaskViewer
  \ingroup app
  \inmodule InstaDam
  \inherits PhotoViewer
  \brief A masked view of the image in the associated PhotoViewer.
*/

/*!
  Creates an instance with parent QWidget \a parent, if any.
*/
MaskViewer::MaskViewer(QWidget *parent):PhotoViewer(parent) {
    zoom = 0;
    hasPhoto = false;
    viewerType = PhotoScene::MASK_VIEWER_TYPE;
    scene->myViewerType = viewerType;
    selectedMask = EnumConstants::THRESHOLD;
}

/*!
  Links this object to \a viewer.
*/
void MaskViewer::LinkToPhotoViewer(PhotoViewer *viewer) {
    hasPhoto = true;
    qInfo()<<"Going to set photoViewer to maskViewer";
    photoViewer = viewer;
    qInfo()<<"Going to set pixmap for mask!";
    photo->setPixmap(photoViewer->filterIm->pixmap());
    qInfo()<<"Set pixmap for mask!";
    imMask = viewer->imMask;
    cvImage = viewer->cvImage;
    cvThumb = viewer->cvThumb;
    setImMask(selectedMask);
    qInfo()<<"Set mask!";
    update();
    fitInView();
}

/*!
  Sets the image mask to \a filterName.
*/
void MaskViewer::setImMask(EnumConstants::maskTypes filterName) {

    selectedMask = filterName;
    if (hasPhoto == true) {
        pixmapFilt = filterControl->qImg.copy();
        imMask = filterControl->qAlpha.copy();
        photo->setPixmap(pixmapFilt);
    }
}
