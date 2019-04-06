#include "newproject.h"

#include <QInputDialog>
#include <QColorDialog>
#include <QTextStream>
#include <QJsonDocument>

#include "ui_newproject.h"
#include "../Selector/label.h"
#include "login.h"
#include "instadam.h"

/*!
  \class newproject
  \ingroup app
  \inmodule InstaDam
  \inherits QDialog
  \brief Creates a dialog for starting a new project.
  */

/*!
  Creates a new instance with parent QWidget \a parent, if any
  */
newproject::newproject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newproject) {
    ui->setupUi(this);
    this->newPr = new Project();
}

/*!
  Destructor
  */
newproject::~newproject() {
    delete ui;
}
#ifdef WASM_BUILD
void newproject::nameAcceptClicked() {
    tempName = labelDialog->labelName->text();
    colorDialog = new ColorDialog(Qt::black, this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog);
    connect(colorDialog, SIGNAL(colorSelected(const QColor)), this,
            SLOT(colorPicked(const QColor)));
    colorDialog->exec();
}
#endif

/*!
  Processes a button click.
  */
void newproject::on_pushButton_clicked() {
    QDialog *dialog = new QDialog(this);
    labelDialog = new Ui::labelDialog;
    labelDialog->setupUi(dialog);
#ifdef WASM_BUILD
    connect(dialog, SIGNAL(accepted()), this, SLOT(nameAcceptClicked()));
#endif
    dialog->exec();
#ifndef WASM_BUILD
    if (dialog->result() == Accepted) {
        tempName = labelDialog->labelName->text();
        QColor color = QColorDialog::getColor(Qt::black, this,
                                              "Pick label color",
                                              QColorDialog::DontUseNativeDialog);
#else
}

void newproject::colorPicked(const QColor &oldcolor) {
    QColor color = colorDialog->selectedColor();
#endif
        if (color.isValid()) {
            QSharedPointer<Label> lb = QSharedPointer<Label>::create();
            lb->setText(tempName);
            lb->setColor(color);
            this->newPr->addLabel(lb);

            QTextStream(stdout) << this->newPr->getLabel(this->newPr->numLabels()-1)->getColor().name() << endl;
            QTextStream(stdout) << this->newPr->getLabel(this->newPr->numLabels()-1)->getText() << endl;
            QTextStream(stdout) << this->newPr->numLabels() << endl;
        }
#ifndef WASM_BUILD
    }
#endif
}

/*!
  Processes the "OK" button being clicked.
  */
Project* newproject::on_buttonBox_accepted()
{
    if(this->runningLocally)
    {
        return this->newPr;
    } else {
        // start server instadam
        InstaDam *w = new InstaDam(nullptr, this->databaseURL, this->accessToken);
        w->show();
        hide();
        w->setLabels();
        w->setCurrentProject(newPr);
        hide();

        return this->newPr;
    }

}

void newproject::saveToServer(){
// create new project
qInfo() << "saving project to server";

    QUrl dabaseLink = QUrl(this->databaseURL+"/project");
    qInfo() << this->newPr->getName();

    QJsonObject js
    {
        {"project_name", this->newPr->getName()}
    };

    QJsonDocument doc(js);
    QByteArray bytes = doc.toJson();
    QNetworkRequest req = QNetworkRequest(dabaseLink);
    QString loginToken = "Bearer "+this->accessToken;
    req.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    rep = manager->post(req, bytes);

    connect(rep, &QNetworkReply::finished,
            this, &newproject::replyFinished);

}

void newproject::replyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if(jsonError.error != QJsonParseError::NoError)
    {
        qInfo() << "Error: " << jsonError.errorString();
    }

    else
    {
        QJsonObject reply = jsonReply.object();
        this->backendId = reply.value("project_id").toInt();

        // save labels
        qInfo() << "saving labels to server";

        QUrl lablesDatabaseLink = QUrl(this->databaseURL+"/project/"+QString::number(this->backendId)+"/labels");
        QString loginToken = "Bearer "+this->accessToken;

        for(int i=0; i<newPr->numLabels();i++)

        {
            QJsonObject jsLabel
            {
                {"label_name", newPr->getLabel(i)->getText()},
                {"label_color", newPr->getLabel(i)->getColor().name()}
            };

            QJsonDocument docLabel(jsLabel);
            QByteArray bytesLabel = docLabel.toJson();
            QNetworkRequest reqLabel = QNetworkRequest(lablesDatabaseLink);

            reqLabel.setRawHeader(QByteArray("Authorization"), loginToken.QString::toUtf8());
            reqLabel.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            rep = manager->post(reqLabel, bytesLabel);

            connect(rep, &QNetworkReply::finished,
                    this, &newproject::labelReplyFinished);
        }


    }
}

void newproject::labelReplyFinished()
{
    qInfo() << "reply received:";
    QByteArray strReply = rep->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonReply = QJsonDocument::fromJson(strReply, &jsonError); // parse and capture the error flag

    if(jsonError.error != QJsonParseError::NoError)
    {
        qInfo() << "Error: " << jsonError.errorString();
    }

    else
    {
        qInfo() << jsonReply;
    }
}
