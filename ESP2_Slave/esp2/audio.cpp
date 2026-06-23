#include "audio.h"
#include <Arduino.h>

HardwareSerial mySerial(2);
DFRobotDFPlayerMini player;

void audio_setup() {
  mySerial.begin(9600, SERIAL_8N1, MP3_RX, MP3_TX);

  if (player.begin(mySerial, false)) {  // false = no ACK, fixes 3rd-play failure
    Serial.println("DFPlayer OK");
    player.volume(AUDIO_VOLUME);
  } else {
    Serial.println("DFPlayer FAIL");
  }
}

void audio_drain() {
  player.available();
}

void audio_play1() {
  player.play(TRACK_1);
  Serial.println("Playing track 1");
}

void audio_play2() {
  player.play(TRACK_2);
  Serial.println("Playing track 2");
}

void audio_play3() {
  player.play(TRACK_3);
  Serial.println("Playing track 3");
}

void audio_play4() {
  player.play(TRACK_4);
  Serial.println("Playing track 4");
}

void audio_play5() {
  player.play(TRACK_5);
  Serial.println("Playing track 5");
}

void audio_play6() {
  player.play(TRACK_6);
  Serial.println("Playing track 6");
}

void audio_play7() {
  player.play(TRACK_7);
  Serial.println("Playing track 7");
}

void audio_play8() {
  player.play(TRACK_8);
  Serial.println("Playing track 8");
}

void audio_stop() {
  player.stop();
  Serial.println("Audio stopped");
}

void audio_setVolume(int vol) {
  vol = constrain(vol, 0, 30);
  player.volume(vol);
  Serial.print("Volume: ");
  Serial.println(vol);
}