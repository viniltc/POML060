#include "stagetwonew.h"
#include "ui_stagetwonew.h"
#include "stageonemain.h"

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

void StageTwoNew::on_pushButton_2_clicked()
{
    hide();
    StageOneMain *back =new StageOneMain(this);
    back ->show();
}
