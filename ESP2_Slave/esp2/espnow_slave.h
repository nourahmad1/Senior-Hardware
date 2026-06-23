#pragma once
#include <WiFi.h>
#include <esp_now.h>
#include "rgb.h"
#include "audio.h"
#include "commands.h"

extern String pendingCmd;
void espnow_slave_setup();