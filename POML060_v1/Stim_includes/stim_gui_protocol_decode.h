#ifndef STIM_GUI_PROTOCOL_DECODE_H
#define STIM_GUI_PROTOCOL_DECODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stim_gui_protocol.h"

extern uint16_t milliseconds;
extern uint32_t seconds;

/**
 * Decode and print a short data block
 * @param b Pointer to the block
 */
void STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock(STIM_GUI_MESSAGE_S_BLOCK_T *b);

#ifdef __cplusplus
}
#endif

#endif // STIM_GUI_PROTOCOL_DEBUG_H
