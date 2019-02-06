#ifndef INSTADAM_H
#define INSTADAM_H

#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

int static const maxNumLabels = 20;

class Label{
  private:
    QColor color;
    QString text;

  public:
    QColor getColor(){
        return color;
    }

    void setColor(QColor col){
        color = col;
    }

    QString getText(){
        return text;
    }

    void setText(QString tx){
        text = tx;
    }
};

class Project{
    private:
//        QString name;
        Label labels[maxNumLabels];

    public:

        Label getLabel(int index){
            return labels[index];
        }

        void setLabel(int index, Label lb){
            labels[index] = lb;
        }

//        QString getName(){
//            return name;
//        }

//        void setName(QString nme){
//            name = nme;
//        }

};

namespace Ui {
class InstaDam;
}

class InstaDam : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstaDam(QWidget *parent = nullptr);
    ~InstaDam();

private slots:
    Project on_actionNew_triggered();

    Project on_actionOpen_triggered();

    void on_actionSave_triggered();

private:
    Ui::InstaDam *ui;
};




#endif // INSTADAM_H
