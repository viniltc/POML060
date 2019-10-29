#ifdef __cplusplus
extern "C" {
#endif

#include "stim_gui_protocol_decode.h"
#include "debug.h"
#include "sensor_message_format.h"
#include "stim_engine.h"
#include "tetra_grip_reporter.h"

const STIM_ENGINE_REG_T reg_data[NUM_STIM_ENGINE_REGS]={
/* Firmware version */          {0,     1,	0,	0,          RD_REG	},
/* Hardware version */          {1,     1,	0,	0,          RD_REG	},
/* Echo */                      {2,     1,	0,	0xff,       RD_REG|WR_REG	},
/* Command error code */        {3,     1,	0,	0,          RD_REG	},
/* Vstim required */            {4,     2,	0,	15000,      RD_REG	},
/* Vstim measured A */          {6,     2,	0,	15000,      RD_REG	},
/* Vstim measured B */          {8,     2,	0,	15000,      RD_REG	},

/* GENERAL */
/* Status Mask */               {10,	2,	0,	0xffff,     RD_REG|WR_REG	},
/* Command/Status */            {12,	2,	0,	0xffff,     RD_REG|WR_REG	},
/* Trigger event */             {14,	1,	0,	255,        WR_REG	},
/* Phase number */              {15,	1,	0,	255,        RD_REG|WR_REG	},
/* Instantaneous pulse width */	{16,	2,	0,  450,        RD_REG	},
/* Set pulse width  */          {18,	2,	0,	450,        RD_REG|WR_REG	},
/* Pulse width modifier */      {20,	1,	0,	200,        RD_REG|WR_REG	},

/* SYNCHRONISATION */
/* Time */                      {24,	4,	0,	0xffffffff,	RD_REG|WR_REG	},
/* Microseconds */              {28,	4,	0,	0xffffffff,	RD_REG|WR_REG	},

/* CALIBRATION */
/* Measured current  */         {32,	2,	0,	15000,      RD_REG|WR_REG	},
/* Vstim offset */              {34,	1,	-128,	127,	RD_REG|WR_REG	},
/* Vstim gain */                {35,	1,	-128,	127,	RD_REG|WR_REG	},

/* OUTPUT LIMITS (apply to all phases) */
/* Max current */               {36,	2,	0,	12000,      RD_REG|WR_REG	},
/* Max pulse width */           {38,	2,	0,	450,        RD_REG|WR_REG	},
/* Min pulse width */           {40,	2,	0,	450,        RD_REG|WR_REG	},
/* Max frequency */             {42,	2,	1,	1000,       RD_REG|WR_REG	},
/* OPTIONS */
/* General options */           {44,	1,	0,	7,          RD_REG|WR_REG	},
/* EEPROM Use */                {45,	1,	0,	0xff,       RD_REG|WR_REG	},
/* Test train timeout */        {46,	2,	0,	6000,       RD_REG|WR_REG	},

/* PER-PHASE STIMULATION PARAMETERS */
/* Phase pointer */             {48,	1,	0,	255,        RD_REG|WR_REG	},
/* Ramp Parameter */            {50,	1,	0,	2,          RD_REG|WR_REG	},
/* Waveform */                  {51,	1,	0,	3,          RD_REG|WR_REG	},
/* Target pulse width */        {52,	2,	0,	450,        RD_REG|WR_REG	},
/* Target Current */            {54,	2,	0,	12000,      RD_REG|WR_REG	},
/* Target Frequency */          {56,	2,	0,	1000,       RD_REG|WR_REG	},
/* Duration */                  {58,	2,	1,	6000,       RD_REG|WR_REG	},
/* Ramp Rate */                 {60,	2,	1,	65000,      RD_REG|WR_REG	},
/* Salford Delay */             {62,	2,	1,	6000,       RD_REG|WR_REG	},
/* Goto phase on Duration / Target */	 {64,	1,	0,	255,	RD_REG|WR_REG	},

/* TRIGGER CONFIGURATION */
/* Trigger Setup */             {65,	1,	0,	255,        RD_REG|WR_REG	},

/* TRANSITION CONFIGURATION */
/* Destination on Trigger1 */	{66,    1,	0,	255,        RD_REG|WR_REG	},
/* Destination on Trigger2 */	{67,	1,	0,	255,        RD_REG|WR_REG	},
/* Destination on Trigger3 */	{68,	1,	0,	255,        RD_REG|WR_REG	},
};


const char *STIM_GUI_MSG_TYPE_T_strings[] = {"wire response", "write command", "read command", "read response"};
const char *STIM_GUI_TOPIC_T_strings[] = {"stimulator", "sub-activity", "channel", "sensor"};

uint32_t seconds=0;

const char *const MENU_ButtonNameStrings[NUM_MENU_BUTTONS]={
    "UP",
    "DOWN",
    "LEFT",
    "RIGHT",
    "SELECT",
    "TEST1",
    "TEST2",
    "TEST3",
    "TEST4",
    "TEST5",
    "SPARE1",
    "SPARE2",
    "FUNCTION",
    "HELP",
    "CUE",
    "PAUSE",
    "POWER",
};


/**
 * Get a signed 32-bit integer from four little-endian bytes
 * @param p Pointer to the bytes
 * @return The integer
 */
static inline int32_t get_int32(uint8_t *const p)
{
    return (int32_t)(p[3]<<24) | (p[2]<<16) | (p[1]<<8) | p[0];
}

/**
 * Get an unsigned 32-bit integer from four little-endian bytes
 * @param p Pointer to the bytes
 * @return The integer
 */
static inline uint32_t get_uint32(uint8_t *const p)
{
    return (uint32_t)(p[3]<<24) | (p[2]<<16) | (p[1]<<8) | p[0];
}

/**
 * Get a signed 16-bit integer from two little-endian bytes
 * @param p Pointer to the bytes
 * @return The integer
 */
static inline int16_t get_int16(uint8_t *const p)
{
    return ((int16_t)p[1]<<8) | p[0];
}

/**
 * Get an unsigned signed 16-bit integer from two little-endian bytes
 * @param p Pointer to the bytes
 * @return The integer
 */
static inline uint16_t get_uint16(uint8_t *const p)
{
    return ((uint16_t)p[1]<<8) | p[0];
}

static void print_sensor_status(SENSOR_STATUS_T status)
{
    DEBUG_Print("Node %s, ", (status & SENSOR_STATUS_NODE_FAILURE) ? "failed":"OK");
    DEBUG_Print("Button %s pressed, ", (status & SENSOR_STATUS_BUTTON_STATUS) ? "is":"not");
    DEBUG_Print("Config %s\n", (status & SENSOR_STATUS_CONFIGURED_OK) ? "OK":"bad");
}

static void print_sensor_extended_status(SENSOR_STATUS_T extended_status)
{
    DEBUG_Print("Calibration %s, ", (extended_status & SENSOR_EXTENDED_STATUS_CALIBRATION_INVALID) ? "invalid":"OK");
    DEBUG_Print("EEPROM %s, ", (extended_status & SENSOR_EXTENDED_STATUS_EEPROM_INVALID) ? "invalid":"OK");
    DEBUG_Print("Hardware %s, ", (extended_status & SENSOR_EXTENDED_STATUS_HARDWARE_FAILURE) ? "failed":"OK");
    DEBUG_Print("Voltage %s, ", (extended_status & SENSOR_EXTENDED_STATUS_LOW_VOLTAGE) ? "low":"OK");
    DEBUG_Print("Battery %s, ", (extended_status & SENSOR_EXTENDED_STATUS_BATTERY_WORN_OUT) ? "worn out":"OK");
}

static void print_power_cycle_count(uint16_t cycles)
{
    if(cycles)
        DEBUG_Print("Power cycle count: %d.\n", cycles);
    else
        DEBUG_Print("Power cycle count is unknown.\n");
}

static void print_hours_of_use(uint16_t hours)
{
    if(hours)
        DEBUG_Print("%d hours of use.\n", hours);
    else
        DEBUG_Print("Hours of use unknown.\n");
}

static void print_battery_voltage(uint8_t voltage_20mV)
{
    if(voltage_20mV==0)
        DEBUG_Print("No battery.\n");
    else
        DEBUG_Print("Battery voltage is %dmV.\n", 20*voltage_20mV);
}

static void print_battery_capacity(uint8_t capacity)
{
    if(capacity==255)
        DEBUG_Print("Battery capacity: unknown.\n");
    else
        DEBUG_Print("Battery capacity: %d%%.\n", capacity);
}

static void print_battery_minutes(uint8_t minutes)
{
    if(minutes==255)
        DEBUG_Print("Battery minutes remaining: unknown.\n");
    else
        DEBUG_Print("Battery minutes remaining: %d.\n", minutes);
}

static void print_activity_options(uint8_t options)
{
    DEBUG_Print("Activity options: %s,", (options & ACTIVITY_OPTIONS_NORMAL_STIMULATION) ? "normal stimulation":"dry run (no stimulation)");
    if(options & ACTIVITY_OPTIONS_AUTOMATIC_TRANSITIONS)
    {
        DEBUG_Print("%s transition mode.\n", (options & ACTIVITY_OPTIONS_USE_EXERCISE_TIMING) ? "exercise":"sensor");
    }
    else
        DEBUG_Print("manual transition mode.\n");
}

static void print_control_lock(uint8_t lock)
{
    switch(lock)
    {
        case 0:
            DEBUG_Print("User controls disabled.\n");
            break;
        case 255:
            DEBUG_Print("User controls never lock.\n");
            break;
        default:
            DEBUG_Print("User controls lock after %ds.\n", lock);
            break;
    }
}
static void print_control_options(uint8_t target)
{
    const char *const adjusts[4] = {"nothing",
                        "current (0-120mA)",
                        "pulse width (0-450us)",
                        "user intensity (0-100%%)"};
    DEBUG_Print("User controls adjust %s (channel %d).\n", adjusts[(target>>3)&3], target&7);
}

static const char *const sensor_model_id_strings[]={
    "FES-UPP Wired movement sensor",
    "FES-UPP Wireless movement sensor",
    "Reserved",
    "FES-UPP EMG",
    "FES-UPP instrumented object",
    "Force Sensitive Resistor",
    "Button",
    "OML Wireless footswitch",
    "Goniometer (single axis)",
    "Goniometer (dual axis)",
};

static const char *const sensor_command_strings[]={
    "RESET",
    "GOTO_BOOTLOADER",
    "POWER_OFF",
    "SLEEP",
    "AUTO_CALIBRATE",
    "SELF_TEST",
    "ASSIGN_ADDRESS",
    "SET_CONFIGURED_BIT",
    "AV_INDICATOR_OFF",
    "AV_INDICATOR_ON",
    "BUS_POWER_OFF",
    "BUS_POWER_ON",
    "COMMAND 12",
    "COMMAND 13",
    "COMMAND 14",
    "COMMAND 15",
    "FREEZE_SAMPLE",
};

static const char *const remote_button_strings[]=
{
    "Reserved",
    "Increase",
    "Decrease",
    "Move",
    "Start/Stop",
};

static uint8_t print_sensor_command(uint8_t *data, uint8_t max_len)
{
    if(!max_len)
        return 0;
    SENSOR_COMMAND_ID_T cmd=(SENSOR_COMMAND_ID_T)data[0];
    // First, print the command:
    if(cmd<(sizeof(sensor_command_strings)/sizeof(sensor_command_strings[0])))
    {
        DEBUG_Print("Command: %s.\n", sensor_command_strings[cmd]);
    }
    else
    {
        DEBUG_Print("Unknown command: %d.\n", cmd);
    }

    // Second, some commands have parameters:
    switch(cmd)
    {
        case SENSOR_CMD_ASSIGN_ADDRESS:
            if(max_len>=6)
            {
                DEBUG_Print("Assigned address %d to UUID=0x%.8X.\n", data[1], get_uint32(data+2));
                return 6;
            }
            break;
        default:
            break;
    }
    return 1;
}

/**
 * Extract the sensor data from the data payload
 */
static uint8_t print_sensor_streaming_data(uint8_t index, uint8_t *data, uint8_t max_len)
{
    char line[200];
    char *s=line;
    size_t len=0;
    
    double quaternion[4];
    int16_t euler213_degrees[3];
    int16_t euler123_degrees[3];
    double acceleration_g[3];
    double gyroscope_dps[3];

    if(max_len==2)
    {
        // assuming this a a grip/lift response - a robust decoder would check sensor model ID.
        DEBUG_Print("%s and %s\n", (data[0]?"Gripped":"Released"),  (data[1]?"up":"down"));
        return 2;
    }
    
    if(max_len<40) // This function needs there to be at least 40 bytes available for a full print 
    {
        DEBUG_Print("print_sensor_streaming_data expected 40 bytes, only %d available.\n", max_len);
        return 0;  // Declare an error by refusing to use any of them.   
    }
    
    uint8_t *p=data; // p is a pointer to the sensor data
    
    int i;
    for(i=0; i<4; i++)
    {
        quaternion[i]=((float)get_int32(p))/(1<<29);
        len+=snprintf(s+len, sizeof(line)-len, "\t%9.6f", quaternion[i]);
        p+=4;
    }

    for(i=0; i<3; i++)
    {
        euler213_degrees[i]=get_int16(p);
        len+=snprintf(s+len, sizeof(line)-len, "\t%d", euler213_degrees[i]);
        p+=2;
    }

    for(i=0; i<3; i++)
    {
        euler123_degrees[i]=get_int16(p);
        len+=snprintf(s+len, sizeof(line)-len, "\t%d", euler123_degrees[i]);
        p+=2;
    }

    for(i=0; i<3; i++)
    {
        acceleration_g[i]=(2.0*get_int16(p))/(1<<15);
        len+=snprintf(s+len, sizeof(line)-len, "\t%6.3f", acceleration_g[i]);
        p+=2;
    }

    for(i=0; i<3; i++)
    {
        gyroscope_dps[i]=(2000.0*get_int16(p))/(1<<15);
        len+=snprintf(s+len, sizeof(line)-len, "\t%6.1f", gyroscope_dps[i]);
        p+=2;
    }
#ifndef PIC32
//    if(logfile && logfile!=(FILE*)(-1))
//    {
//        fprintf(logfile, "%d.%3.3d\t%d\t%*s\n", seconds, milliseconds, index, len, line);
//    }
//    else
#endif // PIC32
    {
        DEBUG_Print("%*s\n", len, line);
    }
    return p-data;
}


/**
 * Decode and print a sensor data block
 * @param b Pointer to the block
 */
static void STIM_GUI_PROTOCOL_DECODE_ProcessSensorShortBlock(STIM_GUI_MESSAGE_S_BLOCK_T *b)
{
    uint8_t len=b->data_length;
    uint8_t addr=b->reg_address;
    uint8_t *data=b->data;
    
    while(len)
    {
        uint8_t bytes_used=0;
        switch(addr)
        {
            case SENSOR_REG_MODEL_ID:
                {
                    uint8_t id=*data;
                    DEBUG_Print("Model is %d (%s).\n", id, (id<sizeof(sensor_model_id_strings)/sizeof(sensor_model_id_strings[0])) ? sensor_model_id_strings[id]:"unknown type");
                    bytes_used=1;
                }
                break;
            case SENSOR_REG_HARDWARE_VERSION:
                DEBUG_Print("Hardware version is %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_FIRMWARE_VERSION:
                DEBUG_Print("Firmware version is %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_LAST_HARDWARE_ERROR_CODE:
                DEBUG_Print("Last hardware error code is %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_LAST_COMMAND_ERROR_CODE:
                DEBUG_Print("Last command error code is %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_STATUS:
                print_sensor_status((SENSOR_STATUS_T)*data);
                bytes_used=1;
                break;
            case SENSOR_REG_EXTENDED_COMMAND_STATUS:
                print_sensor_extended_status((SENSOR_EXTENDED_STATUS_T)*data);
                bytes_used=1;
                break;
            case SENSOR_REG_UNIQUE_UNIT_ID:
                if(len>=4)
                {
                    DEBUG_Print("UUID=0x%.8X\n", get_uint32(data));
                    bytes_used=4;
                }
                break;
            case SENSOR_REG_TIME_SECONDS:
                if(len>=4)
                {
                    DEBUG_Print("Seconds: %d\n", seconds=get_uint32(data));
                    bytes_used=4;
                }
                break;
            case SENSOR_REG_TIME_MICROSECONDS:
                if(len>=4)
                {
                    DEBUG_Print("Micro seconds: %d\n", get_uint32(data));
                    bytes_used=4;
                }
                break;
            case SENSOR_REG_POWER_CYCLE_COUNT:
                if(len>=2)
                {
                    print_power_cycle_count(get_uint16(data));
                    bytes_used=2;
                }
                break;
            case SENSOR_REG_HOURS_OF_USE:
                if(len>=2)
                {
                    print_hours_of_use(get_uint16(data));
                    bytes_used=2;
                }
                break;
            case SENSOR_REG_SUPPLY_VOLTAGE:
                DEBUG_Print("Supply voltage %dmV.\n", 20 * *data);
                bytes_used=1;
                break;
            case SENSOR_REG_BATTERY_VOLTAGE:
                print_battery_voltage(*data);
                bytes_used=1;
                break;
            case SENSOR_REG_BATTERY_CAPACITY_REMAINING:
                print_battery_capacity(*data);
                bytes_used=1;
                break;
            case SENSOR_REG_BATTERY_MINUTES_REMAINING:
                print_battery_minutes(*data);
                bytes_used=1;
                break;
            case SENSOR_REG_GENERAL_OPTIONS:
                DEBUG_Print("General options: 0x%2.2x.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_POWER_MODE:
                DEBUG_Print("Power mode: %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_TEMPERATURE:
                DEBUG_Print("Temperature: %dC.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_COMMAND:
                bytes_used=print_sensor_command(data, len);
                break;
            case SENSOR_REG_SENSOR_OPTIONS:
                DEBUG_Print("Sensor options: 0x%2.2x.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_TRIGGER_OPTIONS:
                DEBUG_Print("Trigger options: 0x%2.2x.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_STREAMING_OPTIONS:
                DEBUG_Print("Streaming options: 0x%2.2x.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_UPDATE_RATE:
                DEBUG_Print("Update rate: %dHz.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_STREAMING_ADDRESS:
                DEBUG_Print("Streaming address: %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_STREAMING_LENGTH:
                DEBUG_Print("Streaming length: %d.\n", *data);
                bytes_used=1;
                break;
            case SENSOR_REG_STREAMING_DATA:
                bytes_used=print_sensor_streaming_data(b->index, data, len);
                break;
            case SENSOR_REG_BUILD_VERSION_INFO:
                DEBUG_Print("Sensor build version information:\n%.*s\n", len, data);
                bytes_used=len;
                break;
        }
        
        if(bytes_used)
        {
            len-=bytes_used;
            addr+=bytes_used;
            data+=bytes_used;
        }
        else
        {
            DEBUG_Print("(%d undecoded sensor bytes)\n", len);
            return;
        }
    }
}



/**
 * Decode and print a stimulator short data block
 * @param b Pointer to the block
 */
static void STIM_GUI_PROTOCOL_DECODE_ProcessStimShortBlock(STIM_GUI_MESSAGE_S_BLOCK_T *b)
{
    uint8_t len=b->data_length;
    uint8_t addr=b->reg_address;
    uint8_t *data=b->data;

    while(len)
    {
        uint8_t bytes_used=0;
        switch(addr)
        {
            case STIM_REG_ECHO:
                DEBUG_Print("Echo data: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_STIMULATOR_TYPE:
                DEBUG_Print("Stimulator type: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_HARDWARE_VERSION:
                DEBUG_Print("Hardware version is %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_FIRMWARE_VERSION:
                DEBUG_Print("Firmware version is %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_LAST_HARDWARE_ERROR_CODE:
                DEBUG_Print("Last hardware error code is %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_LAST_COMMAND_ERROR_CODE:
                DEBUG_Print("Last command error code is %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_STATUS:
                DEBUG_Print("Status code is 0x%2.2x: Config=%s.\n", *data, (*data & STIMULATOR_STATUS_CONFIG_ERROR) ? "Error":"OK");
                bytes_used=1;
                break;
            case STIM_REG_EXTENDED_STATUS:
                DEBUG_Print("Extended status code is 0x%2.2x.\n", *data);
                bytes_used=1;
               break;
            case STIM_REG_UNIQUE_UNIT_ID:
                if(len>=4)
                {
                    DEBUG_Print("UUID=0x%.8X\n", get_uint32(data));
                    bytes_used=4;
                }
                break;
            case STIM_REG_TIME_SECONDS:
                if(len>=4)
                {
                    DEBUG_Print("Time: seconds: %d\n", seconds=get_uint32(data));
                    bytes_used=4;
                }
                break;
            case STIM_REG_TIME_MICROSECONDS:
                if(len>=4)
                {
                    DEBUG_Print("Time: microseconds: %d\n", get_uint32(data));
                    bytes_used=4;
                }
                break;
            case STIM_REG_POWER_CYCLE_COUNT:
                if(len>=2)
                {
                    print_power_cycle_count(get_uint16(data));
                    bytes_used=2;
                }
                break;
            case STIM_REG_HOURS_OF_USE:
                if(len>=2)
                {
                    print_hours_of_use(get_uint16(data));
                    bytes_used=2;
                }
                break;
            case STIM_REG_SUPPLY_VOLTAGE:
                DEBUG_Print("Supply voltage %dmV.\n", 20 * *data);
                bytes_used=1;
                break;
            case STIM_REG_BATTERY_VOLTAGE:
                print_battery_voltage(*data);
                bytes_used=1;
                break;
            case STIM_REG_BATTERY_CAPACITY_REMAINING:
                print_battery_capacity(*data);
                tetra_grip_reporter(TOPIC_STIMULATOR, STIM_REG_BATTERY_CAPACITY_REMAINING, *data);
                bytes_used=1;
                break;
            case STIM_REG_BATTERY_MINUTES_REMAINING:
                print_battery_minutes(*data);
                bytes_used=1;
                break;
            case STIM_REG_GENERAL_OPTIONS:
                DEBUG_Print("General options: 0x%2.2x.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_POWER_MODE:
            {
                STIM_GUI_POWER_MODE_T mode=*data;
                const char *mode_text="Unknown";
                switch(mode)
                {
                    case STIM_POWER_MODE_NORMAL:
                        mode_text="Normal";
                        break;
                    case STIM_POWER_MODE_STANDBY:
                        mode_text="Standby";
                        break;
                    case STIM_POWER_MODE_OFF:
                        mode_text="Off";
                        break;
                }
                DEBUG_Print("Power mode: %d:%s\n", *data, mode_text);
                bytes_used=1;
            }
                break;
            case STIM_REG_TEMPERATURE:
                DEBUG_Print("Temperature: %dC.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_COMMAND:
                DEBUG_Print("Command: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_NUM_STIMULATORS:
                DEBUG_Print("Number of stimulators: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_NUM_CHANNELS:
                DEBUG_Print("Number of stimulation outputs: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_NUM_FOOTSWITCH_SOCKETS:
                DEBUG_Print("Number of foot switch sockets: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_NUM_SMART_SENSORS:
                DEBUG_Print("Number of attached sensors: %d\n", *b->data);
                bytes_used=1;
                break;
            case STIM_REG_SD_CARD_ERROR_CODE:
                DEBUG_Print("SD card error code: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_STIM_RAIL_VOLTAGE:
                DEBUG_Print("Stim rail voltage %dmV.\n", 20 * *data);
                bytes_used=1;
                break;
            case STIM_REG_SENSOR_CONFIGURATION_ID:
                if(len>=2)
                {
                    DEBUG_Print("Sensor configuration ID: %d\n", get_uint16(b->data));
                    bytes_used=2;
                }
                break;
            case STIM_REG_USER_SELECTED:
                DEBUG_Print("User selected: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_ACTIVITY_SELECTED:
                DEBUG_Print("Activity selected: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_ACTIVITY_OPTIONS:
                print_activity_options(*data);
                bytes_used=1;
                break;
            case STIM_REG_ACTIVITY_STATUS:
                DEBUG_Print("Activity: %s.\n", *data ? "running":"stopped");
                bytes_used=1;
                break;
            case STIM_REG_ACTIVITY_TIME_RUNNING_MS:
                if(len>=4)
                {
                    DEBUG_Print("Activity time running: %dms\n", get_uint32(data));
                    bytes_used=4;
                }
                break;
            case STIM_REG_ACTIVITY_MINUTES_REMAINING:
                DEBUG_Print("Activity minutes remaining: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_USER_CONTROL_LOCK:
                print_control_lock(*data);
                bytes_used=1;
                break;
            case STIM_REG_USER_CONTROL_OPTIONS:
                print_control_options(*data);
                bytes_used=1;
                break;
            case STIM_REG_USER_CONTROL_KNOB_INCREMENT:
                DEBUG_Print("User intensity control increment: %d.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_USER_CONTROL_CH1:
            case STIM_REG_USER_CONTROL_CH2:
            case STIM_REG_USER_CONTROL_CH3:
            case STIM_REG_USER_CONTROL_CH4:
            case STIM_REG_USER_CONTROL_CH5:
                DEBUG_Print("User intensity control for CH%d=%d%%.\n", addr-STIM_REG_USER_CONTROL_CH1+1, *data);
                bytes_used=1;
                break;
            case STIM_REG_USER_REMOTE_BUTTON_PRESS:
                DEBUG_Print("Remote Button press: %s.\n", remote_button_strings[*data]);
                bytes_used=1;
                break;
            case STIM_REG_CHANNEL_SELECT_LEDS:
                DEBUG_Print("Channel select LEDs=0x%2.2X.\n", *data);
                bytes_used=1;
                break;
            case STIM_REG_STIMULATOR_BUTTON_PRESS:
                if(*data<NUM_MENU_BUTTONS*2)
                    DEBUG_Print("Stim button %s %s.\n", MENU_ButtonNameStrings[(*data)>>1], (1 & *data)?"Pressed":"Released");
                else
                    DEBUG_Print("Unknown button %s.\n", (1&*data)?"Pressed":"Released");
                bytes_used=1;
                break;
            case STIM_REG_BEEP_REPETITON_PERIOD_100MS:
                DEBUG_Print("Beep repeats every %d.%ds.\n", *data/10, *data%10);
                bytes_used=1;
                break;
            case STIM_REG_BEEP_ON_TIME_10MS:
                DEBUG_Print("Beep on time: %dms.\n", *data*10);
                bytes_used=1;
                break;
            case STIM_REG_BEEP_FREQUENCY_10HZ:
                DEBUG_Print("Beep frequency: %dHz.\n", *data*10);
                bytes_used=1;
                break;
            case STIM_REG_BEEP_COUNTS:
                DEBUG_Print("Beep counts: %d.\n", *data);
                bytes_used=1;
                break;
        }

        if(bytes_used)
        {
            len-=bytes_used;
            addr+=bytes_used;
            data+=bytes_used;
        }
        else
        {
            DEBUG_Print("(%d undecoded stimulator bytes)\n", len);
            return;
        }
    }
}


const char *const transition_condition_code_strings[]={
"Jump", // 248
"Stop", // 249
"Start", // 250
"Activity timeout", // 251
"Exercise timeout", // 252
"Move phase button", // 253
"Phase timeout", //254
"Sensor error" // 255
};
#define NUM_CODE_STRINGS (sizeof(transition_condition_code_strings)/sizeof(transition_condition_code_strings[0]))

/**
 * Decode and print a sub-activity short data block
 * @param b Pointer to the block
 */
static void STIM_GUI_PROTOCOL_DECODE_ProcessSubActivityShortBlock(STIM_GUI_MESSAGE_S_BLOCK_T *b)
{
    uint8_t len=b->data_length;
    uint8_t addr=b->reg_address;
    uint8_t *data=b->data;
    while(len)
    {
        uint8_t bytes_used=0;
        if(addr<STIM_SUB_ACT_REG_ROLE_UUID_BASE)
        {
            switch(addr)
            {
                case STIM_SUB_ACT_REG_CURRENT_PHASE:
                    DEBUG_Print("Current phase: %d\n", *data);
                    bytes_used=1;
                    break;
                case STIM_SUB_ACT_REG_CONDITION_SATISFIED:
                {
                    uint8_t code=*data;
                    bytes_used=1;
                    DEBUG_Print("Condition satisfied: %d %s\n", code, 
                            code<=255-NUM_CODE_STRINGS ? "" : transition_condition_code_strings[code-256+NUM_CODE_STRINGS]);
                    break;
                }
                case STIM_SUB_ACT_REG_CURRENT_ITERATION:
                    if(len>=2)
                    {
                        DEBUG_Print("Current iteration: %d\n", get_uint16(data));
                        bytes_used=2;
                    }
                    break;
                case STIM_SUB_ACT_REG_DURATION_OF_CURRENT_PHASE_MS:
                    if(len>=4)
                    {
                        DEBUG_Print("Duration of current phase: %dms\n", seconds=get_uint32(data));
                        bytes_used=4;
                    }
                    break;
                default:
                    DEBUG_Print("Can't handle byte 0x%2.2x at address %d.\n", *data, addr);
                    bytes_used=1;
            }
        }
        else if(addr<STIM_SUB_ACT_REG_ROLE_CHANGE_BASE)
        {
            //STIM_SUB_ACT_REG_ROLE_UUID_BASE
            if(len>=4)
            {
                uint8_t role=(addr-STIM_SUB_ACT_REG_ROLE_UUID_BASE)/sizeof(uint32_t);
                uint32_t uuid=get_uint32(data);
                bytes_used=4;
                if(uuid)
                    DEBUG_Print("Sensor role %d is assigned to sensor with UUID %8.8x\n", role, uuid);
                else
                    DEBUG_Print("Sensor role %d is unassigned.\n", role);
            }
        }
        else
        {
            // STIM_SUB_ACT_REG_ROLE_CHANGE_BASE
            if(len>=4)
            {
                uint8_t role=(addr-STIM_SUB_ACT_REG_ROLE_CHANGE_BASE)/sizeof(int32_t);
                int32_t change=get_int32(data);
                bytes_used=4;
                DEBUG_Print("Sensor with role %d changed by %d in this phase\n", role, change);
            }
        }
        
        if(bytes_used)
        {
            len-=bytes_used;
            addr+=bytes_used;
            data+=bytes_used;
        }
        else
        {
            DEBUG_Print("(%d undecoded subactivity bytes)\n", len);
            return;
        }
    }
}

static void print_stim_engine_status(const STIM_ENGINE_STATUS_BITS_T status)
{
    DEBUG_Print("Engine paused: %s\n", (status & STIM_ENGINE_StatusPaused) ? "yes":"no");
    DEBUG_Print("Envelope active: %s\n", (status & STIM_ENGINE_StatusStimulation_envelope_active) ? "yes":"no");
    DEBUG_Print("Pulse active: %s\n", (status & STIM_ENGINE_StatusStimulation_pulse_active) ? "yes":"no");
    DEBUG_Print("Timed out: %s\n", (status & STIM_ENGINE_Status_Stimulation_timed_out) ? "yes":"no");
    DEBUG_Print("Command error: %s\n", (status & STIM_ENGINE_Status_Command_error_since_last_read) ? "yes":"no");
    DEBUG_Print("Vstim failure: %s\n", (status & STIM_ENGINE_Status_Vstim_failure) ? "yes":"no");
    DEBUG_Print("Fault active: %s\n", (status & STIM_ENGINE_Status_Fault_input_active) ? "yes":"no");
    DEBUG_Print("Reached target: %s\n", (status & STIM_ENGINE_Status_Reached_target_PW) ? "yes":"no");
    DEBUG_Print("Calibrated: %s\n", (status & STIM_ENGINE_Status_Calibrated) ? "yes":"no");
    DEBUG_Print("Self test: %s\n", (status & STIM_ENGINE_Status_Self_test_passed_OK) ? "OK":"FAILED");
    DEBUG_Print("Config valid: %s\n", (status & STIM_ENGINE_Status_Configuration_validation_OK) ? "yes":"no");
}

static void print_stim_engine_general_options(const STIM_ENGINE_GENERAL_OPTIONS_ENUM options)
{
    DEBUG_Print("%s mode.\n", (options & STIM_ENGINE_GENERAL_OPTIONS_SALFORD_MODE) ? "Salford":"Salisbury");
    DEBUG_Print("Timeout from %s.\n", (options & STIM_ENGINE_GENERAL_OPTIONS_TIMEOUT_FROM_TRIGGER) ? "trigger":"target level");
    switch(options & STIM_ENGINE_GENERAL_OPTIONS_RESERVED)
    {
        case STIM_ENGINE_GENERAL_OPTIONS_NO_AUTO_PHASE_CHANGE:
            DEBUG_Print("No automatic phase change.\n");
            break;
        case STIM_ENGINE_GENERAL_OPTIONS_EXERCISE_MODE:
            DEBUG_Print("Exercise mode\n");
            break;
        case STIM_ENGINE_GENERAL_OPTIONS_PROCESS_TRIGGERS:
            DEBUG_Print("Triggered mode.\n");
            break;
        default:
            DEBUG_Print("Unknown mode.\n");
    }
    DEBUG_Print("Sounder %s.\n", (options & STIM_ENGINE_GENERAL_OPTIONS_SOUNDER_ENABLED) ? "enabled":"disabled");
}

static void print_stim_engine_ramp_parameter(const STIM_ENGINE_RAMP_PARAMETER_ENUM ramp)
{
    switch(ramp)
    {
        case STIM_ENGINE_RAMP_PARAMETER_PULSE_WIDTH:
            DEBUG_Print("Pulse width ramp.\n");
            break;
        case STIM_ENGINE_RAMP_PARAMETER_CURRENT:
            DEBUG_Print("Current ramp\n");
            break;
        case STIM_ENGINE_RAMP_PARAMETER_FREQUENCY:
            DEBUG_Print("Frequency ramp.\n");
            break;
        default:
            DEBUG_Print("Unknown ramp.\n");
    }
}

static void print_stim_engine_waveform(const STIM_ENGINE_WAVEFORM_ENUM waveform)
{
    DEBUG_Print("Waveform: %s, %s, %s.\n",
            (waveform & STIM_ENGINE_WAVEFORM_SYM) ? "SYM":"ASYM",
            (waveform & STIM_ENGINE_WAVEFORM_NEGATIVE) ? "negative":"positive",
            (waveform & STIM_ENGINE_WAVEFORM_TRUE_BIPHASIC) ? "OML standard":"true biphasic");
}

/**
 * Decode and print a channel data block
 * @param b Pointer to the block
 */
static void STIM_GUI_PROTOCOL_DECODE_ProcessChannelShortBlock(STIM_GUI_MESSAGE_S_BLOCK_T *b)
{
    uint8_t len=b->data_length;
    uint8_t addr=b->reg_address;
    uint8_t *data=b->data;
    
    while(len)
    {
        uint8_t bytes_used=0;
        STIM_ENGINE_REG_ID reg;
        if(addr==reg_data[STIM_ENGINE_REG_INSTANTANEOUS_PULSE_WIDTH].addr)
            reg=STIM_ENGINE_REG_INSTANTANEOUS_PULSE_WIDTH; // most common option
        else
            for(reg=0; reg<NUM_STIM_ENGINE_REGS; reg++) // longer, inefficient search.
                if(reg_data[reg].addr==addr)
                    break;
        if(reg==NUM_STIM_ENGINE_REGS)
        {
            DEBUG_Print("Could not find stim engine register with address %d (%d bytes remaining).\n", addr, len);
            return;
        }
        if(len<reg_data[reg].length)
        {
            DEBUG_Print("Stim engine register address %d requires %d bytes but only %d bytes remaining).\n", addr, reg_data[reg].length, len);
            return;
        }
        else
            bytes_used=reg_data[reg].length;
        switch(reg)
        {
            case STIM_ENGINE_REG_FIRMWARE_VERSION:
                DEBUG_Print("Firmware version is %d.\n", *data);
                break;
            case STIM_ENGINE_REG_HARDWARE_VERSION:
                DEBUG_Print("Hardware version is %d.\n", *data);
                break;
            case STIM_ENGINE_REG_ECHO:
                DEBUG_Print("Echo is %d.\n", *data);
                break;
            case STIM_ENGINE_REG_COMMAND_ERROR_CODE:
                DEBUG_Print("Command error code is %d.\n", *data);
                break;
            case STIM_ENGINE_REG_VSTIM_REQUIRED:
                DEBUG_Print("Vstim required: %dmV\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_VSTIM_MEASURED_A:
                DEBUG_Print("Vstim measured A: %dmV\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_VSTIM_MEASURED_B:
                DEBUG_Print("Vstim measured B: %dmV\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_STATUS_MASK:
                DEBUG_Print("Status IRQ mask:\n");
                print_stim_engine_status((STIM_ENGINE_STATUS_BITS_T)*data);
                break;
            case STIM_ENGINE_REG_COMMAND_STATUS:
                DEBUG_Print("Status:\n");
                print_stim_engine_status((STIM_ENGINE_STATUS_BITS_T)*data);
                break;
            case STIM_ENGINE_REG_TRIGGER_EVENT:
                DEBUG_Print("Trigger event %d.\n", *data);
                break;
            case STIM_ENGINE_REG_PHASE_NUMBER:
                DEBUG_Print("Phase number %d.\n", *data);
                break;
            case STIM_ENGINE_REG_INSTANTANEOUS_PULSE_WIDTH:
                DEBUG_Print("Inst.PW: %dus\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_SET_PULSE_WIDTH:
                DEBUG_Print("Set PW: %dus\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_PULSE_WIDTH_MODIFIER:
                DEBUG_Print("PW modifier: %d\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_TIME:
                DEBUG_Print("Seconds: %d\n", get_uint32(data));
                break;
            case STIM_ENGINE_REG_MICROSECONDS:
                DEBUG_Print("Microseconds: %d\n", get_uint32(data));
                break;
            case STIM_ENGINE_REG_MEASURED_CURRENT:
                DEBUG_Print("Measured current (unknown format)\n");
                break;
            case STIM_ENGINE_REG_VSTIM_OFFSET:
                DEBUG_Print("Vstim offset (unknown format)\n");
                break;
            case STIM_ENGINE_REG_VSTIM_GAIN:
                DEBUG_Print("Vstim gain (unknown format)\n");
                break;
            case STIM_ENGINE_REG_MAX_CURRENT:
                DEBUG_Print("Max current: %.1fmA\n", get_uint16(data)/100.0);
                break;
            case STIM_ENGINE_REG_MAX_PULSE_WIDTH:
                DEBUG_Print("Max PW: %dus\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_MIN_PULSE_WIDTH:
                DEBUG_Print("Min PW: %dus\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_MAX_FREQUENCY:
                DEBUG_Print("Max frequency %dHz.\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_GENERAL_OPTIONS:
                print_stim_engine_general_options((STIM_ENGINE_GENERAL_OPTIONS_ENUM)*data);
                break;
            case STIM_ENGINE_REG_EEPROM_USE:
                DEBUG_Print("EEPROM use %d.\n", *data);
                break;
            case STIM_ENGINE_REG_TEST_TRAIN_TIMEOUT:
                DEBUG_Print("Test train timeout %dms.\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_PHASE_POINTER:
                DEBUG_Print("Phase pointer %d.\n", *data);
                break;
            case STIM_ENGINE_REG_RAMP_PARAMETER:
                print_stim_engine_ramp_parameter((STIM_ENGINE_RAMP_PARAMETER_ENUM)*data);
                break;
            case STIM_ENGINE_REG_WAVEFORM:
                print_stim_engine_waveform((STIM_ENGINE_WAVEFORM_ENUM)*data);
                break;
            case STIM_ENGINE_REG_TARGET_PULSE_WIDTH:
                DEBUG_Print("Target PW: %dus\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_TARGET_CURRENT:
                DEBUG_Print("Target current: %.1fmA\n", get_uint16(data)/100.0);
                break;
            case STIM_ENGINE_REG_TARGET_FREQUENCY:
                DEBUG_Print("Target frequency %dHz.\n", *data);
                break;
            case STIM_ENGINE_REG_DURATION:
                DEBUG_Print("Duration: %dms\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_RAMP_RATE:
                DEBUG_Print("Ramp rate: %dns/pulse\n", get_uint16(data)*10);
                break;
            case STIM_ENGINE_REG_SALFORD_DELAY:
                DEBUG_Print("Salford delay: %dms\n", get_uint16(data));
                break;
            case STIM_ENGINE_REG_GOTO_PHASE_ON_DURATION_TARGET:
                DEBUG_Print("Goto phase on duration/target %d.\n", *data);
                break;
            case STIM_ENGINE_REG_TRIGGER_SETUP:
                DEBUG_Print("Goto phase on duration/target %d.\n", *data);
                break;
            case STIM_ENGINE_REG_DESTINATION_ON_TRIGGER1:
                DEBUG_Print("Destination on trigger 1: phase %d.\n", *data);
                break;
            case STIM_ENGINE_REG_DESTINATION_ON_TRIGGER2:
                DEBUG_Print("Destination on trigger 2: phase %d.\n", *data);
                break;
            case STIM_ENGINE_REG_DESTINATION_ON_TRIGGER3:
                DEBUG_Print("Destination on trigger 3: phase %d.\n", *data);
                break;
            case NUM_STIM_ENGINE_REGS:
                DEBUG_Print("Error in STIM_GUI_PROTOCOL_DECODE_ProcessChannelData\n");
                break;
        }
        
        if(bytes_used)
        {
            len-=bytes_used;
            addr+=bytes_used;
            data+=bytes_used;
        }
        else
        {
            DEBUG_Print("(%d undecoded sensor bytes)\n", len);
            return;
        }
    }
}


/**
 * Decode and print a short data block
 * @param b Pointer to the block
 */
void STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock(STIM_GUI_MESSAGE_S_BLOCK_T *b)
{
    if(!b)
    {
        DEBUG_Print("Null pointer to STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock.\n");
        return;
    }

    if(b->topic >= sizeof(STIM_GUI_TOPIC_T_strings)/sizeof(STIM_GUI_TOPIC_T_strings[0])
            || b->msg_type >= sizeof(STIM_GUI_MSG_TYPE_T_strings)/sizeof(STIM_GUI_MSG_TYPE_T_strings[0])
            || b->reg_address>255
            || b->index>15)
    {
        DEBUG_Print("Bad argument to STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock.\n");
        return;
    }
    
    if(b->msg_type==READ_COMMAND)
    {
        DEBUG_Print("Read request for %s index %d, register %d, %d bytes\n", STIM_GUI_TOPIC_T_strings[b->topic], b->index, b->reg_address, b->data_length);
        return;
    }
    else
    {   // All others must supply data.
        if(!b->data_length || !b->data)
        {
            DEBUG_Print("Decoding %s block: Error: data missing!\n", STIM_GUI_TOPIC_T_strings[b->topic]);
            return;
        }
    }

//    if(debugOptions.print_message_headers)
        DEBUG_Print("%s index %d %s: ", STIM_GUI_TOPIC_T_strings[b->topic], b->index, STIM_GUI_MSG_TYPE_T_strings[b->msg_type]);

    switch(b->topic)
    {
        case TOPIC_STIMULATOR:
            if(b->index)
            {
                DEBUG_Print("STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock: Error: this is not a stimulator short block.\n");
                break;
            }
            STIM_GUI_PROTOCOL_DECODE_ProcessStimShortBlock(b);
            break;
        case TOPIC_SUB_ACTIVITY:
            STIM_GUI_PROTOCOL_DECODE_ProcessSubActivityShortBlock(b);
            break;
        case TOPIC_CHANNEL:
            STIM_GUI_PROTOCOL_DECODE_ProcessChannelShortBlock(b);
            break;
        case TOPIC_SENSOR:
            STIM_GUI_PROTOCOL_DECODE_ProcessSensorShortBlock(b);
            break;
    }
}

#ifdef __cplusplus
}
#endif
