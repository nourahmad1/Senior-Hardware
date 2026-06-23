#include "screen.h"
#include "espnow_master.h"
#include "espnow_master_ext.h"
#include "ble.h"
#include "motors.h"
#include "robot.h"

void setup()
{
    Serial.begin(115200);
    delay(5000);

    screen_setup();

    motors_setup();

    espnow_master_setup();
    espnow_master_registerRecvCb();   // enable receiving sensor data from ESP2

    ble_setup();

    robot_init(1, 2, NORTH);

    screen_showHappy();
    delay(1500);
    screen_setup();
}

void loop()
{
   // ble_loop();
  /*  sendCommand("LR");
    delay(2000);

    sendCommand("LG");
    delay(2000);

    sendCommand("LB");
    delay(2000);

    sendCommand("SRGB");
    delay(5000);

    sendCommand("PL1");
    delay(6000);

   sendCommand("PL2");
    delay(6000);

    sendCommand("SP");
    delay(3000);*/
}
