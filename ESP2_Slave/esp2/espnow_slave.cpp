#include "espnow_slave.h"

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    CommandPacket packet;
    memcpy(&packet, incomingData, sizeof(packet));
    pendingCmd = packet.cmd;
}

void espnow_slave_setup() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP FAIL");
        return;
    }
    esp_now_register_recv_cb(onReceive);
    Serial.println("SLAVE READY");
}