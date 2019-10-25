#include "stageonemain.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include "tetra_grip_writer.h"

#include <QApplication>

tetra_grip_api api;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    api.openSerialPort();
    QObject::connect(api.serial, SIGNAL(readyRead()), &api, SLOT(readData()));
    QObject::connect(api.serial, SIGNAL(error(QSerialPort::SerialPortError)),&api, SLOT(ErrorHandler(QSerialPort::SerialPortError))); // error handling


    StageOneMain w(nullptr);
    w.show();
    return a.exec();
}

