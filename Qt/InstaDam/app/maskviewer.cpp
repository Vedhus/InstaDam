#include "maskviewer.h"

/*!
  \class MaskViewer
  \ingroup InstaDam
  \inmodule InstaDam
  \inherits PhotoViewer
  \brief A masked view of the image in the associated PhotoViewer
  */

/*!
  Creates an instance with parent QWidget \a parent, if any.
  */
MaskViewer::MaskViewer(QWidget *parent):PhotoViewer(parent) {
    zoom = 0;
    hasPhoto = false;
    viewerType = PhotoScene::MASK_VIEWER_TYPE;
    scene->myViewerType = viewerType;
    selectedMask = CANNY;
}

/*!
  Links this object to \a viewer.
  */
void MaskViewer::LinkToPhotoViewer(PhotoViewer *viewer) {
    hasPhoto = true;
    photoViewer = viewer;
    photo->setPixmap(photoViewer->filterIm->pixmap());
    imMask = viewer->imMask;
    cvImage = viewer->cvImage;
    cvThumb = viewer->cvThumb;
    setImMask(selectedMask);
    update();
    fitInView();
}

/*!
  Sets the image mask to \a filterName.
  */
void MaskViewer::setImMask(maskTypes filterName) {
    selectedMask = filterName;
    if (hasPhoto == true) {
        pixmapFilt = filterControl->qImg.copy();
        imMask = filterControl->qAlpha.copy();
        photo->setPixmap(pixmapFilt);
    }
}
