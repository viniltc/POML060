#include "stageonemain.h"
#include "tetra_grip_api.h"

#include <QApplication>

tetra_grip_api api;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    api.openSerialPort();


    api.serial->connect(api.serial, SIGNAL(readyRead()), &api, SLOT(readData()));
    api.serial->connect(api.serial, &QSerialPort::readyRead, &api, &tetra_grip_api::readData);

    StageOneMain w(nullptr);
    w.show();
    return a.exec();
}

