#include "imagelist.h"
#include "enumconstants.h"
#include <QFileDialog>
#include <QHttpPart>





/*!
  \class ImageList
  \ingroup app
  \inmodule InstaDam
  \inherits QSlider
  \brief Handles list of images form the server.

 */

ImageList::ImageList(Project* project, QWidget *parent, QString databaseUrl, QString token) :
    QWidget(parent), ui(new Ui::ImageList) {
    ui->setupUi(this);
    accessToken = token;
    this->databaseURL = databaseUrl;
    this->currentProject = project;

}



/*!
 * Destructor
 */

ImageList::~ImageList() {
    delete ui;
}

/*!
 * Adds Item \QJsonObject \a obj to the ImageList.
 */

void ImageList::addItems(QJsonObject obj)
{
    qInfo() << obj;
    foreach(const QString& key, obj.keys()) {
        if (key.compare("project_images") != 0) {
            qInfo() << "incorrect key";
            return;
        }
        QJsonValue value = obj.value(key);
        if (value.isArray()) {
            QJsonArray valueArray = value.toArray();
            qInfo() << valueArray;
            for (int imageNumber = 0; imageNumber < valueArray.size(); imageNumber++) {
                QJsonObject currentObj = valueArray[imageNumber].toObject();
                QTableWidget* table = ui->tableWidget;
                table->insertRow(table->rowCount());
                int column =0;
                foreach(const QString& objkey, currentObj.keys()) //Everything except the thumbnail
                {

                    QJsonValue currentValue = currentObj.value(objkey);
                    if (objkey.compare("id") == 0 || objkey.compare("name") == 0 ||
                       objkey.compare("path") == 0) {
                        if (currentValue.isDouble()) {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(QString::number(currentValue.toDouble())));
                        } else if (currentValue.isString()) {
                            table->setItem(table->rowCount()-1, column, new QTableWidgetItem(currentValue.toString()));
                        }
                        column++;
                    }
                    if(objkey.compare("is_annotated") == 0)
                    {
                        //back_end implementation added this, and we want to display it, but order in json messes up existing implemention, so hard code 4 for column value for now
                        if(currentValue.toBool())
                        {
                            table->setItem(table->rowCount()-1, 4, new QTableWidgetItem("Yes"));
                        }
                        else {
                            table->setItem(table->rowCount()-1, 4, new QTableWidgetItem("No"));
                        }
                    }
                }
                QString databaseGetProjectURL = this->databaseURL+"/image/" + QString::number(currentObj.value("id").toDouble()) + "/thumbnail";
                QUrl databaseLink = QUrl(databaseGetProjectURL);

                QUrlQuery query;

                query.addQueryItem("size_w", "150");
                query.addQueryItem("size_h", "50");

                databaseLink.setQuery(query.query());
                QNetworkRequest req = QNetworkRequest(databaseLink);
                qInfo() << req.url();
                QString loginToken = "Bearer "+this->accessToken;
                req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
                doRequest(req);
            }
        } else {
           qInfo() << "value is not an QJsonArray";
           return;
        }
    }
}

void ImageList::getThumbnailReplyFinished()
{
    qInfo() << "got a thumbnailreply";

    QByteArray strReply = rep->readAll();
    qInfo() << rep->error();
    qInfo() << rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag
    QJsonObject currentObject = jsonReply.object();
    if(jsonError.error != QJsonParseError::NoError){
        qInfo() << "Error: " << jsonError.errorString();
        return;
    }

    foreach(const QString& objkey, currentObject.keys()) //Everything except the thumbnail
    {
        QJsonValue currentValue = currentObject.value(objkey);
        //qInfo() << "Key = " << objkey << ", Value = " << currentObject.value(objkey);
        if(objkey.compare("base64_image")==0)
        {
            if(currentValue.isString())
            {
                QByteArray byteArray = QByteArray::fromBase64(currentValue.toString().toUtf8());
                QPixmap thumbnailPixmap;
                thumbnailPixmap.loadFromData(byteArray, currentObject.value("format").toString().toUtf8());
                QTableWidgetItem *thumbnail = new QTableWidgetItem;
                thumbnail->setData(Qt::DecorationRole, thumbnailPixmap);
                QTableWidget* table = ui->tableWidget;
                table->setItem(table->rowCount()-1, table->columnCount()-1, thumbnail);

            }
        }
    }
}



void ImageList::doRequest(QNetworkRequest req)
{
    rep = manager->get(req);

    connect(rep, &QNetworkReply::finished,
            this, &ImageList::getThumbnailReplyFinished);
    /* wait for reply because backend can't handle multiple requests at same time I think */
    QEventLoop loop;
    connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
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
    qInfo()<<selectedRow.size();
    QTableWidgetItem* item = selectedRow.at(2); //row has path at index 2
    qInfo() << item->text();
    QString filepath = this->databaseURL + "/" + item->text();
    qInfo() << filepath;
    QNetworkRequest req = QNetworkRequest(filepath);
    rep = manager->get(req);

    connect(rep, &QNetworkReply::finished,
            this, &ImageList::fileReplyFinished);

    close();
}


void ImageList::uploadFileReplyFinished()
{
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
    }
}

/*!
 * Slot Called when the upload button is clicked
 */
void ImageList::on_uploadButton_clicked()
{
    qInfo() << "upload button clicked";
    QString filepath = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "../", tr("Image Files (*.jpg *.png *.JPG *PNG *jpeg *JPEG );; All Files (*)"));
    QFile *file = new QFile(filepath);
    QString filename = QFileInfo(filepath).fileName();

    qInfo() << filename;
    QString databaseUploadURL = this->databaseURL+"/image/upload/" + QString::number(currentProject->getId());
    QUrl databaseLink = QUrl(databaseUploadURL);
    qInfo() << databaseLink;

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/*"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);


    multiPart->append(imagePart);


    QNetworkRequest req = QNetworkRequest(databaseLink);
    QString loginToken = "Bearer "+this->accessToken;
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data");

    qDebug() << req.url().toString();
    const QList<QByteArray>& rawHeaderList(req.rawHeaderList());
    foreach (QByteArray rawHeader, rawHeaderList) {
        qDebug() << req.rawHeader(rawHeader);
    }


    rep = manager->post(req, multiPart);
    multiPart->setParent(rep);

    connect(rep, &QNetworkReply::finished,
            this, &ImageList::uploadFileReplyFinished);

}


void ImageList::openAnnotation()
{
    if(selectedRow.at(3)->text().compare("Yes")==0) //the image being loaded has annotations associated with it, load the annotations with it
    {
        qInfo() << "inside openAnnotation";
        //note that selectedRow should be defined before using this function
        if (selectedRow.isEmpty())
            qInfo()<<"Image row was never selected";
        else {
            QString image_id = selectedRow[0]->text();
            for (int i = 1; i < currentProject->numLabels(); i++) {
                QString label_id = QString::number(currentProject->getLabel(i)->getId());
                QString databaseGetAnnotationURL = this->databaseURL+"/annotation/"+label_id+"/"+image_id+"/";
                qInfo()<<databaseGetAnnotationURL;


                QNetworkRequest annotationRequest = QNetworkRequest(databaseGetAnnotationURL);
                QString loginToken = "Bearer "+this->accessToken;
                annotationRequest.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
                annotationReplies[i-1] = manager->get(annotationRequest);
                connect(annotationReplies[i-1], &QNetworkReply::finished,
                        this, &ImageList::annotationReplyFinished);

                QEventLoop loop;
                connect(annotationReplies[i-1], SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();


            }
        }
    }
}

void ImageList::annotationReplyFinished()
{
    QNetworkReply* currentReply = annotationReplies[numAnnotationsReceived];
    numAnnotationsReceived+=1;
    qInfo() << "Got an annotation";
    qInfo() << currentReply->request().url();
    if (currentReply->error()) { //http error 400 means the annotation does not exist
        qInfo() << currentReply->error();
        qInfo() << currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        return;
    }

    QByteArray strReply = currentReply->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if (jsonError.error != QJsonParseError::NoError) {
        qInfo() << "Error: " << jsonError.errorString();
    }
    else {
        qInfo() << jsonReply;
        QJsonObject jsonAnnotation = jsonReply.object();
        jsonLabelArray.append(jsonAnnotation);

    }
    json["labels"] = jsonLabelArray;
    emit allAnnotationsLoaded(json, ANNOTATION);
}






/*!
 * Slot called when the cancel button is clicked.
 */
void ImageList::on_cancelButton_clicked() {
    close();
}
