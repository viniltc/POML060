#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include<QDebug>


stageProgram::stageProgram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);
    ui->pushButton_currOnOne->setCheckable(true);


    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);
}

stageProgram::~stageProgram()
{
    delete ui;
}


void stageProgram::setCurrOnChannelOne(QString value)
{
    unsigned int channel_number = 0;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelTwo(QString value)
{
    unsigned int channel_number = 1;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelThree(QString value)
{
    unsigned int channel_number = 2;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelFour(QString value)
{
    unsigned int channel_number = 3;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::setCurrOnChannelFive(QString value)
{
    unsigned int channel_number = 4;
    unsigned int current_uA = value.toUInt();
    tetra_grip_api::stimulation_set_current(channel_number, current_uA);
}

void stageProgram::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);
}

void stageProgram::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);
}
