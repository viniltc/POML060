#ifndef TETRA_GRIP_API_H
#define TETRA_GRIP_API_H

#include <QObject>
#include "stim_gui_protocol.h"
#include "stim_gui_protocol_rx.h"
#include "stim_gui_protocol_tx.h"
#include "uart.h"
#include "clock.h"
#include "debug.h"
#include "sensor_message_format.h"
#include "stim_gui_protocol_rx.h"
#include "stim_gui_protocol_decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#define _CRT_SECURE_NO_DEPRECATE

class tetra_grip_api : public QObject
{
    Q_OBJECT
public:
    explicit tetra_grip_api(QObject *parent = nullptr);

    #define MAX_CONFIG_FILE_LENGTH (10000)

     void static send_config_file(QByteArray config, bool nonvolatile);
     void static send_long_register(uint8_t, uint32_t, uint8_t *);
     void static clear_stim_config(void);
     bool static send_short_block(STIM_GUI_MESSAGE_S_BLOCK_T *pblock);
     void static stimulation_pause(bool);
     void static stimulation_start(bool);




signals:

public slots:
};

#endif // TETRA_GRIP_API_H
