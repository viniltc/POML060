#include "stageonemain.h"
#include "ui_stageonemain.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"

StageOneMain::StageOneMain(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::StageOneMain)
{
    ui->setupUi(this);

    //connect(api.serial, SIGNAL(readyRead()), &api, SLOT(readData()));


    connect(api.serial, SIGNAL(readyRead()), this, SLOT(serialReceived())); // dummy label to test raw serial data

    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());



}

StageOneMain::~StageOneMain()
{
    delete ui;
}

//extern "C" size_t StageOneMain::send_using_qtserial(uint8_t *data, size_t len)
//{
//    //return serial->write((const char *)data, (qint64)len);
//}

void StageOneMain::on_pushButton_exit_clicked()
{
    QApplication::quit();
    //tetra_grip_api::send_config_file()
}

void StageOneMain::on_pushButton_patients_clicked()
{

    hide();
    stagetwopatients = new StageTwoPatients(this);
    stagetwopatients -> show();
}

void StageOneMain::on_pushButton_programs_clicked()
{
    hide();
    stageprogram = new stageProgram(this);
    stageprogram -> show();
}

void StageOneMain::serialReceived()
{

    ui->label->setText(api.serial->readAll());
}
