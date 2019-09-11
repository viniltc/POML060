#include "tetra_grip_api.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <stdio.h>
#include <stdlib.h>


#define _CRT_SECURE_NO_DEPRECATE

using namespace::std;

tetra_grip_api::tetra_grip_api(QObject *parent) : QObject(parent)
{

}



bool tetra_grip_api::send_short_block(STIM_GUI_MESSAGE_S_BLOCK_T *pblock)
{
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>\nSending: ");
    return STIM_GUI_Send_message(STIMULATOR_ADDRESS1, GUI_ADDRESS, pblock);
}

void tetra_grip_api::clear_stim_config(void)
{
    STIM_GUI_MESSAGE_S_BLOCK_T block={};

    uint8_t command=STIM_COMMAND_CLEAR_STIM_CONFIG;

    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_COMMAND;
    block.data_length=1;
    block.data=&command;
    if(!send_short_block(&block))
        printf("Failed to send the command to clear stim config.\n");
}

void tetra_grip_api::send_long_register(uint8_t reg, uint32_t data_len, uint8_t *data)

{


    STIM_GUI_MESSAGE_L_BLOCK_T block={};

    if(!data)
    {
        printf("Error: null pointer argument to send_long_register().\n");
        return;
    }

    block.msg_type=WRITE_COMMAND;
    block.reg_address=reg;
    block.data_length=data_len;
    block.data=data;
    printf("Writing %d bytes to long register %d: '%*s'...\n", data_len, reg, data_len, data);


    if(STIM_GUI_Send_long_message(STIMULATOR_ADDRESS1, GUI_ADDRESS, &block))
    {
        printf("Write OK\n");
    }
    else
    {
        printf("Write Failed.\n");
    }
}

void tetra_grip_api::send_config_file(QByteArray config, bool nonvolatile)

{
      //  clear_stim_config(); >>>>>>>>>>>   check this

        if(config.length())
        {
            send_long_register((nonvolatile?STIM_LONG_REG_NV_STIM_CONFIG_FILE:STIM_LONG_REG_STIM_CONFIG_FILE), config.length(),(uint8_t *)config.data());
        }
        else
        {
            printf("Could not read any bytes from file.\n");
        }

}

void tetra_grip_api::stimulation_start(bool start)

{
    STIM_GUI_MESSAGE_S_BLOCK_T block={}; // previously it was {0}

    uint8_t en = start;
    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_ACTIVITY_STATUS;
    block.data_length=1;
    block.data=&en;
    if(!send_short_block(&block))
        printf("Failed to send %s command to stimulator.\n", start?"start":"stop");
    else
        printf("Asked stimulator to %s.\n", start?"start":"stop");
}


void tetra_grip_api::stimulation_pause(bool paused)

{
    STIM_GUI_MESSAGE_S_BLOCK_T block={}; // previously it was {0}

    uint8_t en=!paused;
    block.msg_type=WRITE_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_ACTIVITY_STATUS;
    block.data_length=1;
    block.data=&en;
    if(!send_short_block(&block))
        printf("Failed to send %s command to stimulator.\n", paused?"pause":"unpause");
    else
        printf("Asked stimulator to %s.\n", paused?"pause":"unpause");
}


void tetra_grip_api::toggle_pause(void)

{
    bool remote_mode=false;

    static uint8_t paused=true;
    if(!remote_mode)
    {
        paused=!paused;
        stimulation_pause(!paused);
    }
}

void tetra_grip_api::read_stim_status_reg(void)
{
    STIM_GUI_MESSAGE_S_BLOCK_T block={};

    block.msg_type=READ_COMMAND;
    block.topic=TOPIC_STIMULATOR;
    block.index=0;
    block.reg_address=STIM_REG_STATUS;
    block.data_length=1;
    block.data=NULL;
    if(!send_short_block(&block))
    {
        printf("Failed to send the read command for the status register.\n");
    }
}
