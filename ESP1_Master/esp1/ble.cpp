#include "ble.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "espnow_master.h"
#include "screen.h"
#include "robot.h"

// ---------------- UUIDs ----------------
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789001"
#define CHARACTERISTIC_UUID "12345678-1234-1234-1234-123456789002"

// ---------------- Global ----------------
BLECharacteristic *pCharacteristic;

// ================= SERVER CALLBACKS =================
class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        Serial.println("BLE: client connected");
    }

    void onDisconnect(BLEServer *pServer)
    {
        Serial.println("BLE: client disconnected, restarting advertising");
        BLEDevice::getAdvertising()->start();
    }
};

// ================= CHARACTERISTIC CALLBACKS =================
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String cmd = pCharacteristic->getValue();
        cmd.trim();

        if (cmd.length() == 0)
            return;

        Serial.print("BLE CMD: ");
        Serial.println(cmd);

        // -------- Screen Commands -------- (original, untouched)
        if (cmd == "SUN") {
            screen_showSun();
            delay(3000);
        } else if (cmd == "SLF") {
            screen_showSmallLeaf();
            delay(3000);
        } else if (cmd == "LF") {
            screen_showLeaves();
            delay(3000);
        } else if (cmd == "SFL") {
            screen_showSunflower();
            delay(3000);
        } else if (cmd == "HPY") {
            screen_showHappy();
            delay(3000);
        } else if (cmd == "SAD") {
            screen_showSad();
            delay(3000);
        } else if (cmd == "D0") {
            screen_showNumber(0);
            delay(3000);
        } else if (cmd == "D1") {
            screen_showNumber(1);
            delay(3000);
        } else if (cmd == "D2") {
            screen_showNumber(2);
            delay(3000);
        } else if (cmd == "D3") {
            screen_showNumber(3);
            delay(3000);
        } else if (cmd == "D5") {
            screen_showNumber(5);
            delay(3000);
        } else if (cmd == "D10") {
            screen_showNumber(10);
            delay(3000);
        } else if (cmd == "CLR") {
            screen_setup();
            delay(3000);

        // -------- Robot Movement Commands -------- (new)
        } else if (cmd == "MF" || cmd == "MB" || cmd == "TR" || cmd == "TL") {
            robot_execute(cmd);

        // -------- Everything else → ESP2 via ESP-NOW -------- (original)
        } else {
            sendCommand(cmd);
        }
    }
};

// ================= SETUP ================= (original, untouched)
void ble_setup()
{
    Serial.begin(115200);

    BLEDevice::init("RoboLearn");

    BLEServer *server = BLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

    pCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new MyCallbacks());

    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    Serial.println("BLE READY - RoboLearn is advertising");
}

// ================= LOOP ================= (original, untouched)
void ble_loop()
{
    // nothing needed
}
