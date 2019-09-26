#include "stageonemain.h"
#include "ui_stageonemain.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


StageOneMain::StageOneMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StageOneMain)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());

//    serial = new QSerialPort();
//    serial->setPortName("com5");
//    serial->setBaudRate(1000000); // baudrate 1000000 ..1M
//    serial->setDataBits(QSerialPort::Data8);
//    serial->setParity(QSerialPort::NoParity);
//    serial->setStopBits(QSerialPort::OneStop);
//    serial->setFlowControl(QSerialPort::HardwareControl); //Hardware flow control (RTS/CTS), NoFlowControl, SoftwareControl
//    serial->open(QIODevice::ReadWrite);

    connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecived()));


}

StageOneMain::~StageOneMain()
{
    delete ui;
}


void StageOneMain::on_pushButton_exit_clicked()
{
    QApplication::quit();
    tetra_grip_api::send_config_file()
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
