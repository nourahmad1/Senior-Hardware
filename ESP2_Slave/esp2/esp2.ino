#include "rgb.h"
#include "audio.h"
#include "espnow_slave.h"

String pendingCmd = "";

void setup() {
    Serial.begin(115200);
    delay(2000);
    rgb_setup();
    audio_setup();
    espnow_slave_setup();
}

void loop() {
    audio_drain();  // drain DFPlayer response packets every iteration

    if (pendingCmd != "") {
        String cmd = pendingCmd;
        pendingCmd = "";

        if      (cmd == "LR")   { rgb_red();     delay(1000); }
        else if (cmd == "LG")   { rgb_green();   delay(1000); }
        else if (cmd == "LB")   { rgb_blue();    delay(1000); }
        else if (cmd == "LY")   { rgb_yellow();  delay(1000); }
        else if (cmd == "SRGB") { rgb_stop();    delay(1000); }
        else if (cmd == "PL1")  { audio_play1(); delay(2000); }
        else if (cmd == "PL2")  { audio_play2(); delay(2000); }
        else if (cmd == "PL3")  { audio_play3(); delay(2000); }
        else if (cmd == "PL4")  { audio_play4(); delay(2000); }
        else if (cmd == "PL5")  { audio_play5(); delay(2000); }
        else if (cmd == "PL6")  { audio_play6(); delay(2000); }
        else if (cmd == "PL7")  { audio_play7(); delay(2000); }
        else if (cmd == "PL8")  { audio_play8(); delay(2000); }
        else if (cmd == "SP")   { audio_stop();  delay(2000); }
    }
}