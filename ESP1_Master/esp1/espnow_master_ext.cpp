#include "espnow_master_ext.h"
#include "espnow_master.h"
#include "commands.h"
#include <esp_now.h>
#include <Arduino.h>

volatile SensorPacket latestSensors = {-1, -1, -1, -1};
volatile bool         sensorsReady  = false;

static void _onRecv(const esp_now_recv_info_t *info,
                    const uint8_t *data, int len) {
    if (len == sizeof(SensorPacket)) {
        memcpy((void*)&latestSensors, data, sizeof(SensorPacket));
        sensorsReady = true;
        Serial.printf("SENSORS F:%.1f B:%.1f L:%.1f R:%.1f\n",
            latestSensors.d1, latestSensors.d2,
            latestSensors.d3, latestSensors.d4);
    }
}

void espnow_master_registerRecvCb() {
    esp_now_register_recv_cb(_onRecv);
}

bool requestSensors(uint32_t timeoutMs) {
    sensorsReady = false;
    sendCommand("SENSE");
    uint32_t t = millis();
    while (!sensorsReady && (millis() - t) < timeoutMs) delay(10);
    return sensorsReady;
}
