#ifndef TETRA_GRIP_REPORTER_H
#define TETRA_GRIP_REPORTER_H
#include "Stim_includes/stim_gui_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

void tetra_grip_reporter(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value);
void tetra_grip_sensor_reporter(uint8_t index, SENSOR_DATA_T *sample);

#ifdef __cplusplus
}
#endif

#endif // TETRA_GRIP_REPORTER_H
