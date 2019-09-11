#ifndef STIM_ENGINE_H
#define STIM_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>


#ifdef NO_SPI
#define NO_STIM_ENGINES
#endif 

typedef enum {
    STIM_ENGINE_REG_FIRMWARE_VERSION=0,
    STIM_ENGINE_REG_HARDWARE_VERSION,
    STIM_ENGINE_REG_ECHO,
    STIM_ENGINE_REG_COMMAND_ERROR_CODE,
    STIM_ENGINE_REG_VSTIM_REQUIRED,
    STIM_ENGINE_REG_VSTIM_MEASURED_A,
    STIM_ENGINE_REG_VSTIM_MEASURED_B,
    STIM_ENGINE_REG_STATUS_MASK,
    STIM_ENGINE_REG_COMMAND_STATUS,
    STIM_ENGINE_REG_TRIGGER_EVENT,
    STIM_ENGINE_REG_PHASE_NUMBER,
    STIM_ENGINE_REG_INSTANTANEOUS_PULSE_WIDTH,
    STIM_ENGINE_REG_SET_PULSE_WIDTH,
    STIM_ENGINE_REG_PULSE_WIDTH_MODIFIER,
    STIM_ENGINE_REG_TIME,
    STIM_ENGINE_REG_MICROSECONDS,
    STIM_ENGINE_REG_MEASURED_CURRENT,
    STIM_ENGINE_REG_VSTIM_OFFSET,
    STIM_ENGINE_REG_VSTIM_GAIN,
    STIM_ENGINE_REG_MAX_CURRENT,
    STIM_ENGINE_REG_MAX_PULSE_WIDTH,
    STIM_ENGINE_REG_MIN_PULSE_WIDTH,
    STIM_ENGINE_REG_MAX_FREQUENCY,
    STIM_ENGINE_REG_GENERAL_OPTIONS,
    STIM_ENGINE_REG_EEPROM_USE,
    STIM_ENGINE_REG_TEST_TRAIN_TIMEOUT,
    STIM_ENGINE_REG_PHASE_POINTER,
    STIM_ENGINE_REG_RAMP_PARAMETER,
    STIM_ENGINE_REG_WAVEFORM,
    STIM_ENGINE_REG_TARGET_PULSE_WIDTH,
    STIM_ENGINE_REG_TARGET_CURRENT,
    STIM_ENGINE_REG_TARGET_FREQUENCY,
    STIM_ENGINE_REG_DURATION,
    STIM_ENGINE_REG_RAMP_RATE,
    STIM_ENGINE_REG_SALFORD_DELAY,
    STIM_ENGINE_REG_GOTO_PHASE_ON_DURATION_TARGET,
    STIM_ENGINE_REG_TRIGGER_SETUP,
    STIM_ENGINE_REG_DESTINATION_ON_TRIGGER1,
    STIM_ENGINE_REG_DESTINATION_ON_TRIGGER2,
    STIM_ENGINE_REG_DESTINATION_ON_TRIGGER3,
    NUM_STIM_ENGINE_REGS /* Must be last */
} STIM_ENGINE_REG_ID;

// Data about the registers on the engine.
typedef enum { RD_REG=1, WR_REG=2, RD_WR=3} REGTYPE_E;

typedef struct {
    uint8_t addr;
    uint8_t length;
    uint32_t min;
    uint32_t max;
    REGTYPE_E type;
} STIM_ENGINE_REG_T;

extern const STIM_ENGINE_REG_T reg_data[NUM_STIM_ENGINE_REGS];

typedef enum {
    STIM_ENGINE_CMD_REBOOT=0,
    STIM_ENGINE_CMD_GOTO_BOOTLOADER=1,
    STIM_ENGINE_CMD_PERFORM_CALIBRATION=2,
    STIM_ENGINE_CMD_PERFORM_SELF_TEST=3,
    STIM_ENGINE_CMD_CLEAR_STIMULATION_CONFIG=4,
    STIM_ENGINE_CMD_VALIDATE_STIMULATION_CONFIG=5,
    STIM_ENGINE_CMD_PAUSE=6,
    STIM_ENGINE_CMD_UNPAUSE=7,
    STIM_ENGINE_CMD_START_TEST_STIMULATION_FOR_CURRENT_PHASE=8,
    STIM_ENGINE_CMD_STOP_TEST_STIMULATION=9,
    STIM_ENGINE_CMD_SOUNDER_ON=10,
    STIM_ENGINE_CMD_SOUNDER_OFF=11,
    STIM_ENGINE_CMD_SAVE_CALIBRATION=12,
    STIM_ENGINE_CMD_RESTORE_CALIBRATION=13,
    STIM_ENGINE_CMD_START_SYNC=14,
} STIM_ENGINE_CMD_ID;

typedef enum {
    STIM_ENGINE_StatusPaused=1<<0,
    STIM_ENGINE_StatusStimulation_envelope_active=1<<1,
    STIM_ENGINE_StatusStimulation_pulse_active=1<<2,
    STIM_ENGINE_Status_Stimulation_timed_out=1<<3,
    STIM_ENGINE_Status_Command_error_since_last_read=1<<4,
    STIM_ENGINE_Status_Vstim_failure=1<<5,
    STIM_ENGINE_Status_Fault_input_active=1<<6,
    STIM_ENGINE_Status_Reached_target_PW=1<<7,
    STIM_ENGINE_Status_Calibrated=1<<8,
    STIM_ENGINE_Status_Self_test_passed_OK=1<<9,
    STIM_ENGINE_Status_Configuration_validation_OK=1<<10,
} STIM_ENGINE_STATUS_BITS_T;

typedef enum {
            STIM_ENGINE_GENERAL_OPTIONS_SALISBURY_MODE=0,
            STIM_ENGINE_GENERAL_OPTIONS_SALFORD_MODE=1,
                    
            STIM_ENGINE_GENERAL_OPTIONS_TIMEOUT_FROM_TARGET_LEVEL=0,
            STIM_ENGINE_GENERAL_OPTIONS_TIMEOUT_FROM_TRIGGER=2,
                    
            STIM_ENGINE_GENERAL_OPTIONS_NO_AUTO_PHASE_CHANGE=0<<2,
            STIM_ENGINE_GENERAL_OPTIONS_EXERCISE_MODE=1<<2,
            STIM_ENGINE_GENERAL_OPTIONS_PROCESS_TRIGGERS=2<<2,
            STIM_ENGINE_GENERAL_OPTIONS_RESERVED=3<<2,
                    
            STIM_ENGINE_GENERAL_OPTIONS_SOUNDER_DISABLED=0,
            STIM_ENGINE_GENERAL_OPTIONS_SOUNDER_ENABLED=1<<4
} STIM_ENGINE_GENERAL_OPTIONS_ENUM;

typedef enum {
            STIM_ENGINE_RAMP_PARAMETER_PULSE_WIDTH=0,
            STIM_ENGINE_RAMP_PARAMETER_CURRENT=1,
            STIM_ENGINE_RAMP_PARAMETER_FREQUENCY=2,
} STIM_ENGINE_RAMP_PARAMETER_ENUM;

typedef enum {
            STIM_ENGINE_WAVEFORM_ASYM=0<<0,
            STIM_ENGINE_WAVEFORM_SYM=1<<0,

            STIM_ENGINE_WAVEFORM_POSITIVE=0<<1,
            STIM_ENGINE_WAVEFORM_NEGATIVE=1<<1,

            STIM_ENGINE_WAVEFORM_STANDARD=0<<2,
            STIM_ENGINE_WAVEFORM_TRUE_BIPHASIC=1<<2,
} STIM_ENGINE_WAVEFORM_ENUM;

#define STIM_ENGINE_DURATION_NO_TIMEOUT (0xFFFF)

#ifdef PIC32

#include "stim_engine_comms.h"

/**
 * Write a value to a register on a stim engine
 * @param engine Which stim engine
 * @param reg Which register
 * @param data Pointer to the value
 * @return 
 */
bool STIM_ENGINE_WriteRegister(STIM_ENGINE_ID_T engine, STIM_ENGINE_REG_ID reg, void *data);

/**
 * Write data to an address on a stim engine
 * @param engine Which stim engine
 * @param address Which address
 * @param length How many bytes
 * @param data Pointer to the data
 * @return 
 */
bool STIM_ENGINE_Write(STIM_ENGINE_ID_T engine, uint8_t addr, uint8_t length, void *data);

/**
 * Read a value from a register on a stim engine
 * @param engine Which stim engine
 * @param reg Which register
 * @return True if read request was queued
 */
bool STIM_ENGINE_ReadRegister(STIM_ENGINE_ID_T engine, STIM_ENGINE_REG_ID reg);

/**
 * Read data from an address on a stim engine
 * @param engine Which stim engine
 * @param addr Which address
 * @param length How many bytes
 * @return True if read request was queued
 */
bool STIM_ENGINE_Read(STIM_ENGINE_ID_T engine, uint8_t addr, uint8_t length);

/**
 * Decode any responses received from the stim engines and mark the jobs as free.
 */
void STIM_ENGINE_ProcessResponses(void);

/**
 * This should be used as the callback function for jobs that invoke read responses
 * The job is added to a linked list for processing in mainline code.
 * @param job
 */
void STIM_ENGINE_NoteReadResponse(STIM_ENGINE_JOB_T *job);

/**
 * Read a stim engine register and check that the value is as expected.
 * @param engine Which stim engine.
 * @param reg Which register.
 * @param data Pointer to the expected value.
 * @return True if the register was read and matched the expected value.
 */
bool STIM_ENGINE_BlockingReadAndValidateRegister(STIM_ENGINE_ID_T engine, STIM_ENGINE_REG_ID reg, void *data, void *mask);

/**
 * Set up the stim engines according to the stimulators current configuration file
 * @return True if set up successfully.
 */
bool STIM_ENGINE_ConfigurePhases(void);

/**
 * Directly move all engines to the given phase
 * @param phase
 * @return True if phase changed OK.
 */
bool STIM_ENGINE_GotoPhase(uint8_t phase);

bool STIM_ENGINE_AnyEngineHasAnError(void);

bool STIM_ENGINE_AllEnginesAreUnpaused(void);

bool STIM_ENGINE_AllEnginesArePaused(void);

void STIM_ENGINE_AssumeAllEnginesArePausedWithNoError(void);

#endif // PIC32

#ifdef __cplusplus
}
#endif

#endif
