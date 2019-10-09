#include "tetra_grip_writer.h"
#include "tetra_grip_api.h"

size_t tetragrip_writer(const char * source, size_t nBytes)
{
    qint64 number_of_bytes_written = 0;
    if(api.serial)
    {
        number_of_bytes_written = api.serial->write(source, (qint64)nBytes);
        api.serial->flush();
    }
    return (size_t)number_of_bytes_written;
}

