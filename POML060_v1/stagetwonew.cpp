#include "stagetwonew.h"
#include "ui_stagetwonew.h"

StageTwoNew::StageTwoNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StageTwoNew)
{
    ui->setupUi(this);
}

StageTwoNew::~StageTwoNew()
{
    delete ui;
}
