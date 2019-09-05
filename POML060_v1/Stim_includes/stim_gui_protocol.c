#include "stim_gui_protocol.h"
#include "stim_gui_protocol_tx.h"
#include "stim_gui_protocol_rx.h"
#include "uart.h"
#include "hardware.h"
#include "stim_regs.h"

uint8_t STIM_GUI_PROTOCOL_ThisStimulatorID=1;

/**
 * Initialise the STIM-GUI protocol
 * @param uart Which UART to use for stim-GUI communications.
 */
void STIM_GUI_PROTOCOL_Initialise()
{
    UART_Reset(PLIB_USART_INDEX_GUI);
    
    STIM_REGS_Init();
    STIM_GUI_PROTOCOL_TX_Initialise();
    STIM_GUI_PROTOCOL_RX_Initialise();
}
