#ifndef STIM_GUI_PROTOCOL_RX_H
#define STIM_GUI_PROTOCOL_RX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stim_gui_protocol.h"


extern uint32_t STIM_GUI_PROTOCOL_RX_PrintReceivedHexIfLessThanNBytes;
extern bool STIM_GUI_PROTOCOL_RX_DISCARD_SENSOR_DATA_PACKETS;
extern bool STIM_GUI_PROTOCOL_RX_DISCARD_TIME_PACKETS;
extern bool STIM_GUI_PROTOCOL_RX_DISCARD_PULSE_WIDTH_PACKETS;

/**
 * Initialise the receive part of the stim-gui protocol
 */
void STIM_GUI_PROTOCOL_RX_Initialise(void);

/**
 * This forms the received bytes into messages, before validating and decoding them.
 * @param buf Buffer of some more received bytes
 * @param len How many bytes are in the buffer
 */
void STIM_GUI_PROTOCOL_Process_Received_Bytes(uint8_t *buf, size_t len);

/**
 * Try to reload the stimulator configuration file (if one exists)
 * @return false if unable to complete the request at this time. Note that returning true does not mean there is a stored configuration.
 */
bool STIM_GUI_PROTOCOL_ReloadConfig(void);


#ifdef __cplusplus
}
#endif


#endif // STIM_GUI_PROTOCOL_RX_H
