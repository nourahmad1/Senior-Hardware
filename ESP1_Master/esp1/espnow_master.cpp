#include "espnow_master.h"
#include "commands.h"

#include <WiFi.h>
#include <esp_now.h>

// ==========================
// Global Packet
// ==========================

CommandPacket packet;

// ==========================
// Slave MAC Address
// ==========================

uint8_t slaveAddress[] =
{
    0xF4,
    0x2D,
    0xC9,
    0x71,
    0x18,
    0xE4
};

// ==========================
// ESP-NOW Setup
// ==========================

void espnow_master_setup()
{
    Serial.println("MASTER STEP 1");

    WiFi.mode(WIFI_STA);

    Serial.println("MASTER STEP 2");

    WiFi.disconnect(true, true);
    delay(500);

    esp_err_t result = esp_now_init();

    Serial.print("RESULT = ");
    Serial.println(result);

    if (result != ESP_OK)
    {
        Serial.println("ESP FAIL");
        return;
    }

    Serial.println("MASTER STEP 3");

    esp_now_peer_info_t peerInfo = {};

    memcpy(peerInfo.peer_addr,
           slaveAddress,
           6);

    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("PEER FAIL");
        return;
    }

    Serial.println("MASTER READY");
}

// ==========================
// Send Command
// ==========================

void sendCommand(String cmd)
{
    strcpy(packet.cmd, cmd.c_str());

    esp_err_t result =
        esp_now_send(
            slaveAddress,
            (uint8_t *)&packet,
            sizeof(packet));

    Serial.print("SEND RESULT = ");
    Serial.println(result);

    Serial.print("Sent: ");
    Serial.println(cmd);
}