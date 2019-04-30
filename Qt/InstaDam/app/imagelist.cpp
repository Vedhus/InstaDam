#include "imagelist.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QHttpPart>
#include <QMessageBox>
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "enumconstants.h"

/*!
  \class ImageList
  \ingroup app
  \inmodule InstaDam
  \inherits QSlider
  \brief Handles list of images form the server.

*/

/*!
  Creates an ImageList instance with \a project, \a parent, \a databaseUrl,
  and \a token
*/
ImageList::ImageList(Project* project, QWidget *parent, QString databaseUrl,
                     QString token) :
    QWidget(parent), ui(new Ui::ImageList) {
    ui->setupUi(this);
    accessToken = token;
    this->databaseURL = databaseUrl;
    this->currentProject = project;
}

/*!
  Destructor
*/
ImageList::~ImageList() {
    delete ui;
}

#ifdef WASM_BUILD
void ImageList::getThumbnailReplyFin(QNetworkReply* reply){
    rep = reply;
    getThumbnailReplyFinished();
}
void ImageList::imagesReplyFin(QNetworkReply* reply){
    rep = reply;
    imagesReplyFinished();
}
void ImageList::uploadFileReplyFin(QNetworkReply* reply){
    rep = reply;
    uploadFileReplyFinished();
}
void ImageList::annotationReplyFin(QNetworkReply* reply){
    rep = reply;
    annotationReplyFinished();
}
#endif

/*!
  Returns the idList.
*/
QList<int> ImageList::getIdList() {
    return idList;
}

/*!
  Returns the pathList.
*/
QList<QString> ImageList::getPathList() {
    return pathList;
}

/*!
  Returns selectedIdIndex.
*/
int ImageList::getSelectedIdIndex() {
    return selectedIdIndex;
}

/*!
  Sets the selectedIdIndex annotated flag to \c true.
*/
void ImageList::setAnnotated() {
    annotatedList[selectedIdIndex] = true;
}

/*!
  Set the selectedIdIndex ti \a id.
*/
void ImageList::setSelectedIdIndex(int id) {
    selectedIdIndex = id;
}

/*!
 * Adds Item QJsonObject \a obj to the ImageList.
*/
void ImageList::addItems(QJsonObject obj) {
    qInfo() << obj;
    idList.clear();
    annotatedList.clear();
    pathList.clear();
    foreach(const QString& key, obj.keys()) {
        if (key.compare(InstaDamJson::PROJECT_IMAGES) != 0) {
            qInfo() << "incorrect key";
            return;
        }
        QJsonValue value = obj.value(key);
        if (value.isArray()) {
            QJsonArray valueArray = value.toArray();
            qInfo() << valueArray;
            QTableWidget* table = ui->tableWidget;
            table->setRowCount(0);
            for (int imageNumber = 0; imageNumber < valueArray.size(); imageNumber++) {
                QJsonObject currentObj = valueArray[imageNumber].toObject();
                table->insertRow(table->rowCount());
                int column =0;
                foreach(const QString& objkey, currentObj.keys()) {  //Everything except the thumbnail
                    QJsonValue currentValue = currentObj.value(objkey);
                    if (objkey.compare(InstaDamJson::ID) == 0 || objkey.compare(InstaDamJson::NAME) == 0 ||
                       objkey.compare(InstaDamJson::PATH) == 0) {
                        if (currentValue.isDouble()) {
                            idList.push_back(static_cast<int>(currentValue.toDouble()));
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(QString::number(currentValue.toDouble())));
                        } else if (currentValue.isString()) {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(currentValue.toString()));
                        }
                        column++;
                    }
                    if(objkey.compare(InstaDamJson::PATH)==0) {
                        pathList.push_back(currentValue.toString());
                    }
                    if(objkey.compare(InstaDamJson::IS_ANNOTATED) == 0) {
                        //back_end implementation added this, and we want to display it, but order in json messes up existing implemention, so hard code 4 for column value for now
                        if(currentValue.toBool()) {
                            table->setItem(table->rowCount()-1, 4, new QTableWidgetItem("Yes"));
                            annotatedList.push_back(true);
                        } else {
                            table->setItem(table->rowCount()-1, 4, new QTableWidgetItem("No"));
                            annotatedList.push_back(false);
                        }
                    }
                }
                qInfo() << idList;
                QString databaseGetProjectURL = this->databaseURL + "/" +
                        InstaDamJson::IMAGE + "/" +
                        QString::number(currentObj.value(InstaDamJson::ID).toDouble()) +
                        "/" + InstaDamJson::THUMBNAIL;
                QUrl databaseLink = QUrl(databaseGetProjectURL);

                QUrlQuery query;

                query.addQueryItem(InstaDamJson::SIZE_W, "150");
                query.addQueryItem(InstaDamJson::SIZE_H, "50");

                databaseLink.setQuery(query.query());
                QNetworkRequest req = QNetworkRequest(databaseLink);
                QString loginToken = InstaDamJson::BEARER + this->accessToken;
                req.setRawHeader(InstaDamJson::AUTHORIZATION,
                                 loginToken.QString::toUtf8());
                doRequest(req);
            }
        } else {
           qInfo() << "value is not an QJsonArray";
           return;
        }
        qInfo() << pathList;
    }
}

/*!
  Slot called when thumbnail is received over network
*/
void ImageList::getThumbnailReplyFinished() {
    QByteArray strReply = rep->readAll();
    qInfo() << rep->error();
    qInfo() << rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    QJsonObject currentObject = jsonReply.object();
    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
        return;
    }

    foreach(const QString& objkey, currentObject.keys()) {//Everything except the thumbnail
        QJsonValue currentValue = currentObject.value(objkey);
        //qInfo() << "Key = " << objkey << ", Value = " << currentObject.value(objkey);
        if (objkey.compare(InstaDamJson::BASE64_IMAGE)==0) {
            if(currentValue.isString()) {
                QByteArray byteArray = QByteArray::fromBase64(currentValue.toString().toUtf8());
                QPixmap thumbnailPixmap;
                thumbnailPixmap.loadFromData(byteArray, currentObject.value(InstaDamJson::FORMAT).toString().toUtf8());
                QTableWidgetItem *thumbnail = new QTableWidgetItem;
                thumbnail->setData(Qt::DecorationRole, thumbnailPixmap);
                QTableWidget* table = ui->tableWidget;
                table->setItem(table->rowCount()-1, table->columnCount()-1, thumbnail);
            }
        }
    }
}

/*!
  Performs the network request.
*/
void ImageList::doRequest(QNetworkRequest req) {
    rep = manager->get(req);
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(getThumbnailReplyFin(QNetworkReply*)));

    /* wait for reply because backend can't handle multiple requests at same time I think */
    QEventLoop loop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
#else
    connect(rep, &QNetworkReply::finished,
            this, &ImageList::getThumbnailReplyFinished);
    /* wait for reply because backend can't handle multiple requests at same time I think */
    QEventLoop loop;
    connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
#endif
    loop.exec();
}

/*!
 * Waits for the file to be received.
*/
void ImageList::fileReplyFinished() {
    qInfo() << "got a file";
    if (rep->error()) {
        qInfo() << "error getting file";
    }
    QUrl url = rep->url();
    qInfo() << url;
    QString path = QFileInfo(url.path()).fileName();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qInfo() << "could not save to disk";
    }

    file.write(rep->readAll());
    file.close();
    qInfo() << selectedRow.at(0)->text().toInt();
    currentProject->setImageId(selectedRow.at(0)->text().toInt());
    emit fileDownloaded(path);
}

/*!
 * Slot called when the load button is clicked.
*/
void ImageList::on_loadButton_clicked() {
    qInfo() << "load button clicked";
    QTableWidget* table = ui->tableWidget;
    selectedRow = table->selectedItems();
    if(selectedRow.empty()) {
        QMessageBox *messageBox = new QMessageBox;
        messageBox->critical(nullptr, "Error", "Select an Image to Load first");
        messageBox->setFixedSize(500, 200);
        return;
    }
    selectedIdIndex = selectedRow.first()->row(); //save the id index selected, used for save/back, save/next
    QTableWidgetItem* item = selectedRow.at(2); //row has path at index 2
    QString filepath = this->databaseURL + "/" + item->text();
    QNetworkRequest req = QNetworkRequest(filepath);
    rep = manager->get(req);
#ifdef WASM_BUILD
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(fileReplyFin(QNetworkReply*)));
#else
    connect(rep, &QNetworkReply::finished,
            this, &ImageList::fileReplyFinished);
#endif
    emit clearGUI();
    close();
}

/*!
  Slot called when images are received over the network.
*/
void ImageList::imagesReplyFinished() {
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError);  // parse and capture the error flag

      if (jsonError.error != QJsonParseError::NoError) {
            qInfo() << "Error: " << jsonError.errorString();
      } else {
          QJsonObject obj = jsonReply.object();
          addItems(obj);
      }
}

/*!
  Slot called when file upload is completed.
*/
void ImageList::uploadFileReplyFinished() {
    qInfo() << "upload reply";
    if (rep->error()) { //http error 400 means the annotation does not exist
        qInfo() << rep->error();
        qInfo() << rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    }

    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    }
    else {
        qInfo() << jsonReply;

        QString databaseImagesURL = this->databaseURL + "/" +
                InstaDamJson::PROJECTS + "/" +
                QString::number(currentProject->getId()) + "/" +
                InstaDamJson::IMAGES;
        QUrl dabaseLink = QUrl(databaseImagesURL);

        qInfo() << databaseImagesURL;

        QNetworkRequest req = QNetworkRequest(dabaseLink);
        req.setRawHeader(InstaDamJson::AUTHORIZATION,
                         InstaDamJson::BEARER + this->accessToken.toUtf8());
        rep = manager->get(req);
#ifdef WASM_BUILD
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(imagesReplyFin(QNetworkReply*)));
#else
        connect(rep, &QNetworkReply::finished,
                this, &ImageList::imagesReplyFinished);
#endif
    }
}

/*!
  Zips files given by \a files together, returning \c true on success and \c false
  on error.
*/
bool ImageList::zipFilesTogether(QFileInfoList files) {
    QFile inFile;
    QuaZip zip("images.zip");
    if (!zip.open(QuaZip::mdCreate)) {
        qInfo() << QString("testCreate(): zip.open(): %1").arg(zip.getZipError());
        return false;
    }

    QuaZipFile outFile(&zip);
    char c;
    foreach(QFileInfo fileInfo, files) {
        if(!fileInfo.isFile())
            continue;
        qInfo() << fileInfo.filePath();
        qInfo() << fileInfo.fileName();
        qInfo() << fileInfo.dir();
        QString fileNameWithRelativePath = fileInfo.filePath().remove(0, fileInfo.dir().absolutePath().length() + 1);

        inFile.setFileName(fileInfo.filePath());
        if (!inFile.open(QIODevice::ReadOnly)) {
           qInfo() <<  QString("testCreate(): inFile.open(): %1").arg(inFile.errorString().toLocal8Bit().constData());
           return false;
        }

        if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath()))) {
          qInfo() << QString("testCreate(): outFile.open(): %1").arg(outFile.getZipError());
           return false;
        }

        while (inFile.getChar(&c) && outFile.putChar(c));

        if (outFile.getZipError() != UNZ_OK) {
           qInfo() << QString("testCreate(): outFile.putChar(): %1").arg(outFile.getZipError());
           return false;
        }

        outFile.close();

        if (outFile.getZipError() != UNZ_OK) {
          qInfo() << QString("testCreate(): outFile.close(): %1").arg(outFile.getZipError());
          return false;
        }

        inFile.close();
    }


   zip.close();

   if (zip.getZipError() != 0) {
      qInfo() << QString("testCreate(): zip.close(): %1").arg(zip.getZipError());
      return false;
   }
   return true;
}

/*!
 * Slot Called when the upload button is clicked
*/
void ImageList::on_uploadButton_clicked() {
    qInfo() << "upload button clicked";
    QStringList filelist = QFileDialog::getOpenFileNames(this,
        tr("Open Image"), "../", tr("Image Files (*.jpg *.png *.JPG *PNG *jpeg *JPEG );; All Files (*)"));
    if(filelist.size()==0)
        return;

    if(filelist.size()==1)
    {
        QFile *file = new QFile(filelist[0]);
        QString filename = QFileInfo(filelist[0]).fileName();

        qInfo() << filename;
        QString databaseUploadURL = this->databaseURL+"/image/upload/" + QString::number(currentProject->getId());
        QUrl databaseLink = QUrl(databaseUploadURL);
        qInfo() << databaseLink;

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/*"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\"; filename=\"" + filename+  "\""));
        file->open(QIODevice::ReadOnly);
        imagePart.setBodyDevice(file);
        file->setParent(multiPart);


        multiPart->append(imagePart);


        QNetworkRequest req = QNetworkRequest(databaseLink);
        QString loginToken = InstaDamJson::BEARER + this->accessToken;
        req.setRawHeader(InstaDamJson::AUTHORIZATION,
                         loginToken.QString::toUtf8());

        qDebug() << req.url().toString();
        const QList<QByteArray>& rawHeaderList(req.rawHeaderList());
        foreach (QByteArray rawHeader, rawHeaderList) {
            qDebug() << req.rawHeader(rawHeader);
        }

        rep = manager->post(req, multiPart);
        multiPart->setParent(rep);

#ifdef WASM_BUILD
        connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(uploadFileReplyFin(QNetworkReply*)));

#else
        connect(rep, &QNetworkReply::finished,
                this, &ImageList::uploadFileReplyFinished);
#endif

    }
    else //multiple files, upload zip
    {
        //https://stackoverflow.com/questions/2598117/zipping-a-folder-file-using-qt

        QFileInfoList files;
        foreach(QString file, filelist)
            files << QFileInfo(file);

       if (zipFilesTogether(files)) {
           qInfo() << "files successfully zipped (there were no errors)" << endl;
           QString databaseUploadURL = this->databaseURL + "/image/upload/zip/" +
                   QString::number(currentProject->getId());
           QUrl databaseLink = QUrl(databaseUploadURL);
           qInfo() << databaseLink;

           QFile *file = new QFile("images.zip");

           QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
           QHttpPart zipPart;
           zipPart.setHeader(QNetworkRequest::ContentTypeHeader,
                             QVariant(InstaDamJson::APPLICATIONJSON));
           zipPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"zip\"; filename=\"images.zip\""));
           qInfo() << "open: " << file->open(QIODevice::ReadOnly);
           zipPart.setBodyDevice(file);
           file->setParent(multiPart);

           multiPart->append(zipPart);

           QNetworkRequest req = QNetworkRequest(databaseLink);
           QString loginToken = InstaDamJson::BEARER + this->accessToken;
           req.setRawHeader(InstaDamJson::AUTHORIZATION,
                            loginToken.QString::toUtf8());

           rep = manager->post(req, multiPart);
           multiPart->setParent(rep);
#ifdef WASM_BUILD
           connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                   SLOT(uploadFileReplyFin(QNetworkReply*)));
#else
           connect(rep, &QNetworkReply::finished,
                   this, &ImageList::uploadFileReplyFinished);
#endif
       }
    }
}

/*!
  Opens the annotation file.
*/
void ImageList::openAnnotation() {
    qInfo() << "openAnnotation project id: " << currentProject->getId();
    if (annotatedList[selectedIdIndex]) { //the image being loaded has annotations associated with it, load the annotations with it
        qInfo() << "inside openAnnotation";
        //note that selectedRow should be defined before using this function
        if (selectedRow.isEmpty()) {
            qInfo()<<"Image row was never selected";
        } else {
            QString image_id = QString::number(idList[selectedIdIndex]);

            QString databaseGetAnnotationURL = this->databaseURL + "/" +
                    InstaDamJson::ANNOTATION + "/" + image_id + "/";
            qInfo()<<databaseGetAnnotationURL;

            QNetworkRequest annotationRequest = QNetworkRequest(databaseGetAnnotationURL);
            QString loginToken = InstaDamJson::BEARER + this->accessToken;
            annotationRequest.setRawHeader(InstaDamJson::AUTHORIZATION,
                                           loginToken.QString::toUtf8());
            rep = manager->get(annotationRequest);
#ifdef WASM_BUILD
            connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                    SLOT(annotationReplyFin(QNetworkReply*)));

            QEventLoop loop;
            connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
#else
            connect(rep, &QNetworkReply::finished,
                    this, &ImageList::annotationReplyFinished);

            QEventLoop loop;
            connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
#endif
            loop.exec();
        }
    }
}

/*!
  Slot called when annotation is received over the network.
*/
void ImageList::annotationReplyFinished() {
    qInfo() << "annotationReply project id: " << currentProject->getId();
    qInfo() << "Got an annotation";
    qInfo() << rep->request().url();
    if (rep->error()) { //http error 400 means the annotation does not exist
        qInfo() << rep->error();
        qInfo() << rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    }

    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    } else {
        jsonLabelArray = jsonReply.array();
        QJsonArray wrapperArray;
        for (int i =0; i< jsonLabelArray.size();i++) {
            qInfo() << jsonLabelArray[i].toObject();
            wrapperArray.append(jsonLabelArray[i].toObject());
        }
        json["labels"] = wrapperArray;
        emit allAnnotationsLoaded(json, ANNOTATION);
    }
}

/*!
 * Slot called when the cancel button is clicked.
*/
void ImageList::on_cancelButton_clicked() {
    close();
}

/*!
  \fn void ImageList::fileDownloaded(QString path);

  Emitted whne a file has been downloded, giving the \a path.
  */

/*!
  \fn void ImageList::allAnnotationsLoaded(QJsonObject json,fileTypes type);

  Emitted when all annotations have been loaded from \a json, with \a type.
  */

/*!
  \fn void ImageList::clearGUI();

  Emitted when the GUI is cleared.
  */

