#include "projectlist.h"
#include "ui_projectlist.h"
#include <QJsonObject>
#include <QDebug>

ProjectList::ProjectList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectList)
{
    ui->setupUi(this);
}

ProjectList::~ProjectList()
{
    delete ui;
}

void ProjectList::addItems(QJsonObject obj){
//    QList<QJsonObject> list;
    foreach(const QString& key, obj.keys()) {
            QJsonValue value = obj.value(key);
            if(value.isObject()){
                QJsonObject subObj = value.toObject();
                    foreach(const QString& k, subObj.keys()) {
                        QJsonValue val = subObj.value(k);
                        qInfo() << "Key = " << k << ", Value = " << val.toString();
                    }
            }
            else{
                qInfo() << "The returned object does not satisfy the requirements";
                qInfo() << "Server Message: Key = " << key << ", Value = " << value.toString();
            }
        }
}
