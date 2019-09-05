#ifndef SENSOR_MESSAGE_FORMAT_H
#define SENSOR_MESSAGE_FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define COBS_LEN_WITH_OVERHEAD(x) (1+(x)+(x)/254)

#define MAX_NUM_SENSORS (12)
#define MAX_SENSOR_DATA_LEN (245)
#define MAX_SENSOR_MESSAGE_LEN (4+COBS_LEN_WITH_OVERHEAD(MAX_SENSOR_DATA_LEN+2)+1) /* null + addr + reg + len + COBS_len_with_overhead + null */
#define SENSOR_GENERAL_MESSAGE_TIMEOUT_ms   (20)                /* Maximum time to wait for a general message response */
#define SENSOR_STANDARD_POLL_TIMEOUT_ms     (3)                 /* Maximum time to wait for a standard poll message response */
#define SENSOR_UNASSIGNED_POLL_TIMEOUT_ms   1/3 /*no brackets*/ /* Maximum time to wait for the sepcial poll of an unasigned address */
#define SENSOR_MESSAGE_TIMEOUTS_AND_ERRORS_PERMITTED (5) /* Before sensor is condsidered disconnected */

#define SENSOR_ADDRESS_UNUSED (0)
#define SENSOR_ADDRESS_OF_STIMULATOR (13)
#define SENSOR_ADDRESS_NOT_ALLOCATED (14)
#define SENSOR_ADDRESS_BROADCAST (15)
#define SENSOR_ADDRESSES_RESERVED_FOR_WIRELESS (1) /* Addresses SENSOR_ADDRESS_OF_STIMULATOR-SENSOR_ADDRESSES_RESERVED_FOR_WIRELESS to SENSOR_ADDRESS_OF_STIMULATOR-1*/

typedef enum {
    SENSOR_MSG_TYPE_RESERVED=0,
    SENSOR_MSG_TYPE_WRITE=1,
    SENSOR_MSG_TYPE_READ_REQUEST=2,
    SENSOR_MSG_TYPE_POLL_REQUEST=3,
    SENSOR_MSG_TYPE_WRITE_THEN_READ=4,
    SENSOR_MSG_TYPE_EMPTY=5,
    SENSOR_MSG_TYPE_READ_RESPONSE=6,
    SENSOR_MSG_TYPE_POLL_RESPONSE=7,
} SENSOR_MSG_TYPE_T;

typedef enum {START_POS=0, ADDR_TYPE_POS=1, REG_POS=2, LEN_POS=3, DATA_POS=4} SENSOR_MSG_FIELD_POS_T;

typedef enum {
    SENSOR_REG_MODEL_ID=1,
    SENSOR_REG_HARDWARE_VERSION=2,
    SENSOR_REG_FIRMWARE_VERSION=3,
    SENSOR_REG_LAST_HARDWARE_ERROR_CODE=4,
    SENSOR_REG_LAST_COMMAND_ERROR_CODE=5,
    SENSOR_REG_STATUS=6,
    SENSOR_REG_EXTENDED_COMMAND_STATUS=7,
    SENSOR_REG_UNIQUE_UNIT_ID=8,
    SENSOR_REG_TIME_SECONDS=12,
    SENSOR_REG_TIME_MICROSECONDS=16,
    SENSOR_REG_POWER_CYCLE_COUNT=20,
    SENSOR_REG_HOURS_OF_USE=22,
    SENSOR_REG_SUPPLY_VOLTAGE=24,
    SENSOR_REG_BATTERY_VOLTAGE=25,
    SENSOR_REG_BATTERY_CAPACITY_REMAINING=26,
    SENSOR_REG_BATTERY_MINUTES_REMAINING=27,
    SENSOR_REG_GENERAL_OPTIONS=28,
    SENSOR_REG_POWER_MODE=29,
    SENSOR_REG_TEMPERATURE=30,
    SENSOR_REG_COMMAND=31,
    SENSOR_REG_SENSOR_OPTIONS=32,
    SENSOR_REG_TRIGGER_OPTIONS=33,
    SENSOR_REG_STREAMING_OPTIONS=34,
    SENSOR_REG_UPDATE_RATE=35,
    SENSOR_REG_STREAMING_ADDRESS=36,
    SENSOR_REG_STREAMING_LENGTH=37,
    SENSOR_REG_STREAMING_DATA=128,
    SENSOR_REG_BUILD_VERSION_INFO=255,
} SENSOR_REG_ID_T;

// Values for SENSOR_REG_MODEL_ID register
typedef enum {
    SENSOR_MODEL_ID_FESUPP_WIRED_MOVEMENT_SENSOR=0,
    SENSOR_MODEL_ID_FESUPP_WIRELESS_MOVEMENT_SENSOR=1,
    SENSOR_MODEL_ID_2=2,
    SENSOR_MODEL_ID_EMG=3,
    SENSOR_MODEL_ID_FESUPP_INSTRUMENTED_OBJECT=4,
    SENSOR_MODEL_ID_FESUPP_FSR=5,
    SENSOR_MODEL_ID_FESUPP_BUTTON=6,
    SENSOR_MODEL_ID_FESUPP_OML_WFSW=7,
    SENSOR_MODEL_ID_GONIOMETER_SINGLE_AXIS=8,
    SENSOR_MODEL_ID_GONIOMETER_DUAL_AXIS=9,
    NUM_SENSOR_MODEL_IDS=10,
} SENSOR_MODEL_ID_T;

// Values for SENSOR_REG_COMMAND register
typedef enum {
    SENSOR_CMD_RESET=0,
    SENSOR_CMD_GOTO_BOOTLOADER=1,
    SENSOR_CMD_POWER_OFF=2,
    SENSOR_CMD_SLEEP=3,
    SENSOR_CMD_AUTO_CALIBRATE=4,
    SENSOR_CMD_SELF_TEST=5,
    SENSOR_CMD_ASSIGN_ADDRESS=6,
    SENSOR_CMD_SET_CONFIGURED_BIT=7,
    SENSOR_CMD_AV_INDICATOR_OFF=8,
    SENSOR_CMD_AV_INDICATOR_ON=9,
    SENSOR_CMD_BUS_POWER_OFF=10,
    SENSOR_CMD_BUS_POWER_ON=11,
    SENSOR_CMD_FREEZE_SAMPLE=16,
} SENSOR_COMMAND_ID_T;

typedef enum {
    SENSOR_STATUS_NODE_FAILURE=1,
    SENSOR_STATUS_COMMAND_ERROR=2,
    SENSOR_STATUS_BUSY=4,
    SENSOR_STATUS_ACTIVE=8,
    SENSOR_STATUS_DATA_READY=16,
    SENSOR_STATUS_TRIGGERED=32,
    SENSOR_STATUS_BUTTON_STATUS=64,
    SENSOR_STATUS_CONFIGURED_OK=128,
} SENSOR_STATUS_T;

typedef enum {
    SENSOR_EXTENDED_STATUS_CALIBRATION_INVALID=1,
    SENSOR_EXTENDED_STATUS_EEPROM_INVALID=2,
    SENSOR_EXTENDED_STATUS_HARDWARE_FAILURE=4,
    SENSOR_EXTENDED_STATUS_LOW_VOLTAGE=8,
    SENSOR_EXTENDED_STATUS_BATTERY_WORN_OUT=16,
} SENSOR_EXTENDED_STATUS_T;

typedef enum {
    SENSOR_MESSAGE_UNUSED=0,
    SENSOR_MESSAGE_UNQUEUED,
    SENSOR_MESSAGE_DELAYED,
    SENSOR_MESSAGE_PENDING,
    SENSOR_MESSAGE_IN_PROGRESS,
    SENSOR_MESSAGE_COMPLETE,
    SENSOR_MESSAGE_TIMEDOUT,
    SENSOR_MESSAGE_ERROR,
} SENSOR_MESSAGE_STATUS_T;

typedef struct SENSOR_MSG SENSOR_MSG_T;

typedef struct SENSOR_MSG
{
    uint8_t address;
    SENSOR_MSG_TYPE_T type;
    SENSOR_REG_ID_T reg;
    uint8_t len;
    uint8_t *data;
    SENSOR_MSG_T *next;
    SENSOR_MESSAGE_STATUS_T status;
    uint8_t repeat_on_timeout;
} SENSOR_MSG_T;


/**
 * This function writes a complete message (including start and stop NULLs) into the buffer
 * @param m Details of the message to write
 * @param buf Pointer to output buffer
 * @param bufsize Size of output buffer
 * @return Number of bytes written, or 0 if buffer bad arguments or buffer was not big enough.
 */
size_t SENSOR_MESSAGE_FORMAT_MakeMessage(SENSOR_MSG_T *m, uint8_t *buf, size_t bufsize);

/**
 * This checks non-poll-request messages and placed the fields in the message structure.
 * Data will be copied to m->data if a non-null buffer pointer is provided.
 * The size of the data buffer must be supplied in m->len and will be overwritten with the actual size (if smaller).
 * @param m     Where to put the contents of the message.
 * @param buf   Buffer containing a complete message (including nulls).
 * @param len   How many bytes are in the message (including nulls).
 * @return      Whether a valid massage was found.
 */
bool SENSOR_MESSAGE_FORMAT_ValidateMessage(SENSOR_MSG_T *m, uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_MESSAGE_FORMAT_H
