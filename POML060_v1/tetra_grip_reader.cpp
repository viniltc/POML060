#include "tetra_grip_reader.h"
#include "tetra_grip_api.h"
#include "Stim_includes/stim_gui_protocol_rx.h"

void readData()
{
    #define UART_PC_BUFFER_SIZE (65536);
    #define UART_BUFFER_SIZE (4096)
    uint8_t buf[UART_BUFFER_SIZE];

    const QByteArray data = api.serial->readAll();

    STIM_GUI_PROTOCOL_Process_Received_Bytes((uint8_t*)data.data(), (size_t)data.length());
}
