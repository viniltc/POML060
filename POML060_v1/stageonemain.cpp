#include "stageonemain.h"
#include "ui_stageonemain.h"

StageOneMain::StageOneMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StageOneMain)
{
    ui->setupUi(this);
}

StageOneMain::~StageOneMain()
{
    delete ui;
}
