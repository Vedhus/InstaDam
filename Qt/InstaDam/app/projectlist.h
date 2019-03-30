#ifndef PROJECTLIST_H
#define PROJECTLIST_H

#include <QWidget>

namespace Ui {
class ProjectList;
}

class ProjectList : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectList(QWidget *parent = nullptr);
    ~ProjectList();
    void addItems(QJsonObject);

private:
    Ui::ProjectList *ui;

};

#endif // PROJECTLIST_H
