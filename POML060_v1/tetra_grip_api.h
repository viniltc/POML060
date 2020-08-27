#ifndef TETRA_GRIP_API_H
#define TETRA_GRIP_API_H

#include <QObject>
#include "Stim_includes/stim_gui_protocol.h"
#include "Stim_includes/stim_gui_protocol_rx.h"
#include "Stim_includes/stim_gui_protocol_tx.h"
#include "Stim_includes/uart.h"
#include "Stim_includes/clock.h"
#include "Stim_includes/debug.h"
#include "Stim_includes/sensor_message_format.h"
#include "Stim_includes/stim_gui_protocol_decode.h"
#include "Stim_includes/stim_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>

#define _CRT_SECURE_NO_DEPRECATE

class tetra_grip_api : public QObject
{
    Q_OBJECT

public:
    explicit tetra_grip_api(QObject *parent = nullptr);
    //bool remote_mode=false;

    #define MAX_CONFIG_FILE_LENGTH (10000)


     void static send_config_file(QByteArray config, bool nonvolatile);
     void static send_long_register(uint8_t, uint32_t, uint8_t *);
     void static clear_stim_config(void);
     bool static send_short_block(STIM_GUI_MESSAGE_S_BLOCK_T *pblock);
     void static stimulation_pause(bool);
     void static stimulation_start(bool);
     void static toggle_pause(void);
     void static read_stim_status_reg(void);
     void static get_battery_percentage(void);
     void static stimulation_set_current(unsigned int, unsigned int);
     void static get_target_current_channel(uint8_t);
     void static send_event(uint8_t sub_activity_id, uint8_t event);
     void static jump_to_phase(uint8_t sub_activity_id, uint8_t phase);
     void static set_stimulation_target_pulse_width(unsigned int channel_number, unsigned int phase_number, unsigned int pulse_width_us);
     void static set_stimulation_ramp_rate(unsigned int channel_number, unsigned int phase_number, unsigned int ramp_rate_ns);
     //for sensors
     void static reset_sensors(uint8_t sensor_address);
     void static set_sensor_data_rate(uint8_t sensor_address, uint8_t Hz);
     void static sensor_led(uint8_t sensor_address, bool on);



     QSerialPort *serial = nullptr;

     QTimer autoConnectionTimer;
     bool tryToAutoconnect;

signals:

     void successfullyConnectedTo(QString portName);
     void AutoconnectionIsSet(bool value);
     void deviceError(bool);
     void tetraGripEvent(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value);
     void tetraGripSensorEvent(uint8_t index, SENSOR_DATA_T *sample);
     void tetraGripAccelSensorEvent(SENSOR_ACCEL_DATA_T *sample);
     void tetraGripSensorFilteredEvent(int16_t sensor_role, int16_t filter_output);


public slots:

     void openSerialPort();
     void closeSerialPort();
     void readData();
     bool autoconnect();
     bool connectTo(QString port);
     void setAutoconnect(bool value);


private slots:

     void ErrorHandler(QSerialPort::SerialPortError error);

private:

        QString comPortName;

};

extern tetra_grip_api api;

#endif // TETRA_GRIP_API_H
