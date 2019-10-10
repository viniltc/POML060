#include "tetra_grip_reader.h"
#include "tetra_grip_api.h"

void readData()
{
    const QByteArray data = api.serial->readAll();
}
