#ifndef ESPNOW_MASTER_EXT_H
#define ESPNOW_MASTER_EXT_H

#include <Arduino.h>
#include "commands.h"

// Sensor data received from ESP2
extern volatile SensorPacket latestSensors;
extern volatile bool         sensorsReady;

// Call this once inside espnow_master_setup(), after esp_now_init()
// Registers the receive callback for SensorPacket from ESP2
void espnow_master_registerRecvCb();

// Request fresh sensors from ESP2, wait up to timeoutMs
bool requestSensors(uint32_t timeoutMs = 400);

#endif
