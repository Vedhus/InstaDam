#include "maskviewer.h"

MaskViewer::MaskViewer(QWidget *parent):PhotoViewer(parent)
{
    zoom = 0;
    hasPhoto = false;
    viewerType = MASK_VIEWER_TYPE;
    selectedMask = CANNY;

}
void MaskViewer::LinkToPhotoViewer(PhotoViewer *viewer)
{
    hasPhoto = true;
    photoViewer = viewer;
    photo->setPixmap(photoViewer->filterIm->pixmap());
    labels = photoViewer->labels;
    labels->setPixmap(photoViewer->labels->pixmap());
    labelsTemp = photoViewer->labelsTemp;
    imMask = viewer->imMask;
    cvImage = viewer->cvImage;
    cvThumb = viewer->cvThumb;
    setImMask(selectedMask);
    update();
    fitInView();


}


void MaskViewer::setImMask(maskTypes filterName)
{
    selectedMask = filterName;
    if (hasPhoto == true)
    {

        pixmapFilt = filterControl->qImg.copy();
        imMask = filterControl->qAlpha.copy();
        photo->setPixmap(pixmapFilt);
    }
}


      


