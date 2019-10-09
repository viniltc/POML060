

#ifdef __cplusplus
extern "C" {
#endif


#include "stim_gui_protocol_tx.h"
#include "stim_gui_protocol_decode.h"
#include "crc32.h"
#include "debug.h"
#include "uart.h"
#include "hardware.h"
#include "clock.h"
#include "tetra_grip_writer.h"

typedef struct {
    uint32_t crc;
    uint8_t stim_gui_message_counter;
} STIM_GUI_TX_T;

static STIM_GUI_TX_T gui = {0, 0};

/**
 * Initialise the transmit part of the stim-gui protocol
 */
void STIM_GUI_PROTOCOL_TX_Initialise(void)
{
    gui.stim_gui_message_counter=0;
}


#define UART_PC_BUFFER_SIZE (65536)
static char uart_pc_buffer[UART_PC_BUFFER_SIZE];
static size_t uart_pc_buffer_len=0;

// Says there is always plenty of space in the transmit buffer
size_t UART_WriteBytesAvailable(const USART_MODULE_ID id)
{
    return UART_PC_BUFFER_SIZE-uart_pc_buffer_len;
}

//extern size_t writeData(uint8_t *data, size_t len); -> check this VTC

size_t UART_Write_Out(const USART_MODULE_ID id, void * source, size_t nBytes)
{
    uint8_t *data= (uint8_t*)source;
    return tetragrip_writer(data, nBytes);
}

void UART_PC_Buffer_Flush(const USART_MODULE_ID id)
{
    char *buf_ptr=uart_pc_buffer;
    while(uart_pc_buffer_len)
    {
        size_t written=UART_Write_Out(id, buf_ptr, uart_pc_buffer_len);
        uart_pc_buffer_len-=written;
        buf_ptr+=written;
    }
}

size_t UART_PC_Buffered_Write(const USART_MODULE_ID id, char * source, size_t nBytes)
{
    size_t len=nBytes;
    while(len--)
    {
        if(uart_pc_buffer_len>=UART_PC_BUFFER_SIZE) // this is in case the buffer fills up while we are writing
            UART_PC_Buffer_Flush(id);
        uart_pc_buffer[uart_pc_buffer_len++] = *source++;
    }
    return nBytes;
}

//size_t UART_PC_Buffered_Write (const char USART_MODULE_ID id, void * source, size_t nBytes)
static void stim_gui_write_bytes(uint8_t *buf, size_t len)
{
    static uint8_t escape[2]={ESCAPE_BYTE,0};

    uint8_t *b=buf; // pointer to beginning of unsent bytes
    uint8_t *p=buf; // pointer to byte being checked for escaping

    gui.crc=crc32(gui.crc, buf, len); // update the running CRC-32 with these bytes

    // We do not want to call UART_Write for every individual byte (it's inefficient)
    // and we do not want to limit the maximum length or allocate a huge buffer for the worst case output (twice as many bytes as input, if they all have to be escaped).
    // So the strategy in this section is to call UART_Write with as many bytes as possible at once.
    // We must be aware that UART_Write might not write everything (if the tx buf is full). In that case, we will block until there is space.
    while(len--)
    {
        switch(*p)
        {
            case START_OF_MESSAGE_BYTE:
            case END_OF_MESSAGE_BYTE:
            case ESCAPE_BYTE:
                while(b!=p) // make sure we have written all non-escaped bytes so far
                {
                    b+=UART_Write(PLIB_USART_INDEX_GUI, b, p-b);
                }
                escape[1]=(*p)^XOR_BYTE;
                int done=0;
                while(done<2)
                {
                    done+=UART_Write(PLIB_USART_INDEX_GUI, &escape[done], 2-done);
                }
                b++;
                break;
            default:
                // Don't do anything with non-escaped bytes yet: they will be sent together in a call to UART_Write
                break;
        }
        p++;
    }
    while(b!=p) // finish sending any remaining bytes.
    {
        b+=UART_Write(PLIB_USART_INDEX_GUI, b, p-b);
    }
}

// Send the null for start of message.
// @Todo: connect falling edge to timestamp.
static void stim_gui_begin_message(void)
{
    uint8_t start=START_OF_MESSAGE_BYTE;
    gui.crc=0;
    while(UART_Write(PLIB_USART_INDEX_GUI, &start, 1)!=1)
    {} // wait for it to succeed
}

static void stim_gui_end_message(void)
{
    uint32_t crc=gui.crc;
    stim_gui_write_bytes((uint8_t *)&crc, 4);

    uint8_t end=END_OF_MESSAGE_BYTE;
    while(UART_Write(PLIB_USART_INDEX_GUI, &end, 1)!=1)
    {} // wait for it to succeed
}

#define HEADER_SIZE (3)
#define TIME_BLOCK_SIZE (7)

static bool send_header(const uint8_t destination_device, const uint8_t source_device, size_t block_len)
{
    static CLOCK_T last_time={0,0};
    uint8_t header[HEADER_SIZE];

    CLOCK_T now_time={0,0};
#ifdef PIC32
    CLOCK_GetTheTime(&now_time);
#endif

    if(!block_len && !(now_time.seconds>last_time.seconds))
        return false; // This stops the empty message which can occur if another message went out between the second tick and a NULL message from the health timer.

    gui.stim_gui_message_counter++; // incrementing here means that the GUI can see that packets were dropped.

    size_t max_bytes_needed = 2/*NULLS*/ + 2*(HEADER_SIZE+TIME_BLOCK_SIZE+block_len+4/*CRC*/);
    if(UART_WriteBytesAvailable(PLIB_USART_INDEX_GUI)<max_bytes_needed) // We don't try to send it if there isn't room in the output buffer
    {
        //PLIB_PORTS_PinToggle(ISTIM_SYNC); // indicate failure to send.
        return false;
    }

    stim_gui_begin_message();

    header[0]=((destination_device&7)<<4) | (source_device&7);
    header[1]=((now_time.milliseconds&0x3)<<6) | (gui.stim_gui_message_counter & 0x3f);
    header[2]=(uint8_t)(now_time.milliseconds>>2);

    stim_gui_write_bytes(header, HEADER_SIZE);

    // This makes sure that we send the STIM_REG_TIME_SECONDS register if the clock.seconds have incremented.
    if(now_time.seconds>last_time.seconds)
    {
        uint8_t update_time_reg[TIME_BLOCK_SIZE]={(READ_RESPONSE<<6) | (TOPIC_STIMULATOR<<4) | 0/*Index=0*/ , STIM_REG_TIME_SECONDS, 4};
        update_time_reg[3]=now_time.seconds&0xff;
        update_time_reg[4]=(now_time.seconds>>8)&0xff;
        update_time_reg[5]=(now_time.seconds>>16)&0xff;
        update_time_reg[6]=(now_time.seconds>>24)&0xff;
        stim_gui_write_bytes(update_time_reg, TIME_BLOCK_SIZE);
        last_time=now_time;
    }

    return true;
}

const char *topic_strings[4]={"STIM", "SUB_ACT", "CHANNEL", "SENSOR"};

/**
 * Send a message using the Stim-GUI protocol.
 * @param destination_device
 * @param source_device
 * @param blocks One or more linked short register blocks
 * @return True if message was sent
 */
bool STIM_GUI_Send_message(const uint8_t destination_device, const uint8_t source_device, STIM_GUI_MESSAGE_S_BLOCK_T *block)
{
    STIM_GUI_MESSAGE_S_BLOCK_T *p=block;
    size_t total_block_len=0;
    while(p)
    {
        total_block_len+=STIM_GUI_PROTOCOL_S_BLOCK_HEADER_SIZE+p->data_length;
        p=p->next;
    }

    if(!send_header(destination_device, source_device, total_block_len))
        return false;

    while(block)
    {
#ifdef PIC32
        if(debugOptions.guiMessagesToDebug)
            STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock(block); // conditionally print outgoing messages on the PIC
#else
        STIM_GUI_PROTOCOL_DECODE_DecodeShortBlock(block); // always print outgoing messages on the PC.
#endif // PIC32

        uint8_t s_block_header[STIM_GUI_PROTOCOL_S_BLOCK_HEADER_SIZE];
        s_block_header[0]=((block->msg_type&3)<<6) | ((block->topic&3)<<4) | (block->index&0xf);
        s_block_header[1]=block->reg_address;
        s_block_header[2]=block->data_length;
        stim_gui_write_bytes(s_block_header, STIM_GUI_PROTOCOL_S_BLOCK_HEADER_SIZE); // send the block header
        if(block->data_length && block->msg_type!=READ_COMMAND)
        {
            if(block->data)
            {
                stim_gui_write_bytes(block->data, block->data_length); // send the data
            }
            else
            {
                DEBUG_Print("STIM_GUI_Send_message: Null pointer for s-block data.\n");
            }
        }
        block=block->next;
    }

    stim_gui_end_message();
#ifndef PIC32
   UART_PC_Buffer_Flush(PLIB_USART_INDEX_GUI);
#endif
    return true;
}

/**
 * Send a single long block using the Stim-GUI protocol.
 * @param destination_device
 * @param source_device
 * @param block Pointer to one long register block
 * @return True if message was sent
 */
bool STIM_GUI_Send_long_message(const uint8_t destination_device, const uint8_t source_device, STIM_GUI_MESSAGE_L_BLOCK_T *block)
{
    if(!block)
    {
        DEBUG_Print("STIM_GUI_Send_long_message: Null pointer for l-block.\n");
        return false;
    }

    if(!send_header(destination_device, source_device, STIM_GUI_PROTOCOL_L_BLOCK_HEADER_SIZE+block->data_length))
        return false;

    uint8_t l_block_header[STIM_GUI_PROTOCOL_L_BLOCK_HEADER_SIZE];
    l_block_header[0]=((block->msg_type&3)<<6) | 1;
    l_block_header[1]=block->reg_address;
    l_block_header[2]=block->data_length & 0xff;
    l_block_header[3]=(block->data_length>>8) & 0xff;
    l_block_header[4]=(block->data_length>>16) & 0xff;
    l_block_header[5]=(block->data_length>>24) & 0xff;
    stim_gui_write_bytes(l_block_header, STIM_GUI_PROTOCOL_L_BLOCK_HEADER_SIZE); // send the block header

    if(block->data_length && block->msg_type!=READ_COMMAND)
    {
        if(block->data)
        {
            stim_gui_write_bytes(block->data, block->data_length); // send the data
        }
        else
        {
            DEBUG_Print("STIM_GUI_Send_long_message: Null pointer for l-block data.\n");
        }
    }

    stim_gui_end_message();
#ifndef PIC32
   UART_PC_Buffer_Flush(PLIB_USART_INDEX_GUI);
#endif
    return true;
}

#ifdef __cplusplus
}
#endif














































