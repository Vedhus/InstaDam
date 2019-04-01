#include "pixmapops.h"


QPixmap testPixmap(int r = 255,int g = 0,int b = 0, int a = 255)
{
    QPixmap px = QPixmap(200,200);
    QColor color  = QColor(r,g,b,a);
    px.fill(color);
    return px;
}

QPixmap maskPixmaps(QPixmap p1, QPixmap p2, QPixmap mask, brushTypes brushType)
{
    QPixmap maskp2 = joinPixmaps(mask, p2, QPainter::CompositionMode_SourceIn);
    return directPixmaps(p1,maskp2, brushType);

}

QPixmap directPixmaps(QPixmap p1, QPixmap p2, brushTypes brushType)
{
    QPainter::CompositionMode mode;
    if (brushType == ERASER)
        mode = QPainter::CompositionMode_DestinationOut;
    else
        mode = QPainter::CompositionMode_SourceOver;
    return joinPixmaps(p1, p2, mode);
}

QPixmap joinPixmaps(QPixmap p1, QPixmap p2, QPainter::CompositionMode mode= QPainter::CompositionMode_SourceOver)
{

    QPixmap result =  QPixmap(p1.size().expandedTo(p2.size()));
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(QPoint(), p1);
    painter.setCompositionMode(mode);
    painter.drawPixmap(result.rect(), p2, p2.rect());
    painter.end();
    return result;
}


//Source: https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap/
// If inImage exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inImage's
// data with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
//    NOTE: This does not cover all cases - it should be easy to add new ones as required.
cv::Mat QImageToCvMat(QImage inImage, bool inCloneImageData = true )
{
   switch ( inImage.format() )
   {
      // 8-bit, 4 channel
      case QImage::Format_ARGB32:
      case QImage::Format_ARGB32_Premultiplied:
      {
         cv::Mat  mat( inImage.height(), inImage.width(),
                       CV_8UC4,
                       const_cast<uchar*>(inImage.bits()),
                       static_cast<size_t>(inImage.bytesPerLine())
                       );

         return (inCloneImageData ? mat.clone() : mat);
      }

      // 8-bit, 3 channel
      case QImage::Format_RGB32:
      {
         if ( !inCloneImageData )
         {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
         }

         cv::Mat  mat( inImage.height(), inImage.width(),
                       CV_8UC4,
                       const_cast<uchar*>(inImage.bits()),
                       static_cast<size_t>(inImage.bytesPerLine())
                       );

         cv::Mat  matNoAlpha;

         cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

         return matNoAlpha;
      }

      // 8-bit, 3 channel
      case QImage::Format_RGB888:
      {
         if ( !inCloneImageData )
         {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
         }

         QImage   swapped = inImage.rgbSwapped();

         return cv::Mat( swapped.height(), swapped.width(),
                         CV_8UC3,
                         const_cast<uchar*>(swapped.bits()),
                         static_cast<size_t>(swapped.bytesPerLine())
                         ).clone();
      }

      // 8-bit, 1 channel
      case QImage::Format_Indexed8:
      {
         cv::Mat  mat( inImage.height(), inImage.width(),
                       CV_8UC1,
                       const_cast<uchar*>(inImage.bits()),
                       static_cast<size_t>(inImage.bytesPerLine())
                       );

         return (inCloneImageData ? mat.clone() : mat);
      }

      default:
         qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
         break;
   }

   return cv::Mat();
}

// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
// with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
//cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true )
//{
//   return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
//}




