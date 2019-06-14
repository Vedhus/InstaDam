#include "debuginsta.h"
#include "ui_instadam_copy.h"

DebugInsta::DebugInsta(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::DebugInsta)
{
    ui->setupUi(this);
}

DebugInsta::~DebugInsta()
{
    delete ui;
}
