#ifndef STIM_GUI_PROTOCOL_TX_H
#define STIM_GUI_PROTOCOL_TX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stim_gui_protocol.h"

/**
 * Initialise the transmit part of the stim-gui protocol
 */
void STIM_GUI_PROTOCOL_TX_Initialise(void);

/**
 * Send a message using the Stim-GUI protocol.
 * @param destination_device
 * @param source_device
 * @param blocks One or more linked short register blocks 
 */
bool STIM_GUI_Send_message(const uint8_t destination_device, const uint8_t source_device, STIM_GUI_MESSAGE_S_BLOCK_T *block);

/**
 * Send a single long block using the Stim-GUI protocol.
 * @param destination_device
 * @param source_device
 * @param block Pointer to one long register block
 * @return True if message was sent
 */
bool STIM_GUI_Send_long_message(const uint8_t destination_device, const uint8_t source_device, STIM_GUI_MESSAGE_L_BLOCK_T *block);

#ifdef __cplusplus
}
#endif

#endif // STIM_GUI_PROTOCOL_TX_H
