#ifndef ESPNOW_MASTER_H
#define ESPNOW_MASTER_H

#include <Arduino.h>

void espnow_master_setup();
void sendCommand(String cmd);

#endif