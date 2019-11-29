#include "stagetwopatients.h"
#include "ui_stagetwopatients.h"
#include "stageonemain.h"


StageTwoPatients::StageTwoPatients(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StageTwoPatients)
{
    ui->setupUi(this);


    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());
}

StageTwoPatients::~StageTwoPatients()
{
    delete ui;
}

void StageTwoPatients::on_pushButton_bmain_clicked()
{
   hide();
   StageOneMain *back =new StageOneMain("",this);
   back ->show();
}

void StageTwoPatients::on_pushButton_new_clicked()
{

//    hide();
//    StageTwoNew *newone = new StageTwoNew(this);
//    newone->show();

}
