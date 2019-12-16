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
    ui->pushButton_currOnTwo->setCheckable(true);
    ui->pushButton_currOnThree->setCheckable(true);
    ui->pushButton_currOnFour->setCheckable(true);
    ui->pushButton_currOnFive->setCheckable(true);


    ui->widget_currentOne->setEnabled(false);
    ui->widget_currentTwo->setEnabled(false);
    ui->widget_currentThree->setEnabled(false);
    ui->widget_currentFour->setEnabled(false);
    ui->widget_currentFive->setEnabled(false);

    connect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::eventHandler);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);

    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);
}

stageProgram::~stageProgram()
{
    delete ui;
}


void stageProgram::setCurrOnChannelOne(QString value)
{
    unsigned int channel_number = 0;


    unsigned int current_uA=value.toUInt();


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

void stageProgram::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_ACTIVITY_STATUS:

            if(value==true)
            {
                 ui->pushButton_stimStop->setText("Stop");
                 ui->pushButton_stimStart->setText("Running");
            }
            else
            {
                 ui->pushButton_stimStop->setText("Stopped");
                 ui->pushButton_stimStart->setText("Start");
            }
            break;
        }
    }

}
