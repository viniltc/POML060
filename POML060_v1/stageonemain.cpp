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

    serial = new QSerialPort();
    serial->setPortName("com5");
    serial->setBaudRate(1000000); // baudrate 1000000 ..1M
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::HardwareControl); //Hardware flow control (RTS/CTS), NoFlowControl, SoftwareControl
    serial->open(QIODevice::ReadWrite);

    connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecived()));

//    QPixmap pix1("://resources/bu.jpg"); // this is the code to add image to the form
//    int w1 = ui->label_bu->width();
//    int h1 = ui->label_bu->height();
//    ui->label_bu->setPixmap(pix1.scaled(w1,h1,Qt::KeepAspectRatio));

//    QPixmap pix2("://resources/inspire.jpg"); // this is the code to add image to the form
//    int w2 = ui->label_inp->width();
//    int h2 = ui->label_inp->height();
//    ui->label_inp->setPixmap(pix2.scaled(w2,h2,Qt::KeepAspectRatio));

//    QPixmap pix3("://resources/oml.png"); // this is the code to add image to the form
//    int w3 = ui->label_oml->width();
//    int h3 = ui->label_oml->height();
//    ui->label_oml->setPixmap(pix3.scaled(w3,h3,Qt::KeepAspectRatio));

}

StageOneMain::~StageOneMain()
{
    delete ui;
}


void StageOneMain::on_pushButton_exit_clicked()
{
    QApplication::quit();
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
