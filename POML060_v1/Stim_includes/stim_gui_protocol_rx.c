#ifdef __cplusplus
extern "C" {
#endif

#include "stim_gui_protocol_rx.h"
#include "stim_gui_protocol_decode.h"
#include "crc32.h"
#include "debug.h"
#include "uart.h"
#include "clock.h"
#include "hardware.h"
#include <string.h>

DEBUG_OPTIONS_T debugOptions = {false, false, true, false, false, 0};

uint32_t STIM_GUI_PROTOCOL_RX_PrintReceivedHexIfLessThanNBytes=0;
bool STIM_GUI_PROTOCOL_RX_DISCARD_SENSOR_DATA_PACKETS=false;
bool STIM_GUI_PROTOCOL_RX_DISCARD_TIME_PACKETS=false;
bool STIM_GUI_PROTOCOL_RX_DISCARD_PULSE_WIDTH_PACKETS=false;

#define IS_STATUS_READ(msg) ((msg)[3]==0xC0 && (msg)[4]==0x06 && (msg)[5]==0x01)

#ifdef PIC32
    #include "stim_gui_protocol_rx_pic_actions.h" // for read and write to stimulator, sensor and sub-activity registers
    #include "eeprom.h"
    #include "eeprom_map.h"
    #define INDICATE_GUI_COMMS_ERROR do{PLIB_PORTS_PinToggle(ISTIM_SYNC);}while(0)
#else
    #define INDICATE_GUI_COMMS_ERROR do{}while(0)
#endif // PIC32


#define MAX_MESSAGE_LEN_FROM_STIM (11000)
#define CRC32_VALID (0x2144df1c)

char *stim_gui_msg_type_strings[]={"WRITE_RESPONSE", "WRITE_COMMAND", "READ_RESPONSE", "READ_COMMAND"};
char *stim_gui_msg_topic_strings[]={"stimulator", "sub-activity", "channel", "sensor"};

uint16_t milliseconds=0;

/**
 * Initialise the receive part of the stim-gui protocol
 */
void STIM_GUI_PROTOCOL_RX_Initialise(void)
{
}

/**
 * Populate a short block structure with values from the received block
 * @param pblock Pointer to the structure to fill.
 * @param p Pointer to start of the received block
 * @return Total number of bytes this block will need (header + data)
 */
static size_t get_short_block_fields(STIM_GUI_MESSAGE_S_BLOCK_T *pblock, uint8_t *p)
{
    size_t bytes_needed;
    
    bool is_status_read=IS_STATUS_READ(p-3);
    pblock->topic=(*p>>4)&0x3;
    pblock->msg_type=*p>>6;
    pblock->index=*p&0xf;
    p++;
    pblock->reg_address=*p++;
    pblock->data_length=*p++;
    pblock->next=NULL;

    if(!is_status_read && debugOptions.print_message_headers)
        DEBUG_Print("Short block: %s for %s %d, register %d, length %d:", stim_gui_msg_type_strings[pblock->msg_type], stim_gui_msg_topic_strings[pblock->topic], pblock->index, pblock->reg_address, pblock->data_length);

    if(pblock->msg_type==READ_COMMAND)
    {
        pblock->data = NULL;
        bytes_needed=STIM_GUI_PROTOCOL_S_BLOCK_HEADER_SIZE;
    }
    else
    {
        pblock->data = pblock->data_length ? p : NULL;
        bytes_needed=STIM_GUI_PROTOCOL_S_BLOCK_HEADER_SIZE+pblock->data_length;
    }
    
    if(!is_status_read && debugOptions.print_message_headers)
    {
        if(pblock->data && pblock->data_length<=4)
        {
            uint32_t val=0;
            int i;
            for(i=0; i<pblock->data_length; i++)
            {
                DEBUG_Print(" 0x%2.2x", pblock->data[i]);
                val+=pblock->data[i]<<(8*i);
            }
            DEBUG_Print(" u:%u s:%d\n", val, (int32_t)val);
        }
        else
            DEBUG_Print("\n");
    }
    return bytes_needed;
}

/**
 * Populate a long block structure with values from the received block
 * @param pblock Pointer to the structure to fill.
 * @param p Pointer to start of the received block
 * @return Total number of bytes this block will need (header + data)
 */
static size_t get_long_block_fields(STIM_GUI_MESSAGE_L_BLOCK_T *pblock, uint8_t *p)
{
    size_t bytes_needed;
    
    pblock->msg_type=*p++>>6;
    pblock->reg_address=*p++;
    pblock->data_length=*p++;
    pblock->data_length|=(*p++)<<8;
    pblock->data_length|=(*p++)<<16;
    pblock->data_length|=(*p++)<<24;
    
    if(pblock->msg_type==READ_COMMAND)
    {
        pblock->data = NULL;
        bytes_needed=STIM_GUI_PROTOCOL_L_BLOCK_HEADER_SIZE;
    }
    else
    {
        pblock->data = pblock->data_length ? p : NULL;
        bytes_needed=STIM_GUI_PROTOCOL_L_BLOCK_HEADER_SIZE+pblock->data_length;
    }
    return bytes_needed;
}

/**
 * Process a short block 
 * @param block_no Block number (supplied for debug messages)
 * @param start Pointer to the beginning of this block
 * @param limit Pointer to the first byte of checksum
 * @return number of bytes consumed in this block.
 */
static size_t process_stim_gui_short_block(const size_t block_no, uint8_t *const start, uint8_t *const limit)
{
    if(limit<start+3)
    {
        DEBUG_Print("Block %d impossibly short.\n", block_no);
        INDICATE_GUI_COMMS_ERROR;
        return limit-start; // consume all bytes as this message is broken.
    }
    
    STIM_GUI_MESSAGE_S_BLOCK_T block;
    size_t bytes_used;

    bytes_used=get_short_block_fields(&block, start);
    if(limit-start<bytes_used)
    {
        DEBUG_Print("Block %d claims to have %d bytes but the message doesn't have this many left.\n", block_no, block.data_length);
        INDICATE_GUI_COMMS_ERROR;
        return limit-start; // say we used all available bytes.
    }

    if(block.topic==TOPIC_STIMULATOR && *start & 0xf)
    {
        DEBUG_Print("process_stim_gui_short_block: Error: this is not a stimulator short block.\n");
        INDICATE_GUI_COMMS_ERROR;
        return bytes_used;
    }

    switch(block.msg_type)
    {
#ifdef PIC32
        case READ_COMMAND:
            // These messages will be received on the stim
            STIM_GUI_PIC_ACTION_short_block_read(&block);
            break;
        case WRITE_COMMAND:
            STIM_GUI_PIC_ACTION_short_block_write(&block);
            break;
        case READ_RESPONSE:
        case WRITE_RESPONSE:
            DEBUG_Print("Stimulator responses are not expected to be received on the stim.\n");
            INDICATE_GUI_COMMS_ERROR;
            break;
#else
        case READ_COMMAND:
            DEBUG_Print("Stimulator read commands are not expected to be received at the PC.\n");
            break;
        case WRITE_COMMAND:
            DEBUG_Print("Stimulator write commands are not expected to be received at the PC.\n");
            break;
        case READ_RESPONSE:
        case WRITE_RESPONSE:
            // These messages will be received on the PC.
            STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock(&block);
            break;
#endif
    }
    return bytes_used;
}

static size_t process_stimulator_long_block(const size_t block_no, uint8_t *const start, uint8_t *const limit)
{
    if(limit<start+STIM_GUI_PROTOCOL_L_BLOCK_HEADER_SIZE)
    {
        DEBUG_Print("Block %d (long type) is impossibly short.\n", block_no);
        return limit-start; // consume all bytes as this message is broken.
    }
    
    STIM_GUI_MESSAGE_L_BLOCK_T block;
    size_t bytes_used;

    bytes_used=get_long_block_fields(&block, start);
    if(limit-start<bytes_used)
    {
        DEBUG_Print("Block %d (long) claims to have %d bytes but the message doesn't have this many left.\n", block_no, block.data_length);
        return limit-start; // say we used all available bytes.
    }

    if(block.data)
    {
        DEBUG_Print("Long block data:  (%s of %d bytes, register %d)\n", stim_gui_msg_type_strings[block.msg_type], block.data_length, block.reg_address);
        DEBUG_WriteBytes(block.data, block.data_length, true);
        DEBUG_Print("\n");
    }
    
#ifdef PIC32
    STIM_GUI_PIC_ACTION_stimulator_long_block(&block);
#endif

    return bytes_used;
}

bool skip_this_message(const uint8_t *msg, const size_t len)
{
    if(len>6)
    {
        if(STIM_GUI_PROTOCOL_RX_DISCARD_SENSOR_DATA_PACKETS && ((msg[3]&0xf0)==0xb0) && strncmp((char*)msg+4, "\x80\x28", 2)==0)
            return true;
        if(STIM_GUI_PROTOCOL_RX_DISCARD_TIME_PACKETS && strncmp((char*)msg+3, "\x80\x0c\x04", 3)==0)
            return true;
        if(STIM_GUI_PROTOCOL_RX_DISCARD_PULSE_WIDTH_PACKETS && ((msg[3]&0xf0)==0xa0) && strncmp((char*)msg+4, "\x10\x02", 2)==0)
            return true;
    }
    return false;
}

/**
 * Check the checksum and process the blocks in a stim/gui message
 * @param msg Buffer containing all unescaped bytes except start and stop.
 * @param len Number of bytes in the buffer
 */
static void process_stim_gui_message(uint8_t *const msg, const size_t len)
{
    if(len < STIM_GUI_PROTOCOL_RX_PrintReceivedHexIfLessThanNBytes)
    {
        int i;
        CLOCK_T now={0,0};
//skip        CLOCK_GetTheTime(&now);
        DEBUG_Print("\nMessage received (%d bytes, %d.%3.3ds)", len, now.seconds, now.milliseconds);
        for(i=0; i<len; i++)
        {
            DEBUG_Print(" %.2X", msg[i]);
        }
    }
    
    if(len<10)
    {
        DEBUG_Print("\nMessage impossibly short: %d bytes.\n", len);
        INDICATE_GUI_COMMS_ERROR;
        return;
    }

    uint32_t crc=crc32(0, msg, len);
    if(crc!=CRC32_VALID)
    {
        DEBUG_Print("\nChecksum Error (0x%.8x).\n", crc);
        INDICATE_GUI_COMMS_ERROR;
        return;
    }

    uint8_t destination =(msg[0]>>4)&3;
    uint8_t source = msg[0]&3;
    uint8_t counter = msg[1]&0x3f;
    milliseconds = (msg[2]<<2) | (msg[1]>>6);

    if(skip_this_message(msg, len))
        return;
    
    if(!IS_STATUS_READ(msg) && debugOptions.print_message_headers)
        DEBUG_Print("\nMessage (%d bytes)\tDestination:%d\tSource:%d\tCounter:%d\tTimestamp:%d\n", len, destination, source, counter, milliseconds);

    uint8_t *p=msg+3;
    uint8_t *limit=msg+len-4;
    size_t block_no=0;
    while(p<limit)
    {
        switch(*p & 0x3f)
        {
            case 1:// testing for stimulator long blocks: TOPIC=STIMULATOR and INDEX=1
                p+=process_stimulator_long_block(block_no++, p, limit);
                break;
            // Cases 2-15 are not currently used, but could be added in future for a different kind of block. For now handle as though they are short blocks.
            default:
                // Case 0 and all cases above 15 are short blocks.
                p+=process_stim_gui_short_block(block_no++, p, limit);
        }
    }
}

static uint8_t GUI_rx_message_buf[MAX_MESSAGE_LEN_FROM_STIM];
static size_t GUI_rx_msg_len=0;

/**
 * This forms the received bytes into messages, before validating and decoding them.
 * @param buf Buffer of some more received bytes
 * @param len How many bytes are in the buffer
 */
void STIM_GUI_PROTOCOL_Process_Received_Bytes(uint8_t *buf, size_t len)
{
    static bool in_message=false;
    static bool is_escaped=false;

    while(len--)
    {
#ifdef ENABLE_PRINT_ALL_RX_GUI_BYTES
        if(STIM_GUI_PROTOCOL_RX_PrintReceivedHexIfLessThanNBytes)
            DEBUG_Print(" %.2X", *buf);
#endif
        switch(*buf)
        {
            case ESCAPE_BYTE:
                is_escaped=true;
                break;
            case START_OF_MESSAGE_BYTE:
                GUI_rx_msg_len=0;
                in_message=true;
                break;
            case END_OF_MESSAGE_BYTE:
                if(in_message)
                {
                    process_stim_gui_message(GUI_rx_message_buf, GUI_rx_msg_len);                    
                    in_message=false;
                }
                break;
            default:
                if(is_escaped)
                {
                    *buf = *buf ^ XOR_BYTE;
                    is_escaped=false;
                }
                if(in_message)
                {
                    if(GUI_rx_msg_len<MAX_MESSAGE_LEN_FROM_STIM)
                    {
                        GUI_rx_message_buf[GUI_rx_msg_len++]=*buf;
                    }
                    else // we have run out of room for this message: discard the rest.
                    {
                        DEBUG_Print("Truncating long stim/gui message.\n");
                        in_message=false;
                    }
                }
                break;
        }
        buf++;
    }
}

/**
 * Try to reload the stimulator configuration file (if one exists)
 * @return false if unable to complete the request at this time. Note that returning true does not mean there is a stored configuration.
 */
bool STIM_GUI_PROTOCOL_ReloadConfig(void)
{
#if PIC32
    if(GUI_rx_msg_len!=0) // check the GUI_rx_msg_len is zero because we're going to (mis)use the receive buffer as a temporary place to place the config file when read from EEPROM
    {                     // It really should be empty because we only call this function once, before we start reading from the GUI UART.
        DEBUG_Print("Can't reload configuration file because GUI message buffer is not empty...\n");
        return false;
    }
    
    STIM_GUI_MESSAGE_L_BLOCK_T block;

    block.msg_type=WRITE_COMMAND;
    block.reg_address=STIM_LONG_REG_STIM_CONFIG_FILE; // volatile register used (to avoid re-writing the configuration file over the top of itself)
    block.data=GUI_rx_message_buf; // (mis)use the large buffer available).

    DEBUG_Print("Searching for stored configuration file...\n");
    block.data_length=EEPROM_LoadWithChecksum(EEPROM_ID_CONFIG_FILE, EEPROM_ADDR_CONFIG_FILE, block.data, MAX_MESSAGE_LEN_FROM_STIM, false);

    if(block.data_length)
    {
        DEBUG_Print("Reloading %d byte configuration file:\n%.*s\n", block.data_length, block.data);
        STIM_GUI_PIC_ACTION_stimulator_long_block(&block);
    }
    else
    {
        DEBUG_Print("No configuration file found in EEPROM\n");
    }
#else
    DEBUG_Print("Skipping reload of configuration file\n");
#endif
    return true;
}

#ifdef __cplusplus
}
#endif
